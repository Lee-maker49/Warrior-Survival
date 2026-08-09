// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DA/StartUpData/DA_StartUpDataBase.h"
#include "DA_StartUpDataEnemy.generated.h"

class UGameplayAbility_Enemy;
/**
 * 
 */
UCLASS()
class GASPRE_API UDA_StartUpDataEnemy : public UDA_StartUpDataBase
{
	GENERATED_BODY()

public:
	//Enemy's GiveGaToASC Added the process of adding EnemyAbilities to ASC
	virtual void GiveGAToASC(UAbilitySystemComponent_Base* TargetASC,int32 ApplyLevel = 1) override;

private:
	//Enemy specific GameplayAbility
	UPROPERTY(EditDefaultsOnly,Category = "StartUpData")
	TArray< TSubclassOf <UGameplayAbility_Enemy> > EnemyAbilities;
	
};
