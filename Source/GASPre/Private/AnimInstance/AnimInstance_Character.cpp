// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance/AnimInstance_Character.h"
#include "Characters/Character_Base.h"
#include "GameFrameWork/CharacterMovementComponent.h"
#include "KismetAnimationLibrary.h"


void UAnimInstance_Character::NativeInitializeAnimation()
{
    OwningCharacter = Cast<ACharacter_Base>(TryGetPawnOwner());
    
    if(OwningCharacter)
    {
        CharacterMovement = OwningCharacter->GetCharacterMovement();
    }
}

void UAnimInstance_Character::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
    if(!OwningCharacter || !CharacterMovement)
        return;
    
    GroundSpeed = OwningCharacter->GetVelocity().Size2D();
    bHasAcceleration = CharacterMovement->GetCurrentAcceleration().SizeSquared2D() > 0.5f;
    
    LocomotionDirection = UKismetAnimationLibrary::CalculateDirection(OwningCharacter->GetVelocity(), OwningCharacter->GetActorRotation());

}