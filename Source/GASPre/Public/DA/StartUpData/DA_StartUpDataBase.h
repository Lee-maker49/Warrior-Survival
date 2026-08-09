// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_StartUpDataBase.generated.h"

class UAbilitySystemComponent_Base;
class UGameplayAbility_Base;
class UGameplayEffect;
/**
 * 这个类是用来给Character的ASC赋予GA的
 */
UCLASS()
class GASPRE_API UDA_StartUpDataBase : public UDataAsset
{
	GENERATED_BODY()

public:
	//赋予ActivateOnGivenGA和ReactiveGA中的GA
	virtual void GiveGAToASC(UAbilitySystemComponent_Base* TargetASC,int32 ApplyLevel = 1);

protected:
	//被赋予则激活的GA
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "StartUpData")
	TArray< TSubclassOf < UGameplayAbility_Base > > ActivateOnGivenGA;
	//被动GA？
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "StartUpData")
	TArray< TSubclassOf < UGameplayAbility_Base > > ReactiveGA;

	//StartUp Gemaplay Effects
	UPROPERTY(EditDefaultsOnly,Category = "StartUpData")
	TArray<TSubclassOf<UGameplayEffect>> StartUpGameplayEffects;

	//赋予TSubclassOf < UGameplayAbility_Base >数组中的GA给ASC，作为GiveGAToASC的辅助函数
	void GrantAbilities(const TArray< TSubclassOf < UGameplayAbility_Base > >& AbilitiesToGive,UAbilitySystemComponent_Base* TargetASC,int32 ApplyLevel);

};
