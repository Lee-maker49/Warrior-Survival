// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_OrienToTargetActor.generated.h"

/**
 * 
 */
UCLASS()
class GASPRE_API UBTService_OrienToTargetActor : public UBTService
{
	GENERATED_BODY()

	UBTService_OrienToTargetActor();

	//~ Begin UBTNode Interface
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	//set the message shown in ths service node
	virtual FString GetStaticDescription() const override;
	//~ End UBTNode Interface

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere,Category = "Target")
	FBlackboardKeySelector InTargetActorKey;

	UPROPERTY(EditAnywhere,Category = "Target")
	float RotationInterpSpeed;
	
};
