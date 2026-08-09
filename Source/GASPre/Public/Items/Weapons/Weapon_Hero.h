// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Weapons/Weapon_Base.h"
#include "Types/StructTypes.h"
#include "GameplayAbilitySpecHandle.h"

#include "Weapon_Hero.generated.h"

/**
 * 这个类提供英雄武器的武器数据
 */
UCLASS()
class GASPRE_API AWeapon_Hero : public AWeapon_Base
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "WeaponData")
	FHeroWeaponData HeroWeaponData;

	UFUNCTION(BlueprintCallable)
	void AssignWeaponCarriedGASpecHandles(const TArray<FGameplayAbilitySpecHandle> &InWeaponCarriedGASpecHandles);
	UFUNCTION(BlueprintPure)
	TArray<FGameplayAbilitySpecHandle> GetWeaponCarriedGASpecHandles() const;

private:
	TArray<FGameplayAbilitySpecHandle> WeaponCarriedGASpecHandles;

};
