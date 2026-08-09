// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Types/EnumTypes.h"

#include "WarriorSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class GASPRE_API UWarriorSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	//保存的游戏难度
	UPROPERTY(BlueprintReadOnly)
	ESurvivalGameDifficuty SavedCurrentGameDifficuty;

};
