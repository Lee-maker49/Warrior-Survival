// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Character_Base.h"
#include "GameplayTagContainer.h"
#include "Character_Hero.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UDA_InputConfig;
struct FInputActionValue;
class UHeroCombatComponent;
class UHeroUIComponent;

/**
 * 
 */
UCLASS()
class GASPRE_API ACharacter_Hero : public ACharacter_Base
{
	GENERATED_BODY()

public:
	ACharacter_Hero();

	//~ Begin IInterface_PawnCombatComponent Interface.
	virtual UPawnCombatComponent* GetPawnCombatComponent() const override;
	//~ End IInterface_PawnCombatComponent Interface

	//~ Begin IInterface_PawnUI Interface.
	virtual UPawnUIComponent* GetPawnUIComponent() const override;
	virtual UHeroUIComponent* GetHeroUIComponent() const override;
	//~ End IInterface_PawnUI Interface

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;

	//~ Begin APawn Interface.
	//控制到角色的时候触发  加载StartUpData
	virtual void PossessedBy(AController* NewController) override;
	//~ End APawn Interface

private:

#pragma region Components
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Camera",meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Camera",meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Combat",meta = (AllowPrivateAccess = "true"))
	UHeroCombatComponent* HeroCombatComponent;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "UI",meta = (AllowPrivateAccess = "true"))
	UHeroUIComponent* HeroUIComponent;

#pragma endregion

#pragma region Input

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Input",meta = (AllowPrivateAccess = "true"))
	UDA_InputConfig* InputConfig;

	void Input_Move(const FInputActionValue& Value);
	void Input_Look(const FInputActionValue& Value);

	void Input_AbilityPressed(const FGameplayTag AbilityTag);
	void Input_AbilityReleased(const FGameplayTag AbilityTag);

#pragma endregion

public:
	FORCEINLINE UHeroCombatComponent* GetHeroCombatComponent() const {return HeroCombatComponent;}

};
