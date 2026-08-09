// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance/AnimInstance_Base.h"
#include "FunctionLibrary.h"

bool UAnimInstance_Base::DoesOwnerHasTag(FGameplayTag TagToCheck) const
{
    if(APawn* OwnerPawn = TryGetPawnOwner())
    {
        return UFunctionLibrary::NativeDoesActorHaveTag(OwnerPawn,TagToCheck);
    }
    
    return false;
}