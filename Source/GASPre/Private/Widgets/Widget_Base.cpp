// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Widget_Base.h"
#include "Interface/Interface_PawnUI.h"

void UWidget_Base::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if(IInterface_PawnUI* Interface_PawnUI = Cast<IInterface_PawnUI>(GetOwningPlayerPawn()))
    {
        if(UHeroUIComponent* HeroUIComponent = Interface_PawnUI->GetHeroUIComponent())
        {
            BP_OnOwningHeroUIComponentInitialize(HeroUIComponent);
        }
    }

}

void UWidget_Base::InitEnemyCreateWidget(AActor* OwningEnemyActor)
{
    if(IInterface_PawnUI* Interface_PawnUI = Cast<IInterface_PawnUI>(OwningEnemyActor))
    {
        UEnemyUIComponent* EnemyUIComponent = Interface_PawnUI->GetEnemyUIComponent();

        checkf(EnemyUIComponent,TEXT("do not find the enemy ui component"));

        BP_OnOwningEnemyUIComponentInitialize(EnemyUIComponent);
    }
}
