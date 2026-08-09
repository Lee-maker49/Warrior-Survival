// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/GameMode_Base.h"

AGameMode_Base::AGameMode_Base()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = true;
}