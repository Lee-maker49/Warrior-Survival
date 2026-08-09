// Fill out your copyright notice in the Description page of Project Settings.


#include "Interface/Interface_PawnUI.h"

// Add default functionality here for any IInterface_PawnUI functions that are not pure virtual.

UHeroUIComponent *IInterface_PawnUI::GetHeroUIComponent() const
{
    return nullptr;
}

UEnemyUIComponent *IInterface_PawnUI::GetEnemyUIComponent() const
{
    return nullptr;
}
