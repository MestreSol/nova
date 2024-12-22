#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PlayerCharacterData.h"
#include "LobbyGameMode.generated.h"

UCLASS()
class NOVA_API ALobbyGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ALobbyGameMode();

    UFUNCTION(BlueprintCallable, Category = "Lobby")
    void CreateLobby(bool isPrivate);

    UFUNCTION(BlueprintCallable, Category = "Lobby")
    void JoinLobby(FString ID);

    UFUNCTION(BlueprintCallable, Category = "Lobby")
    void LeaveLobby();

    UFUNCTION(BlueprintCallable, Category = "Lobby")
    void SetPlayerReady(bool isReady);

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

private:
    FString GenerateLobbyID();

    FString LobbyID;
    bool bIsPrivate;
    bool bIsGameStarted;
    TMap<FString, bool> PlayerReady;
    TMap<FString, FPlayerCharacterData> PlayerData;
    FPlayerCharacterData CurrentPlayer;
};

