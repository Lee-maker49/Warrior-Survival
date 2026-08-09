// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Combat/HeroCombatComponent.h"
#include "Items/Weapons/Weapon_Hero.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Tags_My.h"

#include "DebugHelpper.h"

AWeapon_Hero *UHeroCombatComponent::GetHeroWeaponByTag(FGameplayTag WeaponTag) const
{
    return Cast<AWeapon_Hero>(GetWeaponByTag(WeaponTag));
}

AWeapon_Hero *UHeroCombatComponent::GetHeroCurrentEquipedWeapon() const
{
    return Cast<AWeapon_Hero>(GetCurrentEquippedWeapon());
}
float UHeroCombatComponent::GetHeroCurrentEquipedWeaponDamageAtLevel(float InLevel) const
{
    return GetHeroCurrentEquipedWeapon()->HeroWeaponData.WeaponBaseDamage.GetValueAtLevel(InLevel);
}
// add the hit actor to the tarray of the hitactor and send the event to the owningactor
void UHeroCombatComponent::OnHitTargetActor(AActor *TargetActor)
{
    if(HitActors.Contains(TargetActor))
        return;
    HitActors.AddUnique(TargetActor);

    FGameplayEventData Data;
    Data.Instigator = GetOwningPawn();
    Data.Target = TargetActor;
    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
        GetOwningPawn(),
        MyGameplayTags::Shared_Event_MeleeHit,
        Data
    );

    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
        GetOwningPawn(),
        MyGameplayTags::Player_Event_HitPause,
        FGameplayEventData()
    );


}
void UHeroCombatComponent::OnWeaponPulledFromTargetActor(AActor *InteractedActor)
{
    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
        GetOwningPawn(),
        MyGameplayTags::Player_Event_HitPause,
        FGameplayEventData()
    );
}