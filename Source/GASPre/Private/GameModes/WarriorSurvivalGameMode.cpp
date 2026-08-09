// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/WarriorSurvivalGameMode.h"
#include "Engine/AssetManager.h"
#include "Characters/Character_Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/TargetPoint.h"
#include "NavigationSystem.h"
#include "FunctionLibrary.h"

#include "DebugHelpper.h"

void AWarriorSurvivalGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
    Super::InitGame(MapName, Options, ErrorMessage);

    //从保存的游戏难度中加载游戏难度
    ESurvivalGameDifficuty LoadedGameDifficuty;
    if(UFunctionLibrary::LoadSavedGameDifficuty(LoadedGameDifficuty))
    {
        GameDifficulty = LoadedGameDifficuty;
        //Debug::Print(FString::Printf(TEXT("Loaded Game Difficuty : %s"),*UEnum::GetValueAsString(GameDifficulty)),FColor::Green);
    }
}


void AWarriorSurvivalGameMode::BeginPlay()
{
    Super::BeginPlay();

    checkf(EnemyWaveSpawnerTable, TEXT("EnemyWaveSpawnerTable is not set in Blueprint"));

    SetCurrentGameState(EWarriorSurvivalGameState::WaitSpwanNewWave);

    TotalWavesToSpawn = EnemyWaveSpawnerTable->GetRowNames().Num();

    PreLoadNextWaveEnemy();
}

void AWarriorSurvivalGameMode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    //等待生成敌人阶段，时间用来显示一些UI
    if(CurrentGameState == EWarriorSurvivalGameState::WaitSpwanNewWave)
    {
        TimePassedSinceStart += DeltaTime;
        if(TimePassedSinceStart >= SpawnNewWaveWaitTime)
        {
            TimePassedSinceStart = 0.f;
            SetCurrentGameState(EWarriorSurvivalGameState::SpawningNewWave);
        }
    }
    //生成敌人阶段  等几秒然后生成敌人
    if(CurrentGameState == EWarriorSurvivalGameState::SpawningNewWave)
    {
        TimePassedSinceStart += DeltaTime;
        if(TimePassedSinceStart >= SpawnEnemiesDelayTime)
        {
            TimePassedSinceStart = 0.f;
            
            //TODO:Spawn New Enemies
            CurrentSpawnedEnemiesCounter += TrySpawnWaveEnemies();

            SetCurrentGameState(EWarriorSurvivalGameState::InProgress);
        }
    }
    //WaveComplete阶段 等待几秒显示UI然后判断进行下一波此或者结束游戏
    if(CurrentGameState == EWarriorSurvivalGameState::WaveComplete)
    {
        //等待一段时间
        TimePassedSinceStart += DeltaTime;
        if(TimePassedSinceStart >= WaveCompletedWaitTime)
        {
            TimePassedSinceStart = 0.f;
            ++CurrentWaveCount;
            //进行下一波次或者结束游戏
            if(HasFinishedAllWaves())
            {
                SetCurrentGameState(EWarriorSurvivalGameState::AllWavesDown);
            }
            else
            {
                //进行下一波次并预加载下一波次的敌人
                SetCurrentGameState(EWarriorSurvivalGameState::WaitSpwanNewWave);
                PreLoadNextWaveEnemy();
            }
        }
    }

}
//设置游戏状态推进游戏进行逻辑
void AWarriorSurvivalGameMode::SetCurrentGameState(EWarriorSurvivalGameState InState)
{
    CurrentGameState = InState;
    OnSurvivalGameStateChanged.Broadcast(CurrentGameState);
}

bool AWarriorSurvivalGameMode::HasFinishedAllWaves() const
{
    return CurrentWaveCount > TotalWavesToSpawn;
}

//预加载下一波次的敌人
void AWarriorSurvivalGameMode::PreLoadNextWaveEnemy()
{
    //判断是否结束
    if(HasFinishedAllWaves())
        return;
    //清空上一次的预加载敌人类Map
    PreLoadedEnemyClassMap.Empty();
    //加载当前波次敌人类放入Map中以供获取
    for(const FWarriorEnemyWaveSpawnerInfo& EnemySpawner : FindCurrentWaveSpawnerRow()->EnemyWaveSpawnerInfos)
    {
        if(EnemySpawner.SoftEnemyClassToSpawn.IsNull())
            continue;
        //异步加载ing
        UAssetManager::GetStreamableManager().RequestAsyncLoad(
            EnemySpawner.SoftEnemyClassToSpawn.ToSoftObjectPath(),
            FStreamableDelegate::CreateLambda(
                [EnemySpawner,this]()
                {
                    if(UClass* LoadedEnemyClass = EnemySpawner.SoftEnemyClassToSpawn.Get())
                    {
                        PreLoadedEnemyClassMap.Emplace(EnemySpawner.SoftEnemyClassToSpawn,LoadedEnemyClass);
                    }
                }
            )
        );
    }
}

