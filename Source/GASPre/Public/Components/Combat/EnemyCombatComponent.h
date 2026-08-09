// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Combat/PawnCombatComponent.h"
#include "EnemyCombatComponent.generated.h"

/**
 * 
 */
UCLASS()
class GASPRE_API UEnemyCombatComponent : public UPawnCombatComponent
{
	GENERATED_BODY()

public:
	//set the enemy version of the OnHitTargetActor function which is the callback function when weapon collision begin
	virtual void OnHitTargetActor(AActor* TargetActor) override;
	
};
