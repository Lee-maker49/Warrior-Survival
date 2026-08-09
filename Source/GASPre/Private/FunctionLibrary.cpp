// Fill out your copyright notice in the Description page of Project Settings.


#include "FunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AbilitySystemComponent_Base.h"
#include "Interface/Interface_PawnCombatComponent.h"
#include "GenericTeamAgentInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "LatentActions.h"
#include "WarriorGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "WarriorSaveGame.h"
#include "Tags_My.h"
#include "Types/WarriorCountDownAction.h"

#include "DebugHelpper.h"

class FCountDownLatentAction : public FPendingLatentAction
{
	float& OutRemainingTime;
	float TotalTime;
	const float StartTime;
	ECountDownCompleteType& OutCompleteType;
	FName ExecutionFunction;
	int32 OutputLink;
	FWeakObjectPtr CallbackTarget;

public:
	FCountDownLatentAction(const FLatentActionInfo& InLatentInfo, float InTime, float& InOutRemainingTime, ECountDownCompleteType& InOutCompleteType)
		: OutRemainingTime(InOutRemainingTime)
		, TotalTime(InTime)
		, StartTime(FPlatformTime::Seconds())
		, OutCompleteType(InOutCompleteType)
		, ExecutionFunction(InLatentInfo.ExecutionFunction)
		, OutputLink(InLatentInfo.Linkage)
		, CallbackTarget(InLatentInfo.CallbackTarget)
	{
	}

	virtual void UpdateOperation(FLatentResponse& Response) override
	{
		const float ElapsedTime = static_cast<float>(FPlatformTime::Seconds() - StartTime);
		OutRemainingTime = FMath::Max(0.f, TotalTime - ElapsedTime);
		OutCompleteType = ECountDownCompleteType::OnGoing;

		if (OutRemainingTime <= 0.f)
		{
			OutCompleteType = ECountDownCompleteType::Completed;
			Response.FinishAndTriggerIf(true, ExecutionFunction, OutputLink, CallbackTarget);
		}
		else
		{
			Response.TriggerLink(ExecutionFunction, OutputLink, CallbackTarget);
		}
	}

#if WITH_EDITOR
	virtual FString GetDescription() const override
	{
		return FString::Printf(TEXT("CountDown Timer: %.2f remaining"), OutRemainingTime);
	}
#endif
};

UAbilitySystemComponent_Base *UFunctionLibrary::NativeGetASCFromActor(AActor *InActor)
{
    check(InActor);

    return CastChecked<UAbilitySystemComponent_Base>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor));
}

void UFunctionLibrary::AddGameplayTagIfNone(AActor *InActor, FGameplayTag TagToAdd)
{
    UAbilitySystemComponent_Base* ASC = NativeGetASCFromActor(InActor);
    if(!ASC->HasMatchingGameplayTag(TagToAdd))
    {
        ASC->AddLooseGameplayTag(TagToAdd);
    }
}

void UFunctionLibrary::RemoveGameplayTagIfFind(AActor *InActor, FGameplayTag TagToRemove)
{
    UAbilitySystemComponent_Base* ASC = NativeGetASCFromActor(InActor);
    if(ASC->HasMatchingGameplayTag(TagToRemove))
    {
        ASC->RemoveLooseGameplayTag(TagToRemove);
    }
}

bool UFunctionLibrary::NativeDoesActorHaveTag(AActor *InActor, FGameplayTag TagToCheck)
{
    UAbilitySystemComponent_Base* ASC = NativeGetASCFromActor(InActor);

    return ASC->HasMatchingGameplayTag(TagToCheck);
}

void UFunctionLibrary::BP_DoesActorHaveTag(AActor *InActor, FGameplayTag TagToCheck, EConfirmType &OutConfirmType)
{
    OutConfirmType = NativeDoesActorHaveTag(InActor,TagToCheck)? EConfirmType::Yes : EConfirmType::No;
}

UPawnCombatComponent *UFunctionLibrary::NativeGetPawnCombatComponentFromActor(AActor *InActor)
{
    check(InActor);

    if(IInterface_PawnCombatComponent* Interface_PawnCombatComponent = Cast<IInterface_PawnCombatComponent>(InActor))
    {
        return Interface_PawnCombatComponent->GetPawnCombatComponent();
    }

    return nullptr;
}

