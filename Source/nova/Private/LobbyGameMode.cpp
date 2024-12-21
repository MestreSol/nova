#include "LobbyGameMode.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"

ALobbyGameMode::ALobbyGameMode()
{
    // Set default values
    IsPrivate = false;
    IsGameStarted = false;
}

void ALobbyGameMode::CreateLobby(bool bIsPrivate)
{
    // Implement Steam lobby creation logic here
    LobbyID = GenerateLobbyID();
    IsPrivate = bIsPrivate;
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

void ALobbyGameMode::SetPlayerReady(bool bIsReady)
{
    // Set the player as ready
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (PlayerController)
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
    IsGameStarted = true;
    // Implement game start logic here
}

FString ALobbyGameMode::GenerateLobbyID()
{
    // Generate a unique lobby ID
    return FString::Printf(TEXT("%08X"), FMath::Rand());
}
