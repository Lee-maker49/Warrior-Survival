// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnExtensionComponent_Base.h"
#include "GameplayTagContainer.h"
#include "PawnCombatComponent.generated.h"

class AWeapon_Base;

UENUM(BlueprintType)
enum class EDamageType : uint8
{
	CurrentWeapon,
	LeftHand,
	RightHand
};

/**
 * 这个类是一个Actor组件，为Actor提供存储和访问武器的方法和属性
 */
UCLASS()
class GASPRE_API UPawnCombatComponent : public UPawnExtensionComponent_Base
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable,Category = "Combat")
	void RegisterSpawnedWeapon(FGameplayTag WeaponTagToRegister,AWeapon_Base* WeaponToRegister,bool bRegisterAsEquippedWeapon = false);
	UFUNCTION(BlueprintCallable,Category = "Combat")
	AWeapon_Base* GetWeaponByTag(FGameplayTag WeaponTagToGet) const;

	UPROPERTY(BlueprintReadWrite,Category = "Combat")
	FGameplayTag CurrentEquippedWeaponTag;

	UFUNCTION(BlueprintCallable,Category = "Combat")
	AWeapon_Base* GetCurrentEquippedWeapon() const;

	//toggle the Weapon Collision
	UFUNCTION(BlueprintCallable,Category = "Combat")
	void ToggleWeaponCollision(bool beShouldEnableCollision,EDamageType DamageType = EDamageType::CurrentWeapon);

	//the callback function when weapon collision begin and end overlap Hitpawn. bind with the delegate when weapon registered.
	virtual void OnHitTargetActor(AActor* TargetActor);
	virtual void OnWeaponPulledFromTargetActor(AActor* InteractedActor);

protected:
	//the hit actors of exec one attack.
	TArray<AActor*> HitActors;

private:
	TMap<FGameplayTag,AWeapon_Base*> CharacterCarriedWeaponMap;

};
