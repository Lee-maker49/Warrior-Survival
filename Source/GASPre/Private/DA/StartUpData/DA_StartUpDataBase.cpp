// Fill out your copyright notice in the Description page of Project Settings.


#include "DA/StartUpData/DA_StartUpDataBase.h"
#include "AbilitySystem/AbilitySystemComponent_Base.h"
#include "AbilitySystem/GameplayAbility/GameplayAbility_Base.h"
//赋予ASC,ActivateOnGivenGA和ReactiveGA中的GA,并且赋予StartUpGameplayEffects中的GE
void UDA_StartUpDataBase::GiveGAToASC(UAbilitySystemComponent_Base *TargetASC, int32 ApplyLevel)
{
    check(TargetASC);

    GrantAbilities(ActivateOnGivenGA,TargetASC,ApplyLevel);
    GrantAbilities(ReactiveGA,TargetASC,ApplyLevel);

    if(!StartUpGameplayEffects.IsEmpty())
    {
        for(const TSubclassOf<UGameplayEffect> &EffectClass : StartUpGameplayEffects)
        {
            if(!EffectClass) return;

            UGameplayEffect* EffectCDO = EffectClass->GetDefaultObject<UGameplayEffect>();

            TargetASC->ApplyGameplayEffectToSelf(
                EffectCDO,
                ApplyLevel,
                TargetASC->MakeEffectContext()
            );
        }
    }
}
void UDA_StartUpDataBase::GrantAbilities(const TArray<TSubclassOf<UGameplayAbility_Base>> &AbilitiesToGive, UAbilitySystemComponent_Base *TargetASC, int32 ApplyLevel)
{
    if(AbilitiesToGive.IsEmpty())
        return;
    
    for(const TSubclassOf<UGameplayAbility_Base> &Ability : AbilitiesToGive)
    {
        if(!Ability)
            return;

        FGameplayAbilitySpec AbilitySpec(Ability);
        AbilitySpec.SourceObject = TargetASC->GetAvatarActor();
        AbilitySpec.Level = ApplyLevel;

        TargetASC->GiveAbility(AbilitySpec);
    }
}