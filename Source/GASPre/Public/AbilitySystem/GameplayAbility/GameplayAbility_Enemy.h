// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/GameplayAbility/GameplayAbility_Base.h"
#include "GameplayAbility_Enemy.generated.h"

class ACharacter_Enemy;
class UEnemyCombatComponent;
/**
 * 
 */
UCLASS()
class GASPRE_API UGameplayAbility_Enemy : public UGameplayAbility_Base
{
	GENERATED_BODY()

public:
	//辅助函数
	//find Enemy Character
	UFUNCTION(BlueprintPure,Category = "Enemy|Ability")
	ACharacter_Enemy* GetEnemyCharacterFromActorInfo();    
	//find Enemy Combat Component           
	UFUNCTION(BlueprintPure,Category = "Enemy|Ability")
	UEnemyCombatComponent* GetEnemyCombatComponentFromActorInfo();

	//a helper function to make damage effect spec handle for enemy in order to apply damage effect to target
	UFUNCTION(BlueprintPure,Category = "Enemy|Ability")
	FGameplayEffectSpecHandle MakeEnemyDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, const FScalableFloat& InDamageScalableFloat);

private:
	//弱引用智能指针，不会阻止GC回收对象
	TWeakObjectPtr<ACharacter_Enemy> CachedEnemyCharacter;
};