UPawnCombatComponent *UFunctionLibrary::BP_GetPawnCombatComponentFromActor(AActor *InActor,EValidType &OutValidType)
{
    UPawnCombatComponent* CombatComponent = NativeGetPawnCombatComponentFromActor(InActor);
    OutValidType = CombatComponent? EValidType::Valid : EValidType::InValid;
    return CombatComponent;
}

bool UFunctionLibrary::IsTargetPawnHostile(APawn *QuaryPawn, APawn *TargetPawn)
{
    check(QuaryPawn && TargetPawn);

    IGenericTeamAgentInterface* QuaryTeamAgentInterface = Cast<IGenericTeamAgentInterface>(QuaryPawn->GetController());
    IGenericTeamAgentInterface* TargetTeamAgentInterface = Cast<IGenericTeamAgentInterface>(TargetPawn->GetController());

    if(QuaryTeamAgentInterface && TargetTeamAgentInterface)
    {
        return QuaryTeamAgentInterface->GetGenericTeamId() != TargetTeamAgentInterface->GetGenericTeamId();
    }

    return false;
}

EHitDirection UFunctionLibrary::NativeGetHitDirection(AActor *Attacker, AActor *Victim)
{
    const FVector AttackerForward = Attacker->GetActorForwardVector();
    const FVector VictimForward = Victim->GetActorForwardVector();

    const float DotProduct = FVector::DotProduct(AttackerForward,VictimForward);
    const float AngleDiff = UKismetMathLibrary::DegAcos(DotProduct);

    if(AngleDiff <= 45.f)
    {
        return EHitDirection::Back;
    }
    else if(AngleDiff > 135.f)
    {
        return EHitDirection::Front;
    }
    else
    {
        const FVector AttackerRight = Attacker->GetActorRightVector();
        const float RightDotProduct = FVector::DotProduct(AttackerRight,VictimForward);
        if(RightDotProduct > 0.f)
        {
            return EHitDirection::Right;
        }
        else
        {
            return EHitDirection::Left;
        }
    }

}

EHitDirection UFunctionLibrary::BP_GetHitDirection(AActor *Attacker, AActor *Victim)
{
    return NativeGetHitDirection(Attacker, Victim);
}

bool UFunctionLibrary::IsBlockValid(AActor *Attacker, AActor *Victim)
{
    check(Attacker && Victim);

    const float DotProduct = FVector::DotProduct(Attacker->GetActorForwardVector(),Victim->GetActorForwardVector());

    // const FString DebugString = FString::Printf(TEXT("DotResult is %f %s"),DotProduct,DotProduct < -0.5f ? TEXT("Block is Valid") : TEXT("Block is Invalid"));
    // Debug::Print(DebugString,DotProduct < -0.5f ? FColor::Green : FColor::Red);

    return DotProduct < -0.5f ? true : false;
}

