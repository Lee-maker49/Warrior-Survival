// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_RotateToFaceTarget.generated.h"

struct FRotateToFaceTargetMemory
{
	TWeakObjectPtr<APawn> OwningPawn;
	TWeakObjectPtr<AActor> TargetActor;

	bool IsValid() const
	{
		return OwningPawn.IsValid() && TargetActor.IsValid();
	}

	void Reset()
	{
		OwningPawn.Reset();
		TargetActor.Reset();
	}

};

/**
 * 
 */
UCLASS()
class GASPRE_API UBTTask_RotateToFaceTarget : public UBTTaskNode
{
	GENERATED_BODY()
	
	UBTTask_RotateToFaceTarget();

	//~ Begin UBTNode Interface
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual uint16 GetInstanceMemorySize() const override;
	virtual FString GetStaticDescription() const override;
	//~ End UBTNode Interface

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);

	bool HasReachedAnglePrecision(APawn* QueryPawn, AActor* TargetActor) const;

	UPROPERTY(EditAnywhere,Category = "Face Target")
	float AnglePrecision;

	UPROPERTY(EditAnywhere,Category = "Face Target")
	float RotationInterpSpeed;

	UPROPERTY(EditAnywhere,Category = "Face Target")
	FBlackboardKeySelector TargetActorToFaceKey;
};
