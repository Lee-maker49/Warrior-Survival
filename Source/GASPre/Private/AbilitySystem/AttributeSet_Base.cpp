// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AttributeSet_Base.h"
#include "GameplayEffectExtension.h"
#include "FunctionLibrary.h"
#include "Tags_My.h"
#include "Interface/Interface_PawnUI.h"
#include "Components/UI/PawnUIComponent.h"
#include "Components/UI/HeroUIComponent.h"

#include "DebugHelpper.h"

UAttributeSet_Base::UAttributeSet_Base()
{
    InitCurrentHealth(1.f);
    InitMaxHealth(1.f);
    InitCurrentRage(1.f);
    InitMaxRage(1.f);
    InitAttackPower(1.f);
    InitDefensePower(1.f);
}
void UAttributeSet_Base::PostGameplayEffectExecute(const FGameplayEffectModCallbackData &Data)
{
    if(!CachedPawnUIInterface.IsValid())
    {
        CachedPawnUIInterface = TWeakInterfacePtr<IInterface_PawnUI>(Data.Target.GetAvatarActor());
    }
    
    checkf(CachedPawnUIInterface.IsValid(),TEXT("%s is didn't implement the interface"),*Data.Target.GetAvatarActor()->GetActorNameOrLabel())

    UPawnUIComponent* PawnUIComponent = CachedPawnUIInterface->GetPawnUIComponent();

    checkf(PawnUIComponent,TEXT("can not extrac a pawnuicomponent from %s"),*Data.Target.GetAvatarActor()->GetActorNameOrLabel());

    if(Data.EvaluatedData.Attribute == GetCurrentHealthAttribute())
    {
        const float NewCurrentHealth = FMath::Clamp(GetCurrentHealth(),0.f,GetMaxHealth());

        SetCurrentHealth(NewCurrentHealth);

        PawnUIComponent->OnCurrentHealthChanged.Broadcast(GetCurrentHealth()/GetMaxHealth());
    }
    if(Data.EvaluatedData.Attribute == GetCurrentRageAttribute())
    {
        const float NewCurrentRage = FMath::Clamp(GetCurrentRage(),0.f,GetMaxRage());

        SetCurrentRage(NewCurrentRage);

        if(UHeroUIComponent* HeroUIComponent = CachedPawnUIInterface->GetHeroUIComponent())
        {
            HeroUIComponent->OnCurrentRageChanged.Broadcast(GetCurrentRage()/GetMaxRage());
        }

    }
    if(Data.EvaluatedData.Attribute == GetDamageTakenAttribute())
    {
        /* Handle the calculation */
        const float OldHealth = GetCurrentHealth();
        const float DamageDone = GetDamageTaken();

        const float NewCurrentHealth = FMath::Clamp(OldHealth - DamageDone,0.f,GetMaxHealth());
        SetCurrentHealth(NewCurrentHealth);

        //const FString DebugString = FString::Printf(TEXT("OldHealth: %f; DamageTaken: %f; NewCurrentHealth: %f"),OldHealth,DamageDone,NewCurrentHealth);
        //Debug::Print(DebugString);
        
        /* Notify the UI */
        PawnUIComponent->OnCurrentHealthChanged.Broadcast(GetCurrentHealth()/GetMaxHealth());

        /* Handle the Character Death */
        if(GetCurrentHealth() == 0.f)
        {
            UFunctionLibrary::AddGameplayTagIfNone(Data.Target.GetAvatarActor(),MyGameplayTags::Shared_Status_Dead);
        }
    }
}