void UFunctionLibrary::CountDownTimer(const UObject* WorldContextObject, float Time, FLatentActionInfo LatentInfo, ECountDownCompleteType& OutCompleteType, float& OutRemainingTime)
{
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		FLatentActionManager& LatentActionManager = World->GetLatentActionManager();
		if (LatentActionManager.FindExistingAction<FCountDownLatentAction>(LatentInfo.CallbackTarget, LatentInfo.UUID) == nullptr)
		{
			LatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID, new FCountDownLatentAction(LatentInfo, Time, OutRemainingTime, OutCompleteType));
		}
	}
}
//获取游戏实例
UWarriorGameInstance *UFunctionLibrary::GetWarriorGameInstance(const UObject *WorldContextObject)
{
    //GEngine 引擎对象指针
    if(GEngine)
    {
        if(UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
        {
            return World->GetGameInstance<UWarriorGameInstance>();
        }
    }

    return nullptr;
}
//切换输入模式
void UFunctionLibrary::ToggleInputMode(const UObject* WorldContextObject,EGameModeInput InInputMode)
{
    APlayerController* PlayerController = nullptr;
    //获取控制器
    if(GEngine)
    {
        if(UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
        {
            PlayerController = World->GetFirstPlayerController();
        }
    }
    if(!PlayerController)
        return;
    //切换输入模式
    FInputModeGameOnly InputModeGameOnly;
    FInputModeUIOnly InputModeUIOnly;
    switch(InInputMode)
    {
        case EGameModeInput::UIOnly:
            PlayerController->SetInputMode(InputModeUIOnly);
            PlayerController->bShowMouseCursor = true;
            break;
        case EGameModeInput::GameOnly:
            PlayerController->SetInputMode(InputModeGameOnly);
            PlayerController->bShowMouseCursor = false;
            break;
        default:
            break;
    }    

}

void UFunctionLibrary::SaveCurrentGameDifficuty(ESurvivalGameDifficuty InGameDifficuty)
{
    //创建SaveMode对象指针
    USaveGame* SaveGame = UGameplayStatics::CreateSaveGameObject(UWarriorSaveGame::StaticClass());
    //转化成WarriorSaveGame对象然后保存难度
    if(UWarriorSaveGame* WarriorSaveGame = Cast<UWarriorSaveGame>(SaveGame) )
    {
        WarriorSaveGame->SavedCurrentGameDifficuty = InGameDifficuty;
        //设置好SaveGame对象后保存到指定的槽位
        const bool bWasSaved = UGameplayStatics::SaveGameToSlot(WarriorSaveGame, MyGameplayTags::GameData_SaveGame_Slot_1.GetTag().ToString(), 0);
    
        //Debug::Print(bWasSaved ? TEXT("Game Difficuty Saved Successfully") : TEXT("Failed to Save Game Difficuty"), bWasSaved ? FColor::Green : FColor::Red);
    }

}

bool UFunctionLibrary::LoadSavedGameDifficuty(ESurvivalGameDifficuty &OutGameDifficuty)
{
    //检查保存游戏对象是否存在
    if(UGameplayStatics::DoesSaveGameExist(MyGameplayTags::GameData_SaveGame_Slot_1.GetTag().ToString(), 0))
    {
        //加载保存的游戏对象
        USaveGame* SaveGame = UGameplayStatics::LoadGameFromSlot(MyGameplayTags::GameData_SaveGame_Slot_1.GetTag().ToString(), 0);
        if(UWarriorSaveGame* WarriorSaveGame = Cast<UWarriorSaveGame>(SaveGame))
        {
            //获取游戏难度
            OutGameDifficuty = WarriorSaveGame->SavedCurrentGameDifficuty;
            //Debug::Print(TEXT("Loaded Saved Game Difficuty Successfully"), FColor::Green);
            return true; 
        }
    }

    return false;
}

void UFunctionLibrary::CountDown(const UObject *WorldContextObject, float TotalTime, float UpdateInterval, float &OutRemainingTime, EWarriorCountDownActionInput CountDownInput,UPARAM(DisplayName = "Output") EWarriorCountDownActionOutput &CountDownOutput, FLatentActionInfo LatentInfo)
{
    //获取World
    UWorld* World = nullptr;
    if(GEngine)
        World = GEngine->GetWorldFromContextObject(WorldContextObject,EGetWorldErrorMode::LogAndReturnNull);
    if(!World)
        return;
    
    FLatentActionManager& LatentActionManager = World->GetLatentActionManager();
    FWarriorCountDownAction* FindAction = LatentActionManager.FindExistingAction<FWarriorCountDownAction>(LatentInfo.CallbackTarget,LatentInfo.UUID);

    if(CountDownInput == EWarriorCountDownActionInput::Start)
    {
        if(!FindAction)
        {
            LatentActionManager.AddNewAction(LatentInfo.CallbackTarget,LatentInfo.UUID,
                new FWarriorCountDownAction(TotalTime,UpdateInterval,OutRemainingTime,CountDownOutput,LatentInfo)
            );
        }
    }
    if(CountDownInput == EWarriorCountDownActionInput::Cancel)
    {
        if(FindAction)
        {
            FindAction->CancelAction();
        }
    }

}

bool UFunctionLibrary::ApplyGameplayEffectSpacHandleToTarget(AActor *SourceActor, AActor *TargetActor, const FGameplayEffectSpecHandle &InSpecHandle)
{
    //获取两个ASC
    UAbilitySystemComponent_Base* SourceASC = NativeGetASCFromActor(SourceActor);
    UAbilitySystemComponent_Base* TargetASC = NativeGetASCFromActor(TargetActor);
    //通过ASC应用效果
    FActiveGameplayEffectHandle ActiveGameplayEffectHandle = SourceASC->ApplyGameplayEffectSpecToTarget(*InSpecHandle.Data,TargetASC);
    //返回是否应用成功
    return ActiveGameplayEffectHandle.WasSuccessfullyApplied();
}
