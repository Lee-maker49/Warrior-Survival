// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_RotateToFaceTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Kismet/KismetMathLibrary.h"

UBTTask_RotateToFaceTarget::UBTTask_RotateToFaceTarget()
{
    NodeName = TEXT("Native Rotate To Face Target");

    AnglePrecision = 10.f;
    RotationInterpSpeed = 5.f;

    bNotifyTick = true;
    bNotifyTaskFinished = true;
    bCreateNodeInstance = false;

    INIT_TASK_NODE_NOTIFY_FLAGS();

    TargetActorToFaceKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_RotateToFaceTarget, TargetActorToFaceKey), AActor::StaticClass());

}

void UBTTask_RotateToFaceTarget::InitializeFromAsset(UBehaviorTree& Asset)
{
    Super::InitializeFromAsset(Asset);

    //resolve the BB Key
    UBlackboardData* BlackboardData = GetBlackboardAsset();
    if (BlackboardData)
    {
        TargetActorToFaceKey.ResolveSelectedKey(*BlackboardData);
    }
}

uint16 UBTTask_RotateToFaceTarget::GetInstanceMemorySize() const
{
    return sizeof(FRotateToFaceTargetMemory);
}

FString UBTTask_RotateToFaceTarget::GetStaticDescription() const
{
    const FString KeyDescription = TargetActorToFaceKey.SelectedKeyName.ToString();

    return FString::Printf(TEXT("Rotate To Face Target: %s"), *KeyDescription);
}
//维护数组和给出当前节点状态
EBTNodeResult::Type UBTTask_RotateToFaceTarget::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    UObject* TargetObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetActorToFaceKey.SelectedKeyName);
    AActor* TargetActor = Cast<AActor>(TargetObject);

    APawn* OwningPawn = OwnerComp.GetAIOwner()->GetPawn();

    FRotateToFaceTargetMemory* Memory = CastInstanceNodeMemory<FRotateToFaceTargetMemory>(NodeMemory);
    check(Memory);

    Memory->OwningPawn = OwningPawn;
    Memory->TargetActor = TargetActor;

    if(!Memory->IsValid())
    {
        return EBTNodeResult::Failed;
    }

    if(HasReachedAnglePrecision(OwningPawn, TargetActor))
    {
        Memory->Reset();
        return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::InProgress;
}

void UBTTask_RotateToFaceTarget::TickTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds)
{
    FRotateToFaceTargetMemory* Memory = CastInstanceNodeMemory<FRotateToFaceTargetMemory>(NodeMemory);

    if(!Memory->IsValid())
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
    }

    if(HasReachedAnglePrecision(Memory->OwningPawn.Get(), Memory->TargetActor.Get()))
    {
        Memory->Reset();
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
    else
    {
        const FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(Memory->OwningPawn.Get()->GetActorLocation(), Memory->TargetActor.Get()->GetActorLocation());
        const FRotator TargetRot = FMath::RInterpTo(Memory->OwningPawn.Get()->GetActorRotation(), LookAtRot, DeltaSeconds, RotationInterpSpeed);

        Memory->OwningPawn.Get()->SetActorRotation(TargetRot);
    }
}

bool UBTTask_RotateToFaceTarget::HasReachedAnglePrecision(APawn *QueryPawn, AActor *TargetActor) const
{
    const FVector OwneForward = QueryPawn->GetActorForwardVector();
    const FVector ToTarget = (TargetActor->GetActorLocation() - QueryPawn->GetActorLocation()).GetSafeNormal();

    const float DotProduct = FVector::DotProduct(OwneForward, ToTarget);
    const float AngleDiff = UKismetMathLibrary::DegAcos(DotProduct);

    return AngleDiff <= AnglePrecision;
}