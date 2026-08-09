// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameplayTagContainer.h"

#include "WarriorGameInstance.generated.h"

//关卡结构体，与Tag相联
USTRUCT(BlueprintType)
struct FWarriorGameLevel
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly,meta = (Categories = "GameData.Level"))
	FGameplayTag LevelTag;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> Level;

	bool IsValid() const
	{
		return LevelTag.IsValid() && !Level.IsNull();
	}
};

/**
 * 
 */
UCLASS()
class GASPRE_API UWarriorGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;

protected:
	//游戏实例中保存的关卡数组
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<FWarriorGameLevel> GameLevelSets;

	virtual void OnPreLoadMapFunction(const FString& MapName);
	virtual void OnDestinationWorldLoaded(UWorld* LoadedWorld);
	

public:
	//通过Tag获取关卡
	UFUNCTION(BlueprintPure,meta = (GameplayTagFilter = "GameData.Level"))
	TSoftObjectPtr<UWorld> GetGameLevelByTag(FGameplayTag InTag); 

};
