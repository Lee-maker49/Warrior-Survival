// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Combat/EnemyCombatComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Tags_My.h"
#include "FunctionLibrary.h"

#include "DebugHelpper.h"

//maintain the HitActors array and Send the GameplayEvent to enemyself to handle the damage logic
void UEnemyCombatComponent::OnHitTargetActor(AActor *TargetActor)
{
    if(HitActors.Contains(TargetActor))
    {
        return;
    }
    HitActors.AddUnique(TargetActor);

    bool bIsValidBlock = false;

    const bool bIsPlayerBlocking = UFunctionLibrary::NativeDoesActorHaveTag(TargetActor,MyGameplayTags::Player_Status_Blocking);
    const bool bIsMyAttackUnblockable = UFunctionLibrary::NativeDoesActorHaveTag(GetOwningPawn(),MyGameplayTags::Enemy_Status_UnBlockAble);

    //如果攻击不可阻挡或者玩家没有格挡，则格挡直接保持默认值无效
    //如果玩家在格挡状态，敌人的攻击不是不可阻挡的那么检查格挡是否有效
    if(bIsPlayerBlocking && !bIsMyAttackUnblockable)
    {
        bIsValidBlock = UFunctionLibrary::IsBlockValid(GetOwningPawn(),TargetActor);
    }

    //构造EventData
    FGameplayEventData Data;
    Data.Instigator = GetOwningPawn();
    Data.Target = TargetActor;

    if(bIsValidBlock)
    {
        //Handle the Successful Block
        UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
            TargetActor,
            MyGameplayTags::Player_Event_SuccessfulBlock,
            Data
        );
    }
    else
    {
        //Handle the Failed Block
        UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
            GetOwningPawn(),
            MyGameplayTags::Shared_Event_MeleeHit,
            Data
        );
    }

    
}