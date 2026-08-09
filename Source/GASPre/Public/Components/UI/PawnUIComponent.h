// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnExtensionComponent_Base.h"
#include "PawnUIComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPercentChangeDelegate,float,NewPercent);

/**
 * 这个类用于定义Pawn的属性委托
 */
UCLASS()
class GASPRE_API UPawnUIComponent : public UPawnExtensionComponent_Base
{
	GENERATED_BODY()

public:
	//BlueprintAssignable：让蓝图可以绑定和监听这个委托
	//定义当前血量委托
	UPROPERTY(BlueprintAssignable)
	FOnPercentChangeDelegate OnCurrentHealthChanged;

};
