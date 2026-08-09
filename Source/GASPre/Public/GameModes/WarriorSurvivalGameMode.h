// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameModes/GameMode_Base.h"
#include "WarriorSurvivalGameMode.generated.h"

class ACharacter_Enemy;

//创建一个游戏状态枚举
UENUM(BlueprintType)
enum class EWarriorSurvivalGameState : uint8
{
	WaitSpwanNewWave,
	SpawningNewWave,
	InProgress,
	WaveComplete,
	AllWavesDown,
	PlayerDied
};

//特定种类敌人生成器
USTRUCT(BlueprintType)
struct FWarriorEnemyWaveSpawnerInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSoftClassPtr<class ACharacter_Enemy> SoftEnemyClassToSpawn;

	UPROPERTY(EditAnywhere)
	int32 MinEnemyCount = 1;

	UPROPERTY(EditAnywhere)
	int32 MaxEnemyCount = 3;
};

//波次敌人生成器
USTRUCT(BlueprintType)
struct FWarriorEnemyWaveSpawnerTableRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<FWarriorEnemyWaveSpawnerInfo> EnemyWaveSpawnerInfos;

	UPROPERTY(EditAnywhere)
	int32 TotalEnemyCount = 3;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSurvivalGameStateChangedDelegate, EWarriorSurvivalGameState, CurrentState);

/**
 * 
 */
UCLASS()
class GASPRE_API AWarriorSurvivalGameMode : public AGameMode_Base
{
	GENERATED_BODY()

protected:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	void SetCurrentGameState(EWarriorSurvivalGameState InState);

	bool HasFinishedAllWaves() const;

	void PreLoadNextWaveEnemy();

	FWarriorEnemyWaveSpawnerTableRow* FindCurrentWaveSpawnerRow() const;

	int32 TrySpawnWaveEnemies();

	bool ShouldKeepSpawnEnemy() const;

	//创建一个敌人销毁时的回调函数
	UFUNCTION()
	void OnEnemyDied(AActor* DeadActor);

	//当前游戏状态
	UPROPERTY()
	EWarriorSurvivalGameState CurrentGameState;
	//定义游戏状态改变的委托，暴露给蓝图以供蓝图更新UI
	UPROPERTY(BlueprintAssignable,BlueprintCallable)
	FOnSurvivalGameStateChangedDelegate OnSurvivalGameStateChanged;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category="WaveDefinition",meta=(AllowPrivateAccess="true"))
	UDataTable* EnemyWaveSpawnerTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="WaveDefinition", meta=(AllowPrivateAccess="true"))
	int32 TotalWavesToSpawn;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="WaveDefinition", meta=(AllowPrivateAccess="true"))
	int32 CurrentWaveCount = 1;

	//当前波次在场敌人的数量 会随着敌人的死亡更新
	UPROPERTY()
	int32 CurrentSpawnedEnemiesCounter = 0;
	//当前波次总生成的敌人数量 会随着敌人的生成更新
	UPROPERTY()
	int32 TotalSpawnedEnemiesThisWaveCounter = 0;

	UPROPERTY()
	TArray<AActor*> TargetPointArray;

	UPROPERTY()
	float TimePassedSinceStart = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="WaveDefinition", meta=(AllowPrivateAccess="true"))
	float SpawnNewWaveWaitTime = 5.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="WaveDefinition", meta=(AllowPrivateAccess="true"))
	float SpawnEnemiesDelayTime = 2.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="WaveDefinition", meta=(AllowPrivateAccess="true"))
	float WaveCompletedWaitTime = 5.f;

	UPROPERTY()
	TMap< TSoftClassPtr < ACharacter_Enemy >, UClass*> PreLoadedEnemyClassMap;
};
