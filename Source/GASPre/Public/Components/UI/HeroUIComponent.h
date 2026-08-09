// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/UI/PawnUIComponent.h"
#include "HeroUIComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponChangedUIDelegate,TSoftObjectPtr<UTexture2D>,SoftWeaponUI);

/**
 * 
 */
UCLASS()
class GASPRE_API UHeroUIComponent : public UPawnUIComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnPercentChangeDelegate OnCurrentRageChanged;

	UPROPERTY(BlueprintCallable,BlueprintAssignable)
	FOnWeaponChangedUIDelegate WeaponChangedUIDelegate;
};
