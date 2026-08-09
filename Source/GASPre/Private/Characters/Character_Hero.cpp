// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Character_Hero.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFrameWork/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFrameWork/CharacterMovementComponent.h"
#include "DA/Input/DA_InputConfig.h"
#include "Components/Input/EIC_Hero.h"
#include "Tags_My.h"
#include "DA/StartUpData/DA_StartUpDataBase.h"
#include "Components/Combat/HeroCombatComponent.h"
#include "AbilitySystem/AbilitySystemComponent_Base.h"
#include "Components/UI/HeroUIComponent.h"
#include "GameModes/GameMode_Base.h"

#include "DebugHelpper.h"

ACharacter_Hero::ACharacter_Hero()
{
    GetCapsuleComponent()->InitCapsuleSize(42.0f,96.0f);

    bUseControllerRotationPitch = false;
    bUseControllerRotationRoll = false;
    bUseControllerRotationYaw = false;

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(GetRootComponent());
    CameraBoom->TargetArmLength = 200.0f;
    CameraBoom->SocketOffset = FVector(0.0,55.0,65.0);
    CameraBoom->bUsePawnControlRotation = true;

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(CameraBoom,USpringArmComponent::SocketName);
    CameraBoom->bUsePawnControlRotation = false;

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0,500.0,0.0);
    GetCharacterMovement()->MaxWalkSpeed = 400.0f;
    GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;

    HeroCombatComponent = CreateDefaultSubobject<UHeroCombatComponent>(TEXT("HeroCombatComponent"));

    HeroUIComponent = CreateDefaultSubobject<UHeroUIComponent>(TEXT("HeroUIComponent"));
}
UPawnCombatComponent *ACharacter_Hero::GetPawnCombatComponent() const
{
    return HeroCombatComponent;
}
UPawnUIComponent *ACharacter_Hero::GetPawnUIComponent() const
{
    return HeroUIComponent;
}
UHeroUIComponent *ACharacter_Hero::GetHeroUIComponent() const
{
    return HeroUIComponent;
}

void ACharacter_Hero::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    ULocalPlayer* LocalPlayer = GetController<APlayerController>()->GetLocalPlayer();
    UEnhancedInputLocalPlayerSubsystem* System = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

    checkf(System,TEXT("no System"));

    System->AddMappingContext(InputConfig->DefaultMappingContext,0);

    UEIC_Hero* EIC_Hero = CastChecked<UEIC_Hero>(PlayerInputComponent);
    EIC_Hero->BindInputActions(InputConfig,MyGameplayTags::InputTag_Move,ETriggerEvent::Triggered,this,&ThisClass::Input_Move);
    EIC_Hero->BindInputActions(InputConfig,MyGameplayTags::InputTag_Look,ETriggerEvent::Triggered,this,&ThisClass::Input_Look);

    EIC_Hero->BindAbilityInputAction(InputConfig,this,&ThisClass::Input_AbilityPressed,&ThisClass::Input_AbilityReleased);   
}
void ACharacter_Hero::BeginPlay() 
{
    Super::BeginPlay();

    //Debug::Print("Working");
}

void ACharacter_Hero::PossessedBy(AController *NewController)
{
    Super::PossessedBy(NewController);
    
    //获取基础能力 包括多个GA和GE
    if(!DA_StartUpDataBase.IsNull())
    {
        //加载数据资产
        if(UDA_StartUpDataBase* Data = DA_StartUpDataBase.LoadSynchronous())
        {
            //根据游戏难度设置能力等级
            int32 AbilityLevel = 1;

            if(AGameMode_Base* GameMode = GetWorld()->GetAuthGameMode<AGameMode_Base>())
            {
                switch(GameMode->GetGameDifficulty())
                {
                case ESurvivalGameDifficuty::Easy:
                        AbilityLevel = 4;
                        //Debug::Print(TEXT("Current Game Difficuty is Easy"));
                        break;
                    case ESurvivalGameDifficuty::Normal:
                        AbilityLevel = 3;
                        //Debug::Print(TEXT("Current Game Difficuty is Normal"));
                        break;
                    case ESurvivalGameDifficuty::Hard:
                        AbilityLevel = 2;
                        //Debug::Print(TEXT("Current Game Difficuty is Hard"));
                        break;
                    case ESurvivalGameDifficuty::VeryHard:
                        AbilityLevel = 1;
                        //Debug::Print(TEXT("Current Game Difficuty is Very Hard"));
                        break;
                    default:
                        break;
                }
            }
            //给ASC赋予GA
            Data->GiveGAToASC(ASC,AbilityLevel);
        }
    }
}
//输入回调
void ACharacter_Hero::Input_Move(const FInputActionValue &Value)
{
    const FVector2D MovementVector = Value.Get<FVector2D>();
    const FRotator MovementRotation = FRotator(0.0f,Controller->GetControlRotation().Yaw,0.0f);

    if(MovementVector.Y != 0.0f)
    {
        const FVector ForwarDir = MovementRotation.RotateVector(FVector::ForwardVector);
        AddMovementInput(ForwarDir,MovementVector.Y);
    }
    if(MovementVector.X != 0.0f)
    {
        const FVector RightDir = MovementRotation.RotateVector(FVector::RightVector);
        AddMovementInput(RightDir,MovementVector.X);
    }
}
void ACharacter_Hero::Input_Look(const FInputActionValue& Value)
{
    const FVector2D LookData = Value.Get<FVector2D>();
    if(Controller)
    {
        AddControllerPitchInput(LookData.Y);
        AddControllerYawInput(LookData.X);
    }
}
//输入回调函数传递Tag给OnAbilityPressed函数
void ACharacter_Hero::Input_AbilityPressed(const FGameplayTag AbilityTag)
{
    ASC->OnAbilityPressed(AbilityTag);
}

void ACharacter_Hero::Input_AbilityReleased(const FGameplayTag AbilityTag)
{
    ASC->OnAbilityReleased(AbilityTag);
}
