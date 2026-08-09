// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayTagContainer.h"
#include "ScalableFloat.h"

#include "StructTypes.generated.h"

class UAnimInstance_HeroLinkedLayer;
class UGameplayAbility_Base;
class UInputMappingContext;

/**
 * 这个结构体用来提供输入Tag和GA
 * 什么叫将输入Tag和GA绑定在一起：GiveAbility的时候会构造AbilitySpec
 * 将Tag写入Spec中，就可以在以后通过检测Tag的方式来判断要激活的是什么Ability
 */
USTRUCT(BlueprintType)
struct FHeroAbilitySet
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta = (Categories = "InputTag"))
	FGameplayTag InputTag;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSubclassOf<UGameplayAbility_Base> GAToGrant;

	bool IsValid() const;
};

/**
 * 这个类为武器提供数据
 */
USTRUCT(BlueprintType)
struct FHeroWeaponData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSubclassOf<UAnimInstance_HeroLinkedLayer> WeaponAnimLayerLink; 

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	UInputMappingContext* WeaponInputMappingContext;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta = (TitleProperty = "InputTag"))
	TArray<FHeroAbilitySet> DefaultWeaponAbility;
	//damage can be improved by level
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FScalableFloat WeaponBaseDamage;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> SoftWeaponIconTexture;
};


