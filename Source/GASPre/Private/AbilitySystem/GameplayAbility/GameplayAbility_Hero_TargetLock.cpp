// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GameplayAbility/GameplayAbility_Hero_TargetLock.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Characters/Character_Hero.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/Widget_Base.h"
#include "Controllers/PlayerController_Base.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Components/SizeBox.h"

#include "DebugHelpper.h"

void UGameplayAbility_Hero_TargetLock::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData *TriggerEventData)
{
    TryLockOnTarget();

    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGameplayAbility_Hero_TargetLock::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    CleanUp();
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGameplayAbility_Hero_TargetLock::TryLockOnTarget()
{
    GetAvailableActorsToLock();

    if(AvailableActorsToLock.IsEmpty())
    {
        CancelTargetLockAbility();
        return;
    }
    
    CurrentLockedActor = GetNearestTargetFromAvailableActors(AvailableActorsToLock);

    if(CurrentLockedActor)
    {
        DrawTargetLockWidget();
        SetTargetLockWidgetPosition();
    }
    else
    {
        CancelTargetLockAbility();
    }
}

void UGameplayAbility_Hero_TargetLock::GetAvailableActorsToLock()
{
    TArray<FHitResult> BoxTraceResults;

    UKismetSystemLibrary::BoxTraceMultiForObjects(
        GetHeroCharacterFromActorInfo(),
        GetHeroCharacterFromActorInfo()->GetActorLocation(),
        GetHeroCharacterFromActorInfo()->GetActorLocation() + GetHeroCharacterFromActorInfo()->GetActorForwardVector() * BoxTraceDistance,
        TraceBoxSize / 2.0f,
        GetHeroCharacterFromActorInfo()->GetActorForwardVector().ToOrientationRotator(),
        BoxTraceChannel,
        false,
        TArray<AActor*>(),
        bShowPersistentDebugShape ? EDrawDebugTrace::Persistent : EDrawDebugTrace::None,
        BoxTraceResults,
        true
    );

    for(const FHitResult& TraceResult : BoxTraceResults)
    {
        if(AActor* HitActor = TraceResult.GetActor())
        {
            if(HitActor != GetHeroCharacterFromActorInfo())
            {
                AvailableActorsToLock.AddUnique(HitActor);

                //Debug::Print(FString::Printf(TEXT("Found Actor: %s"), *HitActor->GetActorNameOrLabel()));
            }
        }
    }

}

//CancelAbility will exec the EndAbility function.
void UGameplayAbility_Hero_TargetLock::CancelTargetLockAbility()
{
    CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
}

//Clean Up When the Ability Ends
void UGameplayAbility_Hero_TargetLock::CleanUp()
{
    AvailableActorsToLock.Empty();

    CurrentLockedActor = nullptr;

    if(DrawnTargetLockWidget)
    {
        DrawnTargetLockWidget->RemoveFromParent();
    }

}

AActor* UGameplayAbility_Hero_TargetLock::GetNearestTargetFromAvailableActors(const TArray<AActor*>& InAvailableActors)
{
    float Distance = 0.f;
    return UGameplayStatics::FindNearestActor(GetHeroCharacterFromActorInfo()->GetActorLocation(), InAvailableActors, Distance);
}

void UGameplayAbility_Hero_TargetLock::DrawTargetLockWidget()
{
    if(!DrawnTargetLockWidget)
    {
        checkf(TargetLockWidgetClass, TEXT("Forget to Set the TargetLockWidgetClass"));

        DrawnTargetLockWidget = CreateWidget<UWidget_Base>(GetControllerFromActorInfo(), TargetLockWidgetClass);
        check(DrawnTargetLockWidget);
        DrawnTargetLockWidget->AddToViewport();
    }
}

void UGameplayAbility_Hero_TargetLock::SetTargetLockWidgetPosition()
{
    if(!DrawnTargetLockWidget || !CurrentLockedActor)
    {
        CancelTargetLockAbility();
        return;
    }

    FVector2D ScreenPosition;
    UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(
        GetControllerFromActorInfo(),
        CurrentLockedActor->GetActorLocation(),
        ScreenPosition,
        true
    );

    if(TargetLockWidgetSize == FVector2D::ZeroVector)
    {
        DrawnTargetLockWidget->WidgetTree->ForEachWidget(
            [this](UWidget* FoundWidget)
            {
                if(USizeBox* FoundSizeBox = Cast<USizeBox>(FoundWidget))
                {
                    TargetLockWidgetSize.X = FoundSizeBox->GetWidthOverride();
                    TargetLockWidgetSize.Y = FoundSizeBox->GetHeightOverride();
                }
            }
        );
    }

    ScreenPosition -= (TargetLockWidgetSize / 2.f);

    DrawnTargetLockWidget->SetPositionInViewport(ScreenPosition,false);
}
