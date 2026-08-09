// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GenericTeamAgentInterface.h"
#include "PlayerController_Base.generated.h"

/**
 * 
 */
UCLASS()
class GASPRE_API APlayerController_Base : public APlayerController,public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	APlayerController_Base();
	//~ Begin IGenericTeamAgentInterface Interface.
	virtual FGenericTeamId GetGenericTeamId() const override;
	//~ End IGenericTeamAgentInterface Interface
private:
	FGenericTeamId HeroTeamId;
};
