// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Types/EnumTypes.h"
#include "GameMode_Base.generated.h"

/**
 * 
 */
UCLASS()
class GASPRE_API AGameMode_Base : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGameMode_Base();

protected:
	//游戏难度成员变量
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GameMode")
	ESurvivalGameDifficuty GameDifficulty;

public:
	//暴露给外面的获取游戏难度的函数
	FORCEINLINE ESurvivalGameDifficuty GetGameDifficulty() const { return GameDifficulty; }
};
