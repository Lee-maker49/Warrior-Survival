// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstance/AnimInstance_Base.h"
#include "AnimInstance_HeroLinkedLayer.generated.h"

class UAnimInstance_CharacterHero;
/**
 * 
 */
UCLASS()
class GASPRE_API UAnimInstance_HeroLinkedLayer : public UAnimInstance_Base
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure,meta = (BlueprintThreadSafe))
	UAnimInstance_CharacterHero* GetHeroAnimInstance() const;
	
};
