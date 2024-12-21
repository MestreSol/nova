// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LobbyGameMode.generated.h"

UCLASS()
class NOVA_API ALobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ALobbyGameMode();

	TArray<APlayerController*> Players;
	TMap<FString, bool> PlayerReady;
	FString LobbyID;
	FString LobbyOwner;
	bool IsPrivate;
	bool IsGameStarted;
	FString GenerateLobbyID();

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

};
