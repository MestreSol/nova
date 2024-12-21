// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
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

    // Adicione outros atributos conforme necessário
};


