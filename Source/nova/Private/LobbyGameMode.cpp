#include "LobbyGameMode.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "PlayerCharacterSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Engine/Texture2D.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "OnlineSubsystemUtils.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Interfaces/OnlineIdentityInterface.h"


ALobbyGameMode::ALobbyGameMode(): CurrentPlayer()
{
    bIsPrivate = false;
    bIsGameStarted = false;
}

void ALobbyGameMode::CreateLobby(const bool bIsPrivate)
{
    this->LobbyID = GenerateLobbyID();
    this->bIsPrivate = bIsPrivate;
}

void ALobbyGameMode::JoinLobby(FString ID)
{
    this->LobbyID = ID;
}

void ALobbyGameMode::LeaveLobby()
{
    this->LobbyID.Empty();
}

void ALobbyGameMode::SetPlayerReady(const bool bIsReady)
{
    if (const APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
    {
        PlayerReady.Add(PlayerController->GetName(), bIsReady);
    }
}

void ALobbyGameMode::StartGame()
{
    // Check if all players are ready
    for (const auto& PlayerStatus : PlayerReady)
    {
        if (!PlayerStatus.Value)
        {
            return; // Not all players are ready
        }
    }

    // Start the game
    bIsGameStarted = true;
    // Implement game start logic here
}

void ALobbyGameMode::SavePlayerCharacters()
{
    if (UPlayerCharacterSaveGame* SaveGameInstance = Cast<UPlayerCharacterSaveGame>(UGameplayStatics::CreateSaveGameObject(UPlayerCharacterSaveGame::StaticClass())))
    {
        // Populate SaveGameInstance with data from PlayerData
        SaveGameInstance->PlayerCharacters.Empty();
        for (const auto& Entry : PlayerData)
        {
            SaveGameInstance->PlayerCharacters.Add(Entry.Value);
        }

        UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("PlayerCharacterSaveSlot"), 0);
    }
}

TMap<FString, FPlayerCharacterData> ALobbyGameMode::LoadPlayerCharacters()
{
    UPlayerCharacterSaveGame* LoadGameInstance = Cast<UPlayerCharacterSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("PlayerCharacterSaveSlot"), 0));

    if (LoadGameInstance)
    {
        for (const auto& Entry : LoadGameInstance->PlayerCharacters)
        {
            PlayerData.Add(Entry.CharacterName, Entry);
        }
    }
    return PlayerData;
}

FString ALobbyGameMode::GenerateLobbyID()
{
    return FString::Printf(TEXT("%08X"), FMath::Rand());
}

bool ALobbyGameMode::AddNewPlayer(FPlayerCharacterData playerData)
{
    if (PlayerData.Contains(playerData.CharacterName))
    {
        return false;
    }

    PlayerData.Add(playerData.CharacterName, playerData);

    SavePlayerCharacters();
    NotifyPlayerJoined(playerData);
    return true;
}

bool ALobbyGameMode::RemovePlayerByName(FText name)
{
    if (const FString PlayerName = name.ToString(); PlayerData.Contains(PlayerName))
    {
        const FPlayerCharacterData RemovedPlayer = PlayerData[PlayerName];
        PlayerData.Remove(PlayerName);
        SavePlayerCharacters(); // Save the updated player data
        NotifyPlayerLeft(RemovedPlayer);
        return true;
    }
    return false;

}

FString ALobbyGameMode::GetSteamUsername()
{
    if (const IOnlineSubsystem* OS = Online::GetSubsystem(GetWorld())) {
        const IOnlineIdentityPtr Identity = OS->GetIdentityInterface();
        if (Identity.IsValid()) {
            TSharedPtr<const FUniqueNetId> Id = Identity->GetUniquePlayerId(0);
            if (Id.IsValid()) {
                UserId = Id;
                return Identity->GetPlayerNickname(*UserId);
            }
        }
    }
    return FString();
}

UTexture2D* ALobbyGameMode::GetSteamAvatar()
{
    IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
    if (OnlineSubsystem)
    {
        IOnlineIdentityPtr Identity = OnlineSubsystem->GetIdentityInterface();
        if (Identity.IsValid())
        {
            TSharedPtr<const FUniqueNetId> LocalUserId = Identity->GetUniquePlayerId(0);
            if (LocalUserId.IsValid())
            {
                int32 AvatarSize = 64; // Tamanho do avatar (32, 64, 184)
                FString AvatarUrl;
                TSharedPtr<FUserOnlineAccount> UserAccount = Identity->GetUserAccount(*LocalUserId);
                if (UserAccount.IsValid())
                {
                    UserAccount->GetUserAttribute(TEXT("avatar"), AvatarUrl);
                }
                if (!AvatarUrl.IsEmpty())
                {
                    TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
                    HttpRequest->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
                    {
                        if (bWasSuccessful && Response.IsValid() && Response->GetContentLength() > 0)
                        {
                            TArray<uint8> ImageData = Response->GetContent();
                            IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
                            TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);
                            if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(ImageData.GetData(), ImageData.Num()))
                            {
                                TArray<uint8> UncompressedBGRA;
                                if (ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, UncompressedBGRA))
                                {
                                    UTexture2D* AvatarTexture = UTexture2D::CreateTransient(ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), PF_B8G8R8A8);
                                    if (AvatarTexture)
                                    {
                                        void* TextureData = AvatarTexture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
                                        FMemory::Memcpy(TextureData, UncompressedBGRA.GetData(), UncompressedBGRA.Num());
                                        AvatarTexture->GetPlatformData()->Mips[0].BulkData.Unlock();
                                        AvatarTexture->UpdateResource();
                                        // Process the avatar texture here
                                    }
                                }
                            }
                        }
                    });
                    HttpRequest->SetURL(AvatarUrl);
                    HttpRequest->SetVerb(TEXT("GET"));
                    HttpRequest->ProcessRequest();
                }
            }
        }
    }
    return nullptr;
}

void ALobbyGameMode::NotifyPlayerJoined(const FPlayerCharacterData& Data)
{
    OnPlayerJoined.Broadcast(Data);
}

void ALobbyGameMode::NotifyPlayerLeft(const FPlayerCharacterData& Data)
{
    OnPlayerLeft.Broadcast(Data);
}

