// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"

#include "WarriorProjectile_Base.generated.h"

class UBoxComponent;
class UNiagaraComponent;
class UProjectileMovementComponent;
struct FGameplayEventData;

UENUM(BlueprintType)
enum class EProjectileDamagePolicy : uint8
{
	OnHit,
	OnBeginOverlap
};

UCLASS()
class GASPRE_API AWarriorProjectile_Base : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWarriorProjectile_Base();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//碰撞体
	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly,Category = "Projectile")
	UBoxComponent* ProjectileCollisionBox;
	//粒子组件
	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly,Category = "Projectile")
	UNiagaraComponent* ProjectileNiagaraComponent;
	//投射物运动组件
	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly,Category = "Projectile")
	UProjectileMovementComponent* ProjectileMovementComponent;
	//伤害策略
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Projectile")
	EProjectileDamagePolicy ProjectileDamagePolicy = EProjectileDamagePolicy::OnHit;
	//伤害效果,这个效果成员变量将会在GA生成这个投掷物的时候构造
	UPROPERTY(BlueprintReadOnly,Category="Projectile",meta = (ExposeOnSpawn = "true"))
	FGameplayEffectSpecHandle ProjectileDamageEffectSpecHandle;
	//碰撞回调函数
	UFUNCTION()
	virtual void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
	virtual void OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	//播放碰撞物撞击音效
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Projectile Hit FX"))
	void BP_OnProjectileHitFX(const FVector& HitLocation);

private:
	//应用伤害效果
	void HandleApplyProjectileDamage(APawn* HitPawn,const FGameplayEventData& InPayload);

};
