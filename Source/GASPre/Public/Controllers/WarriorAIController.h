// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "WarriorAIController.generated.h"

class UAIPerceptionComponent;
class UAISenseConfig_Sight;

/**
 * 
 */
UCLASS()
class GASPRE_API AWarriorAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AWarriorAIController(const FObjectInitializer& ObjectInitializer);

	//~ Begin IGenericTeamAgentInterface Interface.
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	//~ End IGenericTeamAgentInterface Interface
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	UAIPerceptionComponent* EnemyPerceptionComponent;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	UAISenseConfig_Sight* AISenseConfig_Sight;
					
	UFUNCTION()
	void OnEnemyPerceptionUpdate(AActor* Actor, FAIStimulus Stimulus);

private:
	UPROPERTY(EditDefaultsOnly,Category = "Detour Crowd Avoidance Config")
	bool bEnanbleDetourCrowdAvoidence = true;

	UPROPERTY(EditDefaultsOnly,Category = "Detour Crowd Avoidance Config",meta = (EditCondition = "bEnanbleDetourCrowdAvoidence",UIMIN = "1",UIMAX = "4"))
	int32 DetourCrowdAvoidenceQuality = 4;

	UPROPERTY(EditDefaultsOnly,Category = "Detour Crowd Avoidance Config",meta = (EditCondition = "bEnanbleDetourCrowdAvoidence"))
	float CollisionQueryRange = 600.f;
};
