// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance/Hero/AnimInstance_CharacterHero.h"
#include "Characters/Character_Hero.h"

void UAnimInstance_CharacterHero::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();
  
    if(OwningCharacter)
    {
        OwningHeroCharacter = Cast<ACharacter_Hero>(OwningCharacter);
    }
}

void UAnimInstance_CharacterHero::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

    if(bHasAcceleration)
    {
        IsShouldEnterRelaxState = false;
        IdleElpasedTime = 0.0f;
    }
    else
    {
        IdleElpasedTime += DeltaSeconds;
        IsShouldEnterRelaxState = (IdleElpasedTime >= InRelaxStateThreshold);
    }
}