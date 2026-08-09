// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GameplayAbility/GameplayAbility_Base.h"
#include "AbilitySystem/AbilitySystemComponent_Base.h"
#include "Components/Combat/PawnCombatComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

void UGameplayAbility_Base::OnGiveAbility(const FGameplayAbilityActorInfo *ActorInfo, const FGameplayAbilitySpec &Spec)
{
    Super::OnGiveAbility(ActorInfo,Spec);

    if(ActivationPolicy == EAbilityActivationPolicy::OnGiven)
    {
        if(ActorInfo && !Spec.IsActive())
        {
            ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle);
        }
    }
    
}
void UGameplayAbility_Base::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    Super::EndAbility(Handle,ActorInfo,ActivationInfo,bReplicateEndAbility,bWasCancelled);

    if(ActivationPolicy == EAbilityActivationPolicy::OnGiven)
    {
        if(ActorInfo)
            ActorInfo->AbilitySystemComponent->ClearAbility(Handle);
    }
}
UPawnCombatComponent *UGameplayAbility_Base::GetPawnCombatComponentFromActorInfo() const
{
    return GetAvatarActorFromActorInfo()->FindComponentByClass<UPawnCombatComponent>();
}
UAbilitySystemComponent_Base *UGameplayAbility_Base::GetMyAbilitySystemComponentFromActorInfo() const
{
    return Cast<UAbilitySystemComponent_Base>(CurrentActorInfo->AbilitySystemComponent);
}
FActiveGameplayEffectHandle UGameplayAbility_Base::NativeApplyEffectSpecHandleToTarget(AActor *TargetActor, const FGameplayEffectSpecHandle &SpecHandleToUSe)
{
    UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

    check(TargetASC && SpecHandleToUSe.IsValid());

    return GetMyAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(
        *SpecHandleToUSe.Data,
        TargetASC
    );
}

FActiveGameplayEffectHandle UGameplayAbility_Base::BP_ApplyEffectSpecHandleToTarget(AActor *TargetActor, const FGameplayEffectSpecHandle &SpecHandleToUSe, ESuccessType &OutSuccessType)
{
    FActiveGameplayEffectHandle ActiveGameplayEffectHandle = NativeApplyEffectSpecHandleToTarget(TargetActor,SpecHandleToUSe);

    OutSuccessType = ActiveGameplayEffectHandle.WasSuccessfullyApplied()? ESuccessType::Successful : ESuccessType::Failed;
    
    return ActiveGameplayEffectHandle;
}