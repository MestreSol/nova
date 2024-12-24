#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PlayerCharacterData.h"
#include "LobbyGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerChanged, const FPlayerCharacterData&, PlayerData);
UCLASS()
class NOVA_API ALobbyGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ALobbyGameMode();

    UFUNCTION(BlueprintCallable, Category = "Lobby")
    void CreateLobby(bool bIsPrivate);

    UFUNCTION(BlueprintCallable, Category = "Lobby")
    void JoinLobby(FString ID);

    UFUNCTION(BlueprintCallable, Category = "Lobby")
    void LeaveLobby();

    UFUNCTION(BlueprintCallable, Category = "Lobby")
    void SetPlayerReady(bool bIsReady);

    UFUNCTION(BlueprintCallable, Category = "Lobby")
    void StartGame();

    UFUNCTION(BlueprintCallable, Category = "Lobby")
    void SavePlayerCharacters();

    UFUNCTION(BlueprintCallable, Category = "Save")
    TMap<FString, FPlayerCharacterData> LoadPlayerCharacters();

    UFUNCTION(BlueprintCallable, Category = "Save")
    bool AddNewPlayer(FPlayerCharacterData playerData);

    UFUNCTION(BlueprintCallable, Category = "Save")
    bool RemovePlayerByName(FText name);

    UFUNCTION(BlueprintCallable, Category = "Steam")
    FString GetSteamUsername();

    UFUNCTION(BlueprintCallable, Category = "Steam")
    UTexture2D* GetSteamAvatar();
    void NotifyPlayerJoined(const FPlayerCharacterData& Data);
    void NotifyPlayerLeft(const FPlayerCharacterData& Data);

private:
    static FString GenerateLobbyID();

    FString LobbyID;
    bool bIsPrivate;
    bool bIsGameStarted;
    TMap<FString, bool> PlayerReady;
    TMap<FString, FPlayerCharacterData> PlayerData;
    FPlayerCharacterData CurrentPlayer;
    TSharedPtr<const FUniqueNetId> UserId; // Adicionado para armazenar o UserId
    
public:
    FOnPlayerChanged OnPlayerJoined;
    FOnPlayerChanged OnPlayerLeft;
    
};






