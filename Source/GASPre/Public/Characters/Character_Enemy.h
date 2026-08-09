// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Character_Base.h"
#include "Character_Enemy.generated.h"

class UEnemyCombatComponent;
class UEnemyUIComponent;
class UWidgetComponent;
/**
 * 
 */
UCLASS()
class GASPRE_API ACharacter_Enemy : public ACharacter_Base
{
	GENERATED_BODY()

public:
	//构造函数
	ACharacter_Enemy();

	//~ Begin IInterface_PawnCombatComponent Interface.
	virtual UPawnCombatComponent* GetPawnCombatComponent() const override;
	//~ End IInterface_PawnCombatComponent Interface

	//~ Begin IInterface_PawnUI Interface.
	virtual UPawnUIComponent* GetPawnUIComponent() const override;
	virtual UEnemyUIComponent* GetEnemyUIComponent() const override;
	//~ End IInterface_PawnUI Interface
	
protected:
	virtual void BeginPlay() override;

	//~ Begin APawn Interface.
	//控制到角色的时候触发  加载StartUpData
	virtual void PossessedBy(AController* NewController) override;
	//~ End APawn Interface

	//管理武器的组件
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Combat")
	UEnemyCombatComponent* EnemyCombatComponent;

	//给UI的属性广播组件
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "UI")
	UEnemyUIComponent* EnemyUIComponent;
	//UI组件
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "UI")
	UWidgetComponent* EnemyHealthWidgetComponent;

private:
	//初始化敌人能力数据
	void InitEnemyStartUpData();

public:
	FORCEINLINE UEnemyCombatComponent* GetEnemyCombatComponent() const {return EnemyCombatComponent;}
};
