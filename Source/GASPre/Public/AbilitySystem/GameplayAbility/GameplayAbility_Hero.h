// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/GameplayAbility/GameplayAbility_Base.h"
#include "GameplayAbility_Hero.generated.h"

class ACharacter_Hero;
class APlayerController_Base;
class UHeroCombatComponent;
/**
 * 
 */
UCLASS()
class GASPRE_API UGameplayAbility_Hero : public UGameplayAbility_Base
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure,Category = "Ability")
	ACharacter_Hero* GetHeroCharacterFromActorInfo();
	UFUNCTION(BlueprintPure,Category = "Ability")
	APlayerController_Base* GetControllerFromActorInfo();
	UFUNCTION(BlueprintPure,Category = "Ability")
	UHeroCombatComponent* GetHeroCombatComponentFromActorInfo();

	//provide effectspechandle of damage effect
	UFUNCTION(BlueprintPure,Category = "Ability")
	FGameplayEffectSpecHandle MakeHeroDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass,float InWeaponBaseDamage,FGameplayTag InCurrentAttackTypeTag,int32 InCurrentComboCount);

private:
	TWeakObjectPtr<ACharacter_Hero> HeroCharacter;
	TWeakObjectPtr<APlayerController_Base> Controller;
	
};
