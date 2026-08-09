// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DA/StartUpData/DA_StartUpDataBase.h"
#include "Types/StructTypes.h"
#include "DA_StartUpDataHero.generated.h"



/**
 * 
 */
UCLASS()
class GASPRE_API UDA_StartUpDataHero : public UDA_StartUpDataBase
{
	GENERATED_BODY()
	
public:
	//把AbilitySets中的GA都赋予给ASC       赋予GA的时候顺手要把GA的Tag添加到Spec中
	virtual void GiveGAToASC(UAbilitySystemComponent_Base* TargetASC,int32 ApplyLevel = 1) override;
	
private:
	UPROPERTY(EditDefaultsOnly,Category = "StartUpData",meta = (TitleProperty = "InputTag"))
	TArray<FHeroAbilitySet> AbilitySets;

};
