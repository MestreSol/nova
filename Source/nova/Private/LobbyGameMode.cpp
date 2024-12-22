#include "LobbyGameMode.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "PlayerCharacterSaveGame.h"
#include "Kismet/GameplayStatics.h"

ALobbyGameMode::ALobbyGameMode()
{
    // Set default values
    bIsPrivate = false;
    bIsGameStarted = false;
}

void ALobbyGameMode::CreateLobby(bool isPrivate)
{
    // Implement Steam lobby creation logic here
    this->LobbyID = GenerateLobbyID();
    this->bIsPrivate = isPrivate;
    // Store lobby code and other details
}

void ALobbyGameMode::JoinLobby(FString ID)
{
    // Implement Steam lobby join logic here
    this->LobbyID = ID;
}

void ALobbyGameMode::LeaveLobby()
{
    // Implement Steam lobby leave logic here
    this->LobbyID.Empty();
}

void ALobbyGameMode::SetPlayerReady(bool isReady)
{
    // Set the player as ready
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (PlayerController)
    {
        PlayerReady.Add(PlayerController->GetName(), isReady);
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
    UPlayerCharacterSaveGame* SaveGameInstance = Cast<UPlayerCharacterSaveGame>(UGameplayStatics::CreateSaveGameObject(UPlayerCharacterSaveGame::StaticClass()));
    if (SaveGameInstance)
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
    return true;
}

bool ALobbyGameMode::RemovePlayerByName(FText name)
{
    FString PlayerName = name.ToString();
    if (PlayerData.Contains(PlayerName))
    {
        PlayerData.Remove(PlayerName);
        SavePlayerCharacters(); // Save the updated player data
        return true;
    }
    return false;
}
