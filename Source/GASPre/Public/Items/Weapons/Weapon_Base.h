// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon_Base.generated.h"

class UBoxComponent;

DECLARE_DELEGATE_OneParam(FOnTargetinteractedDelegate,AActor*)

UCLASS()
class GASPRE_API AWeapon_Base : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeapon_Base();

	FOnTargetinteractedDelegate OnWeaponHitTarget;
	FOnTargetinteractedDelegate OnWeaponPulledFromTarget;

protected:
	//武器的碰撞体和静态网格体
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Weapons")
	UStaticMeshComponent* WeaponMesh;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Weapons")
	UBoxComponent* WeaponBoxCollision;

	//collision beginoverlap callback function
	UFUNCTION()
	virtual void OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	//collision endoverlap callback function
	UFUNCTION()
	virtual void OnCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	FORCEINLINE UBoxComponent* GetWeaponCollision() const {return WeaponBoxCollision;}
};
