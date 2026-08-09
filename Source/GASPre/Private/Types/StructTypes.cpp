// Fill out your copyright notice in the Description page of Project Settings.


#include "Types/StructTypes.h"
#include "AbilitySystem/GameplayAbility/GameplayAbility_Base.h"

bool FHeroAbilitySet::IsValid() const
{
    return InputTag.IsValid() && GAToGrant;
}