FWarriorEnemyWaveSpawnerTableRow* AWarriorSurvivalGameMode::FindCurrentWaveSpawnerRow() const
{
    const FName CurrentWaveRowName = FName(TEXT("Wave") + FString::FromInt(CurrentWaveCount)); 

    FWarriorEnemyWaveSpawnerTableRow* CurrentRow = EnemyWaveSpawnerTable->FindRow<FWarriorEnemyWaveSpawnerTableRow>(CurrentWaveRowName,FString());

    checkf(CurrentRow,TEXT("can not find the CurrentRow under the name %s"),*CurrentWaveRowName.ToString());

    return CurrentRow;
}

//生成波次敌人
int32 AWarriorSurvivalGameMode::TrySpawnWaveEnemies()
{
    //获取生成点
    if(TargetPointArray.IsEmpty())
        UGameplayStatics::GetAllActorsOfClass(this,ATargetPoint::StaticClass(),TargetPointArray);

    checkf(!TargetPointArray.IsEmpty(),TEXT("No Valid TargetPoint Founded In Level : %s"),*GetWorld()->GetName());

    //每个波次当前生成的敌人数量
    uint32 EnemySpawnedTishTime = 0;

    //生成参数供SpawnActor使用 这个参数是SPawnActor的生成策略，检测如果生成的地方有其他的碰撞会调整一下，无论怎样一定生成
    FActorSpawnParameters SpawnParam;
    SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    //轮询当前波次的每种敌人的生成信息
    for(const FWarriorEnemyWaveSpawnerInfo EnemyWaveSpawner : FindCurrentWaveSpawnerRow()->EnemyWaveSpawnerInfos)
    {
        if(EnemyWaveSpawner.SoftEnemyClassToSpawn.IsNull()) continue;
        //生成的数量在Min和Max之间随机
        const int32 NumToSpawn = FMath::RandRange(EnemyWaveSpawner.MinEnemyCount,EnemyWaveSpawner.MaxEnemyCount);
        //在PreLoadNextWaveEnemy中已经预加载了敌人类，这里直接从Map中获取
        UClass* LoadedEnemyClass = PreLoadedEnemyClassMap.FindChecked(EnemyWaveSpawner.SoftEnemyClassToSpawn);
        //逐个生成敌人
        for(int32 i=0; i < NumToSpawn; ++i)
        {
            //设置生成地址和生成方向
            const int32 RandomTargetPoint = FMath::RandRange(0,TargetPointArray.Num()-1);
            const FVector SpawnOrigin = TargetPointArray[RandomTargetPoint]->GetActorLocation();
            const FRotator SpawnRotation = TargetPointArray[RandomTargetPoint]->GetActorForwardVector().ToOrientationRotator();
            //在生成位置添加随机值
            FVector RandomLocation;
            UNavigationSystemV1::K2_GetRandomLocationInNavigableRadius(this,SpawnOrigin,RandomLocation,400.f);
            RandomLocation += FVector(0.f,0.f,150.f);
            //生成敌人
            ACharacter_Enemy* SpawnedEnemy = GetWorld()->SpawnActor<ACharacter_Enemy>(LoadedEnemyClass,RandomLocation,SpawnRotation,SpawnParam);

            //生成成功后计数，因为每种敌人的生成数量是随机的，所以要有一个成员变量来计数然后判定是否继续往下生成，也不能保证每种敌人都生成了以后会达到总数量要求
            if(SpawnedEnemy)
            {
                //添加敌人死亡时的委托
                SpawnedEnemy->OnDestroyed.AddUniqueDynamic(this,&AWarriorSurvivalGameMode::OnEnemyDied);

                ++EnemySpawnedTishTime;
                ++TotalSpawnedEnemiesThisWaveCounter;
            }

            if(!ShouldKeepSpawnEnemy())
            {
                return EnemySpawnedTishTime;
            }
        }

    }

    return EnemySpawnedTishTime;

}
//波次当前生成的敌人数量小于总数量需求的话就Keep，返回真
bool AWarriorSurvivalGameMode::ShouldKeepSpawnEnemy() const
{
    return TotalSpawnedEnemiesThisWaveCounter < FindCurrentWaveSpawnerRow()->TotalEnemyCount;
}
//敌人死亡的回调函数
void AWarriorSurvivalGameMode::OnEnemyDied(AActor* DeadActor)
{
    //当前在场的敌人数量减一
    CurrentSpawnedEnemiesCounter--;

    //打印一下当前计数器情况
    //Debug::Print(FString::Printf(TEXT("CurrentSpawnedEnemiesCounter: %i, TotalSpawnedEnemiesThisWaveCounter: %i"),CurrentSpawnedEnemiesCounter,TotalSpawnedEnemiesThisWaveCounter));

    //因为之前说过敌人在一次波次生成之后是有情况没有生成到设置的总数量的，所以要补生成
    if(ShouldKeepSpawnEnemy())
    {
        CurrentSpawnedEnemiesCounter += TrySpawnWaveEnemies();
    }
    //如果不需要生成敌人了并且当前在场敌人数量为0的话就清除计数器然后推进游戏状态到WaveComplete阶段
    else if(CurrentSpawnedEnemiesCounter == 0)
    {
        TotalSpawnedEnemiesThisWaveCounter = 0;
        CurrentSpawnedEnemiesCounter = 0;

        SetCurrentGameState(EWarriorSurvivalGameState::WaveComplete);
    }
    
}
