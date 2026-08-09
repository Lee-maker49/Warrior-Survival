// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GameplayAbility/GameplayAbility_Hero.h"
#include "Characters/Character_Hero.h"
#include "Controllers/PlayerController_Base.h"
#include "AbilitySystem/AbilitySystemComponent_Base.h"
#include "Tags_My.h"

ACharacter_Hero *UGameplayAbility_Hero::GetHeroCharacterFromActorInfo()
{
    if(!HeroCharacter.IsValid())
        HeroCharacter = Cast<ACharacter_Hero>(CurrentActorInfo->AvatarActor);
    return HeroCharacter.IsValid()? HeroCharacter.Get() : nullptr;
}
APlayerController_Base *UGameplayAbility_Hero::GetControllerFromActorInfo()
{
    if(!Controller.IsValid())
        Controller = Cast<APlayerController_Base>(CurrentActorInfo->PlayerController);
    return Controller.IsValid()? Controller.Get() : nullptr;
}
UHeroCombatComponent *UGameplayAbility_Hero::GetHeroCombatComponentFromActorInfo()
{
    return GetHeroCharacterFromActorInfo()->GetHeroCombatComponent();
}
//make the effectspechandle of damage effect and provide the info of base damage ,base damage tag , attack tag and combo count in the spechandle
FGameplayEffectSpecHandle UGameplayAbility_Hero::MakeHeroDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, float InWeaponBaseDamage, FGameplayTag InCurrentAttackTypeTag, int32 InCurrentComboCount)
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
        InWeaponBaseDamage
    );

    if(InCurrentAttackTypeTag.IsValid())
    {
        SpecHandle.Data->SetSetByCallerMagnitude(InCurrentAttackTypeTag,InCurrentComboCount);
    }

    return SpecHandle;
}