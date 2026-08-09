// Fill out your copyright notice in the Description page of Project Settings.


#include "DA/StartUpData/DA_StartUpDataEnemy.h"
#include "AbilitySystem/AbilitySystemComponent_Base.h"
#include "AbilitySystem/GameplayAbility/GameplayAbility_Enemy.h"

void UDA_StartUpDataEnemy::GiveGAToASC(UAbilitySystemComponent_Base *TargetASC, int32 ApplyLevel)
{
    Super::GiveGAToASC(TargetASC,ApplyLevel);

    if(!EnemyAbilities.IsEmpty())
    {
        for(const TSubclassOf<UGameplayAbility_Enemy> &AbilityClass : EnemyAbilities)
        {
            if(!AbilityClass) continue;

            FGameplayAbilitySpec AbilitySpec(AbilityClass);
            AbilitySpec.SourceObject = TargetASC->GetAvatarActor();
            AbilitySpec.Level = ApplyLevel;

            TargetASC->GiveAbility(AbilitySpec);
        }
    }
}