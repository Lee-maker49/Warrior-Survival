// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GameplayTagContainer.h"

#include "AnimInstance_Base.generated.h"

/**
 * 
 */
UCLASS()
class GASPRE_API UAnimInstance_Base : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
	bool DoesOwnerHasTag(FGameplayTag TagToCheck) const;

};
