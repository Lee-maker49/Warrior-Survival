// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Combat/PawnCombatComponent.h"
#include "Items/Weapons/Weapon_Base.h"
#include "Components/BoxComponent.h"

#include "DebugHelpper.h"

//把武器注册到武器表中
void UPawnCombatComponent::RegisterSpawnedWeapon(FGameplayTag WeaponTagToRegister, AWeapon_Base *WeaponToRegister, bool bRegisterAsEquippedWeapon)
{
    checkf(!CharacterCarriedWeaponMap.Contains(WeaponTagToRegister),TEXT("A WeaponTag Named %s has been Registered"),*WeaponTagToRegister.ToString());
    check(WeaponToRegister);

    CharacterCarriedWeaponMap.Emplace(WeaponTagToRegister,WeaponToRegister);

    //set the delegate callback function when the weapon was registed
    WeaponToRegister->OnWeaponHitTarget.BindUObject(this,&ThisClass::OnHitTargetActor);
    WeaponToRegister->OnWeaponPulledFromTarget.BindUObject(this,&ThisClass::OnWeaponPulledFromTargetActor);

    if(bRegisterAsEquippedWeapon)
        CurrentEquippedWeaponTag = WeaponTagToRegister;

    //const FString Msg = FString::Printf(TEXT("A Weapon named %s has been registered by Tag named %s"),*WeaponToRegister->GetName(),*WeaponTagToRegister.ToString());
    //Debug::Print(Msg);
}
//通过Tag找武器
AWeapon_Base *UPawnCombatComponent::GetWeaponByTag(FGameplayTag WeaponTagToGet) const
{
    if(CharacterCarriedWeaponMap.Contains(WeaponTagToGet))
    {
        if(AWeapon_Base* const* Weapon = CharacterCarriedWeaponMap.Find(WeaponTagToGet))
        {
            return *Weapon;
        }
    }
    return nullptr;
}
//找到现在装备的武器
AWeapon_Base *UPawnCombatComponent::GetCurrentEquippedWeapon() const
{
    if(!CurrentEquippedWeaponTag.IsValid())
        return nullptr;
    
    return GetWeaponByTag(CurrentEquippedWeaponTag);
}

void UPawnCombatComponent::ToggleWeaponCollision(bool beShouldEnableCollision, EDamageType DamageType)
{
    if(DamageType == EDamageType::CurrentWeapon)
    {
        AWeapon_Base* CurrentWeapon = GetCurrentEquippedWeapon();

        check(CurrentWeapon);

        if(beShouldEnableCollision)
        {
            CurrentWeapon->GetWeaponCollision()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
        }
        else
        {
            CurrentWeapon->GetWeaponCollision()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        
            HitActors.Empty();
        }
    }
}

void UPawnCombatComponent::OnHitTargetActor(AActor *TargetActor)
{
}

void UPawnCombatComponent::OnWeaponPulledFromTargetActor(AActor *InteractedActor)
{
}
