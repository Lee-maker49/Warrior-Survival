// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilitySystemComponent_Base.h"
#include "AbilitySystem/GameplayAbility/GameplayAbility_Hero.h"
#include "Tags_My.h"

//通过Tag在所有可激活的GA中查找并激活对应GA
void UAbilitySystemComponent_Base::OnAbilityPressed(const FGameplayTag &GATag)
{
    if(!GATag.IsValid()) return;

    for(const FGameplayAbilitySpec &AbilitySpec : GetActivatableAbilities())
    {
        if(!AbilitySpec.DynamicAbilityTags.HasTagExact(GATag)) continue;

        //Handle the Input of Toggleable Ability
        if(GATag.MatchesTag(MyGameplayTags::InputTag_Toggleable))
        {
            if(AbilitySpec.IsActive())
            {
                CancelAbilityHandle(AbilitySpec.Handle);
            }
            else
            {
                TryActivateAbility(AbilitySpec.Handle);
            }
        }
        else
        {
            //Handle the Input of Normal Ability
            TryActivateAbility(AbilitySpec.Handle);
        }
    }
}
//专门处理需要长按激活的GA，按下时激活，松开时结束
void UAbilitySystemComponent_Base::OnAbilityReleased(const FGameplayTag &GATag)
{
    if(!GATag.IsValid() || !GATag.MatchesTag(MyGameplayTags::InputTag_MustBeHeld))
    {
        return;
    } 

    for(const FGameplayAbilitySpec &AbilitySpec : GetActivatableAbilities())
    {
        if(AbilitySpec.DynamicAbilityTags.HasTagExact(GATag) && AbilitySpec.IsActive())
        {
            CancelAbilityHandle(AbilitySpec.Handle);
        }
    }

}

void UAbilitySystemComponent_Base::GrantHeroWeaponAbility(const TArray<FHeroAbilitySet> &WeaponAbility, int32 ApplyLevel,TArray<FGameplayAbilitySpecHandle> &GrantGASpecHandle)
{
    if(WeaponAbility.IsEmpty()) return;

    for(const FHeroAbilitySet AbilitySet : WeaponAbility)
    {
        if(!AbilitySet.IsValid()) continue;

        FGameplayAbilitySpec AbilitySpec(AbilitySet.GAToGrant);
        AbilitySpec.SourceObject = GetAvatarActor();
        AbilitySpec.Level = ApplyLevel;
        AbilitySpec.DynamicAbilityTags.AddTag(AbilitySet.InputTag);

        GrantGASpecHandle.AddUnique(GiveAbility(AbilitySpec));
    }
}

void UAbilitySystemComponent_Base::RemoveGrantedHeroWeaponAbility(UPARAM(ref) TArray<FGameplayAbilitySpecHandle> &InGASpecHandlesToRemove)
{
    if(InGASpecHandlesToRemove.IsEmpty()) return;

    for(const FGameplayAbilitySpecHandle SpecHandle : InGASpecHandlesToRemove)
    {
        if(SpecHandle.IsValid())
        {
            ClearAbility(SpecHandle);
        }
    }

    InGASpecHandlesToRemove.Empty();
}

bool UAbilitySystemComponent_Base::TryActivateAbilityByTag(FGameplayTag AbilityTagToActivate)
{
    check(AbilityTagToActivate.IsValid());

    TArray<FGameplayAbilitySpec*> FindAbilitySpecs;
    GetActivatableGameplayAbilitySpecsByAllMatchingTags(AbilityTagToActivate.GetSingleTagContainer(), FindAbilitySpecs);

    if(!FindAbilitySpecs.IsEmpty())
    {
        const int32 RandomIndex = FMath::RandRange(0, FindAbilitySpecs.Num() - 1);
        FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecs[RandomIndex];
        
        check(AbilitySpec);
        
        if(!AbilitySpec->IsActive())
        {
            return TryActivateAbility(AbilitySpec->Handle);
        }
    }

    return false;
}
