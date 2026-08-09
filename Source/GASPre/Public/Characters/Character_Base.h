// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interface/Interface_PawnCombatComponent.h"
#include "Interface/Interface_PawnUI.h"

#include "Character_Base.generated.h"

class UAbilitySystemComponent_Base;
class UAttributeSet_Base;
class UDA_StartUpDataBase;
class UMotionWarpingComponent;

UCLASS()
class GASPRE_API ACharacter_Base : public ACharacter,public IAbilitySystemInterface,public IInterface_PawnCombatComponent,public IInterface_PawnUI
{
	GENERATED_BODY()

public:
	ACharacter_Base();

	//~ Begin IAbilitySystemInterface Interface.
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~ End IAbilitySystemInterface Interface

	//~ Begin IInterface_PawnCombatComponent Interface.
	virtual UPawnCombatComponent* GetPawnCombatComponent() const override;
	//~ End IInterface_PawnCombatComponent Interface

	//~ Begin IInterface_PawnUI Interface.
	virtual UPawnUIComponent* GetPawnUIComponent() const override;
	//~ End IInterface_PawnUI Interface

protected:
	//~ Begin APawn Interface.
	virtual void PossessedBy(AController* NewController) override;
	//~ End APawn Interface

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "AbilitySys")
	UAbilitySystemComponent_Base* ASC;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "AbilitySys")
	UAttributeSet_Base* AS;
	//初始能力数据
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "StartDataAsset")
	TSoftObjectPtr<UDA_StartUpDataBase> DA_StartUpDataBase;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "MotionWarping")
	UMotionWarpingComponent* MotionWarpingComp;

public:
	FORCEINLINE UAbilitySystemComponent_Base* GetASC() const {return ASC;}
	FORCEINLINE UAttributeSet_Base* GetAS() const {return AS;}
};
