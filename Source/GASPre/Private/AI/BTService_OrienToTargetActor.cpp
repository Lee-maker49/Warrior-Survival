// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_OrienToTargetActor.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Kismet/KismetMathLibrary.h"

UBTService_OrienToTargetActor::UBTService_OrienToTargetActor()
{
    NodeName = TEXT("Native Orient Rotation To Target Actor");

    INIT_SERVICE_NODE_NOTIFY_FLAGS();

    RotationInterpSpeed = 5.f;
    Interval = 0.f;
    RandomDeviation = 0.f;

    InTargetActorKey.AddObjectFilter(this,GET_MEMBER_NAME_CHECKED(ThisClass,InTargetActorKey),AActor::StaticClass());

}

void UBTService_OrienToTargetActor::InitializeFromAsset(UBehaviorTree& Asset)
{
    Super::InitializeFromAsset(Asset);

    //resolve the BBkeyselector
    if(UBlackboardData* BlackBoardData = GetBlackboardAsset())
    {
        InTargetActorKey.ResolveSelectedKey(*BlackBoardData);
    }
}

FString UBTService_OrienToTargetActor::GetStaticDescription() const
{
    const FString KeyName = InTargetActorKey.SelectedKeyName.ToString();

    return FString::Printf(TEXT("Orien To Target: %s Key %s"),*KeyName,*GetStaticServiceDescription());
}
//Tick Node Function
void UBTService_OrienToTargetActor::TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp,NodeMemory,DeltaSeconds);

    //get the targetactor
    UObject* TargetObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(InTargetActorKey.SelectedKeyName);
    AActor* TargetActor = Cast<AActor>(TargetObject);

    //get the owneractor
    APawn* OwnerPawn = OwnerComp.GetAIOwner()->GetPawn();

    if(OwnerPawn && TargetActor)
    {
        const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(OwnerPawn->GetActorLocation(),TargetActor->GetActorLocation());
        const FRotator TargetRot = FMath::RInterpTo(OwnerPawn->GetActorRotation(),LookAtRotation,DeltaSeconds,RotationInterpSpeed);

        OwnerPawn->SetActorRotation(TargetRot);
    }

}