// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "Types/StructTypes.h"

#include "AbilitySystemComponent_Base.generated.h"

/**
 * 这个类为通过输入激活的GA提供实现方法
 * 这个类提供所有赋予，激活和移除GA的方法
 */
UCLASS()
class GASPRE_API UAbilitySystemComponent_Base : public UAbilitySystemComponent
{
	GENERATED_BODY()
	 
public:
	void OnAbilityPressed(const FGameplayTag &GATag);
	void OnAbilityReleased(const FGameplayTag &GATag);

	//专门赋予武器携带的GA
	UFUNCTION(BlueprintCallable,Category = "Hero|Ability",meta = (ApplyLevel = "1"))
	void GrantHeroWeaponAbility(const TArray<FHeroAbilitySet> &WeaponAbility,int32 ApplyLevel,TArray<FGameplayAbilitySpecHandle> &GrantGASpecHandle);
	//专门移除武器携带的GA
	UFUNCTION(BlueprintCallable,Category = "Hero|Ability")
	void RemoveGrantedHeroWeaponAbility(UPARAM(ref) TArray<FGameplayAbilitySpecHandle> &InGASpecHandlesToRemove);

	UFUNCTION(BlueprintCallable,Category = "Hero|Ability")
	bool TryActivateAbilityByTag(FGameplayTag AbilityTagToActivate);
};
