#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PlayerCharacterData.h"
#include "PlayerCharacterSaveGame.generated.h"

UCLASS()
class NOVA_API UPlayerCharacterSaveGame : public USaveGame
{
    GENERATED_BODY()

public:
    UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
    TArray<FPlayerCharacterData> PlayerCharacters;
};
