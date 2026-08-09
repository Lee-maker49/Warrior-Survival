// Fill out your copyright notice in the Description page of Project Settings.


#include "DA/StartUpData/DA_StartUpDataHero.h"
#include "AbilitySystem/GameplayAbility/GameplayAbility_Base.h"
#include "AbilitySystem/AbilitySystemComponent_Base.h"

void UDA_StartUpDataHero::GiveGAToASC(UAbilitySystemComponent_Base *TargetASC, int32 ApplyLevel)
{
    Super::GiveGAToASC(TargetASC,ApplyLevel);

    for(const FHeroAbilitySet &AbilitySet : AbilitySets)
    {
        if(!AbilitySet.IsValid()) continue;

        FGameplayAbilitySpec AbilitySpec(AbilitySet.GAToGrant);
        AbilitySpec.SourceObject = TargetASC->GetAvatarActor();
        AbilitySpec.Level = ApplyLevel;
        AbilitySpec.DynamicAbilityTags.AddTag(AbilitySet.InputTag);  

        TargetASC->GiveAbility(AbilitySpec);
    }
}