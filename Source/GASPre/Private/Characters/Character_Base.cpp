// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Character_Base.h"
#include "AbilitySystem/AbilitySystemComponent_Base.h"
#include "AbilitySystem/AttributeSet_Base.h"
#include "MotionWarpingComponent.h"

// Sets default values
ACharacter_Base::ACharacter_Base()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	GetMesh()->bReceivesDecals = false;

	//在Base中构造ASC和AS components
	ASC = CreateDefaultSubobject<UAbilitySystemComponent_Base>(TEXT("ASC"));
	AS = CreateDefaultSubobject<UAttributeSet_Base>(TEXT("AS"));

	MotionWarpingComp = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComp"));
}

UAbilitySystemComponent *ACharacter_Base::GetAbilitySystemComponent() const
{
    return GetASC();
}
UPawnCombatComponent *ACharacter_Base::GetPawnCombatComponent() const
{
    return nullptr;
}
UPawnUIComponent *ACharacter_Base::GetPawnUIComponent() const
{
    return nullptr;
}
void ACharacter_Base::PossessedBy(AController *NewController)
{
	Super::PossessedBy(NewController);
	if(ASC)
	{
		ASC->InitAbilityActorInfo(this,this);
		
		ensureMsgf(!DA_StartUpDataBase.IsNull(),TEXT("StartUpData Is Null Actor Name Is : %s"),*GetName());
	}
}
