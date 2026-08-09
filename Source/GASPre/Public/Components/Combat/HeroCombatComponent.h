// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Combat/PawnCombatComponent.h"
#include "HeroCombatComponent.generated.h"

class AWeapon_Hero;
/**
 * 
 */
UCLASS()
class GASPRE_API UHeroCombatComponent : public UPawnCombatComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable,Category = "Combat")
	AWeapon_Hero* GetHeroWeaponByTag(FGameplayTag WeaponTag) const;

	//get the current equiped weapon
	UFUNCTION(BlueprintCallable,Category = "Combat")
	AWeapon_Hero* GetHeroCurrentEquipedWeapon() const;
	//get the current equiped weapon weaponbasedamage at level
	UFUNCTION(BlueprintCallable,Category = "Combat")
	float GetHeroCurrentEquipedWeaponDamageAtLevel(float InLevel) const;

	// override the callback function when weapon collision begin and end overlap Hitpawn
	virtual void OnHitTargetActor(AActor* TargetActor) override;
	virtual void OnWeaponPulledFromTargetActor(AActor* InteractedActor) override;

};
