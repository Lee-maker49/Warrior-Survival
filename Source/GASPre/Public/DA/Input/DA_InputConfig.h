// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "DA_InputConfig.generated.h"

class UInputAction;
class UInputMappingContext;


USTRUCT(BlueprintType)
struct FInputDataStruct
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta = (Categories = "InputTag"))
	FGameplayTag InputTag;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	UInputAction* InputAction;

	bool IsValid() const
	{
		return InputTag.IsValid() && InputAction;
	}
};

/**
 * 这个类为玩家角色提供输入配置数据
 * 他只是把输入标签和IA联系起来了 通过寻找标签去找到相应的InputAction
 */
UCLASS()
class GASPRE_API UDA_InputConfig : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta = (TitleProperty = "InputTag"))
	TArray<FInputDataStruct> NativeInputStructActions;

	UInputAction* FindInputActionByTag(const FGameplayTag& InputTag)  const;

	//GA的输入事件配置，输入会产生输入事件，在输入事件中让ASC在已经赋予的GA中激活对应Tag的GA
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta = (TitleProperty = "InputTag"))
	TArray<FInputDataStruct> AbilityInputStructAbilityActions;
};
