// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Weapon_Base.h"
#include "Components/BoxComponent.h"
#include "FunctionLibrary.h"

#include "DebugHelpper.h"

AWeapon_Base::AWeapon_Base()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);

	WeaponBoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCollision"));
	WeaponBoxCollision->SetupAttachment(RootComponent);
	WeaponBoxCollision->SetBoxExtent(FVector(20.f));
	WeaponBoxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//set collision delegate function
	WeaponBoxCollision->OnComponentBeginOverlap.AddUniqueDynamic(this,&ThisClass::OnCollisionBeginOverlap);
	WeaponBoxCollision->OnComponentEndOverlap.AddUniqueDynamic(this,&ThisClass::OnCollisionEndOverlap);
}

//ensure hit the enemy and exec the delegate. the callback function of the delegate is defined by the cambatcomponent
void AWeapon_Base::OnCollisionBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	APawn* WeaponOwningPawn = GetInstigator<APawn>();
	checkf(WeaponOwningPawn,TEXT("forget assign instigator as owner of ths Weapon %s"),*GetName());

	if(APawn* HitPawn = Cast<APawn>(OtherActor))
	{
		if(UFunctionLibrary::IsTargetPawnHostile(WeaponOwningPawn, HitPawn))
		{
			OnWeaponHitTarget.ExecuteIfBound(OtherActor);
		}
	}
}
void AWeapon_Base::OnCollisionEndOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
	APawn* WeaponOwningPawn = GetInstigator<APawn>();
	checkf(WeaponOwningPawn,TEXT("forget assign instigator as owner of ths Weapon %s"),*GetName());

	if(APawn* HitPawn = Cast<APawn>(OtherActor))
	{
		if(UFunctionLibrary::IsTargetPawnHostile(WeaponOwningPawn, HitPawn))
		{
			OnWeaponPulledFromTarget.ExecuteIfBound(OtherActor);
		}
	}
}
