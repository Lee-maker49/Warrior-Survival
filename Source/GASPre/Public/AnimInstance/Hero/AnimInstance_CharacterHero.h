// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstance/AnimInstance_Character.h"
#include "AnimInstance_CharacterHero.generated.h"

class ACharacter_Hero;
/**
 * 
 */
UCLASS()
class GASPRE_API UAnimInstance_CharacterHero : public UAnimInstance_Character
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly,Category = "AnimData|Reference")
	ACharacter_Hero* OwningHeroCharacter;

	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly,Category = "AnimData|Data")
	bool IsShouldEnterRelaxState;
	UPROPERTY(EditdefaultsOnly,BlueprintReadOnly,Category = "AnimData|Data")
	float InRelaxStateThreshold = 5.0f;

	float IdleElpasedTime = 0.0f;
};
