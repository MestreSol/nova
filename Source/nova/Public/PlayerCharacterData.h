#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacterData.generated.h"

USTRUCT(BlueprintType)
struct FPlayerCharacterData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
    FString CharacterName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
    int32 Level;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
    FString Class;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Atributs")
    int32 STR;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Atributs")
    int32 CON;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Atributs")
    int32 SIZ;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Atributs")
    int32 DEX;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Atributs")
    int32 APP;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Atributs")
    int32 INT;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Atributs")
    int32 POW;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Atributs")
    int32 EDU;
};









