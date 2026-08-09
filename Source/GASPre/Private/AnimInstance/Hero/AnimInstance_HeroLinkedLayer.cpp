// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance/Hero/AnimInstance_HeroLinkedLayer.h"
#include "AnimInstance/Hero/AnimInstance_CharacterHero.h"

UAnimInstance_CharacterHero *UAnimInstance_HeroLinkedLayer::GetHeroAnimInstance() const
{
    return Cast<UAnimInstance_CharacterHero>(GetOwningComponent()->GetAnimInstance());
}