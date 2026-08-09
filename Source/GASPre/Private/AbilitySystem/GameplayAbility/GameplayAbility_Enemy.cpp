// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GameplayAbility/GameplayAbility_Enemy.h"
#include "Characters/Character_Enemy.h"
#include "AbilitySystem/AbilitySystemComponent_Base.h"
#include "Tags_My.h"

ACharacter_Enemy *UGameplayAbility_Enemy::GetEnemyCharacterFromActorInfo()
{
    if(!CachedEnemyCharacter.IsValid())
        CachedEnemyCharacter = Cast<ACharacter_Enemy>(CurrentActorInfo->AvatarActor);

    return CachedEnemyCharacter.IsValid()? CachedEnemyCharacter.Get() : nullptr;
}
UEnemyCombatComponent *UGameplayAbility_Enemy::GetEnemyCombatComponentFromActorInfo()
{ 
    return GetEnemyCharacterFromActorInfo()->GetEnemyCombatComponent();
}


FGameplayEffectSpecHandle UGameplayAbility_Enemy::MakeEnemyDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, const FScalableFloat &InDamageScalableFloat)
{
     check(EffectClass);

    FGameplayEffectContextHandle ContextHandle = GetMyAbilitySystemComponentFromActorInfo()->MakeEffectContext();
    ContextHandle.SetAbility(this);
    ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
    ContextHandle.AddInstigator(GetAvatarActorFromActorInfo(),GetAvatarActorFromActorInfo());

    FGameplayEffectSpecHandle SpecHandle = GetMyAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
        EffectClass,
        GetAbilityLevel(),
        ContextHandle
    );

    SpecHandle.Data->SetSetByCallerMagnitude(
        MyGameplayTags::Shared_SetByCaller_BaseDamage,
        InDamageScalableFloat.GetValueAtLevel(GetAbilityLevel())
    );
    return SpecHandle;
}