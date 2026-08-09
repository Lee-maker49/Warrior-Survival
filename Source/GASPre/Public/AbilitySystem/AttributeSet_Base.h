// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystem/AbilitySystemComponent_Base.h"

#include "AttributeSet_Base.generated.h"

class IInterface_PawnUI;

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
/**
 * 
 */
UCLASS()
class GASPRE_API UAttributeSet_Base : public UAttributeSet
{
	GENERATED_BODY()

public:
	UAttributeSet_Base();

	//对属性值进行修正，如果没有修正最终就是使用GE的，进行修正了就是用他的
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData &Data) override;

	UPROPERTY(BlueprintReadOnly,Category = "Health")
	FGameplayAttributeData CurrentHealth;
	ATTRIBUTE_ACCESSORS(UAttributeSet_Base,CurrentHealth)

	UPROPERTY(BlueprintReadOnly,Category = "Health")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UAttributeSet_Base,MaxHealth)

	UPROPERTY(BlueprintReadOnly,Category = "Rage")
	FGameplayAttributeData CurrentRage;
	ATTRIBUTE_ACCESSORS(UAttributeSet_Base,CurrentRage)

	UPROPERTY(BlueprintReadOnly,Category = "Rage")
	FGameplayAttributeData MaxRage;
	ATTRIBUTE_ACCESSORS(UAttributeSet_Base,MaxRage)

	UPROPERTY(BlueprintReadOnly,Category = "Damage")
	FGameplayAttributeData AttackPower;
	ATTRIBUTE_ACCESSORS(UAttributeSet_Base,AttackPower)

	UPROPERTY(BlueprintReadOnly,Category = "Damage")
	FGameplayAttributeData DefensePower;
	ATTRIBUTE_ACCESSORS(UAttributeSet_Base,DefensePower)
	//
	UPROPERTY(BlueprintReadOnly,Category = "Damage")
	FGameplayAttributeData DamageTaken;
	ATTRIBUTE_ACCESSORS(UAttributeSet_Base,DamageTaken)

private:
	TWeakInterfacePtr<IInterface_PawnUI> CachedPawnUIInterface;

};
