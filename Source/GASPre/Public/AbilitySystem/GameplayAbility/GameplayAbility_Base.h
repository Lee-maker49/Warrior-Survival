// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Types/EnumTypes.h"

#include "GameplayAbility_Base.generated.h"

class UPawnCombatComponent;
class UAbilitySystemComponent_Base;

UENUM(BlueprintType)
enum class EAbilityActivationPolicy : uint8
{
	OnTrigger,
	OnGiven
};
/**
 * 
 */
UCLASS()
class GASPRE_API UGameplayAbility_Base : public UGameplayAbility
{
	GENERATED_BODY()

protected:
	//~ Begin UGameplayAbility Interface.
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End UGameplayAbility Interface
	UPROPERTY(EditDefaultsOnly,Category = "GA")
	EAbilityActivationPolicy ActivationPolicy = EAbilityActivationPolicy::OnTrigger;

	UFUNCTION(BlueprintPure,Category = "MyAbility")
	UPawnCombatComponent* GetPawnCombatComponentFromActorInfo() const;
	UFUNCTION(BlueprintPure,Category = "MyAbility")
	UAbilitySystemComponent_Base* GetMyAbilitySystemComponentFromActorInfo() const;

	//native version of applying effectspechandle to targetactorasc
	FActiveGameplayEffectHandle NativeApplyEffectSpecHandleToTarget(AActor* TargetActor,const FGameplayEffectSpecHandle &SpecHandleToUSe);
	//BP version of applying effectspechandle to targetactorasc
	UFUNCTION(BlueprintCallable,Category = "MyAbility",meta = (DisplayName = "Apply Effect Spec Handle To Target" ,ExpandEnumAsExecs = "OutSuccessType"))
	FActiveGameplayEffectHandle BP_ApplyEffectSpecHandleToTarget(AActor* TargetActor,const FGameplayEffectSpecHandle &SpecHandleToUSe,ESuccessType &OutSuccessType);

};
