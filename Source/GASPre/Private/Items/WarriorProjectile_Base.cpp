// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/WarriorProjectile_Base.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "FunctionLibrary.h"
#include "Tags_My.h"
#include "AbilitySystemBlueprintLibrary.h"

#include "DebugHelpper.h"

AWarriorProjectile_Base::AWarriorProjectile_Base()
{
	PrimaryActorTick.bCanEverTick = false;

	//构造碰撞组件，碰撞策略和碰撞回调函数
	ProjectileCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ProjectileCollisionBox"));
	SetRootComponent(ProjectileCollisionBox);
	ProjectileCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ProjectileCollisionBox->SetCollisionResponseToChannel(ECC_Pawn,ECR_Block);
	ProjectileCollisionBox->SetCollisionResponseToChannel(ECC_WorldDynamic,ECR_Block);
	ProjectileCollisionBox->SetCollisionResponseToChannel(ECC_WorldStatic,ECR_Block);
	ProjectileCollisionBox->OnComponentHit.AddUniqueDynamic(this, &ThisClass::OnProjectileHit);
	ProjectileCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnProjectileBeginOverlap);
	//构建粒子组件
	ProjectileNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ProjectileNiagaraComponent"));
	ProjectileNiagaraComponent->SetupAttachment(GetRootComponent());
	//投掷物运动组件
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->InitialSpeed = 700.f;
	ProjectileMovementComponent->MaxSpeed = 900.f;
	ProjectileMovementComponent->Velocity = FVector(1.f,0.f,0.f);
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;
	//生命周期
	InitialLifeSpan = 4.f;

}

void AWarriorProjectile_Base::BeginPlay()
{
	Super::BeginPlay();
	//根据伤害策略改变对Pawn的碰撞方式
	if(ProjectileDamagePolicy == EProjectileDamagePolicy::OnBeginOverlap)
	{
		ProjectileCollisionBox->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);
	}
	
}

void AWarriorProjectile_Base::OnProjectileHit(UPrimitiveComponent *HitComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit)
{
	BP_OnProjectileHitFX(Hit.ImpactPoint);
	//获取碰撞者
	APawn* HitPawn = Cast<APawn>(OtherActor);
	//处理碰到的不是敌人的情况
	if(!HitPawn || !UFunctionLibrary::IsTargetPawnHostile(GetInstigator(),HitPawn))
	{
		//直接销毁然后返回
		Destroy();
		return;
	}
	//碰到的是敌人的情况

	//处理格挡情况
	bool IsValidBlock = false;
	const bool IsPlayerBlocking = UFunctionLibrary::NativeDoesActorHaveTag(HitPawn,MyGameplayTags::Player_Status_Blocking);
	//如果HitPawn正在格挡的话检测格挡是否有效
	if(IsPlayerBlocking)
	{
		IsValidBlock = UFunctionLibrary::IsBlockValid(this,HitPawn);
	}
	//HitPawn如果格挡有效
	FGameplayEventData EventData;
	EventData.Instigator = this;
	EventData.Target = HitPawn;
	if(IsValidBlock)
	{
		//发送格挡成功事件通知给HitPawn
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			HitPawn,
			MyGameplayTags::Player_Event_SuccessfulBlock,
			EventData
		);
	}
	//如果格挡无效
	else
	{
		HandleApplyProjectileDamage(HitPawn,EventData);

	}
	//无论怎么样碰到了敌人都应该销毁
	Destroy();
}

void AWarriorProjectile_Base::OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}
//应用伤害效果
void AWarriorProjectile_Base::HandleApplyProjectileDamage(APawn* HitPawn,const FGameplayEventData& InPayload)
{
	checkf(ProjectileDamageEffectSpecHandle.IsValid(),TEXT("Forget assign EffectSpecHandle"));
	//应用伤害效果
	const bool bWasApplied = UFunctionLibrary::ApplyGameplayEffectSpacHandleToTarget(GetInstigator(),HitPawn,ProjectileDamageEffectSpecHandle);
	//如果成功应用了伤害效果
	if(bWasApplied)
	{
		//向目标发送伤害React事件
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			HitPawn,
			MyGameplayTags::Shared_Event_HitReact,
			InPayload
		);
	}
}
