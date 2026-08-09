// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstance/AnimInstance_Base.h"
#include "AnimInstance_Character.generated.h"

class ACharacter_Base;
class UCharacterMovementComponent;
/**
 * 
 */
UCLASS()
class GASPRE_API UAnimInstance_Character : public UAnimInstance_Base
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY()
	ACharacter_Base* OwningCharacter;
	UPROPERTY()
	UCharacterMovementComponent* CharacterMovement;

	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly,Category = "AnimData|Locomotion")
	float GroundSpeed;
	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly,Category = "AnimData|Locomotion")
	bool bHasAcceleration;

	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly,Category = "AnimData|Locomotion")
	float LocomotionDirection;
};
