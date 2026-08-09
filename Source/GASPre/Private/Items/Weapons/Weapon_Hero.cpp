// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Weapon_Hero.h"

void AWeapon_Hero::AssignWeaponCarriedGASpecHandles(const TArray<FGameplayAbilitySpecHandle> &InWeaponCarriedGASpecHandles)
{
    WeaponCarriedGASpecHandles = InWeaponCarriedGASpecHandles;
}

TArray<FGameplayAbilitySpecHandle> AWeapon_Hero::GetWeaponCarriedGASpecHandles() const
{
    return WeaponCarriedGASpecHandles;
}
