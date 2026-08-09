// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Character_Enemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/Combat/EnemyCombatComponent.h"
#include "Engine/AssetManager.h"
#include "DA/StartUpData/DA_StartUpDataBase.h"
#include "Components/UI/EnemyUIComponent.h"
#include "Components/WidgetComponent.h"
#include "Widgets/Widget_Base.h"
#include "GameModes/GameMode_Base.h"

#include "DebugHelpper.h"

ACharacter_Enemy::ACharacter_Enemy()
{
    //native config
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

    bUseControllerRotationPitch = false;
    bUseControllerRotationRoll = false;
    bUseControllerRotationYaw = false;

    GetCharacterMovement()->bUseControllerDesiredRotation = false;
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.f,180.f,0.f);
    GetCharacterMovement()->MaxWalkSpeed = 300.f;
    GetCharacterMovement()->BrakingDecelerationWalking = 100.f;

    //构造组件
    EnemyCombatComponent = CreateDefaultSubobject<UEnemyCombatComponent>(TEXT("EnemyCombatComponent"));

    EnemyUIComponent = CreateDefaultSubobject<UEnemyUIComponent>(TEXT("EnemyUIComponent"));

    EnemyHealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("EnemyHealthWidgetComponent"));
    EnemyHealthWidgetComponent->SetupAttachment(GetMesh());

}
UPawnCombatComponent *ACharacter_Enemy::GetPawnCombatComponent() const
{
    return EnemyCombatComponent;
}

UPawnUIComponent *ACharacter_Enemy::GetPawnUIComponent() const
{
    return EnemyUIComponent;
}

UEnemyUIComponent *ACharacter_Enemy::GetEnemyUIComponent() const
{
    return EnemyUIComponent;
}

void ACharacter_Enemy::BeginPlay()
{
    Super::BeginPlay();

    if(UWidget_Base* EnemyHealthWidget = Cast<UWidget_Base>(EnemyHealthWidgetComponent->GetUserWidgetObject()))
    {
        EnemyHealthWidget->InitEnemyCreateWidget(this);
    }
}

void ACharacter_Enemy::PossessedBy(AController *NewController)
{
    Super::PossessedBy(NewController);

    InitEnemyStartUpData();
}

void ACharacter_Enemy::InitEnemyStartUpData()
{
    if(DA_StartUpDataBase.IsNull()) return;
    //根据游戏难度设置能力等级
    int32 AbilityLevel = 1;

    if(AGameMode_Base* GameMode = GetWorld()->GetAuthGameMode<AGameMode_Base>())
    {
        switch(GameMode->GetGameDifficulty())
        {
            case ESurvivalGameDifficuty::Easy:
                AbilityLevel = 1;
                break;
            case ESurvivalGameDifficuty::Normal:
                AbilityLevel = 2;
                break;
            case ESurvivalGameDifficuty::Hard:
                AbilityLevel = 3;
                break;
            case ESurvivalGameDifficuty::VeryHard:
                AbilityLevel = 4;
                break;
            default:
                break;
        }
    }
    //异步加载初始能力数据
    UAssetManager::GetStreamableManager().RequestAsyncLoad(
        DA_StartUpDataBase.ToSoftObjectPath(),
        FStreamableDelegate::CreateLambda(
            [this,AbilityLevel]()
            {
                if(UDA_StartUpDataBase* LoadedData = DA_StartUpDataBase.Get())
                {
                    LoadedData->GiveGAToASC(ASC,AbilityLevel);
                }
            }
        )
    );
        
}