// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_Base.generated.h"

class UHeroUIComponent;
class UEnemyUIComponent;
/**
 * 
 */
UCLASS()
class GASPRE_API UWidget_Base : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	//BlueprintImplementableEvent意思时必须在蓝图中重写的函数
	//组件刚获取HeroUIComponent时的初始化
	UFUNCTION(BlueprintImplementableEvent,meta = (DisplayName = "On Owning Hero UI Component Initialize"))
	void BP_OnOwningHeroUIComponentInitialize(UHeroUIComponent* OwningHeroUIComponent);
	//组件刚获取EnemyUIComponent时的初始化
	UFUNCTION(BlueprintImplementableEvent,meta = (DisplayName = "On Owning Enemy UI Component Initialize"))
	void BP_OnOwningEnemyUIComponentInitialize(UEnemyUIComponent* OwningEnemyUIComponent);

public: 	 
	UFUNCTION(BlueprintCallable)
	void InitEnemyCreateWidget(AActor* OwningEnemyActor);

};
