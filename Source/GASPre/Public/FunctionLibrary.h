// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Types/EnumTypes.h"

#include "FunctionLibrary.generated.h"

class UAbilitySystemComponent_Base;
class UPawnCombatComponent;
class UWarriorGameInstance;

/**
 * 
 */
UCLASS()
class GASPRE_API UFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	static UAbilitySystemComponent_Base* NativeGetASCFromActor(AActor* InActor);

	UFUNCTION(BlueprintCallable,Category = "MyFunctionLibrary")
	static void AddGameplayTagIfNone(AActor* InActor,FGameplayTag TagToAdd);
	UFUNCTION(BlueprintCallable,Category = "MyFunctionLibrary")
	static void RemoveGameplayTagIfFind(AActor* InActor , FGameplayTag TagToRemove);

	static bool NativeDoesActorHaveTag(AActor* InActor,FGameplayTag TagToCheck);

	UFUNCTION(BlueprintCallable,Category = "MyFunctionLibrary",meta = (DisplayName = "Does Actor Have Name",ExpandEnumAsExecs = "OutConfirmType"))
	static void BP_DoesActorHaveTag(AActor* InActor,FGameplayTag TagToCheck,EConfirmType &OutConfirmType);

	static UPawnCombatComponent* NativeGetPawnCombatComponentFromActor(AActor* InActor);

	UFUNCTION(BlueprintCallable,Category = "MyFunctionLibrary",meta = (DisplayName = "Get PawnCombatComponent From Actor",ExpandEnumAsExecs = "OutValidType"))
	static UPawnCombatComponent* BP_GetPawnCombatComponentFromActor(AActor* InActor,EValidType &OutValidType);

	UFUNCTION(BlueprintPure,Category = "MyFunctionLibrary")
	static bool IsTargetPawnHostile(APawn* QuaryPawn,APawn* TargetPawn);

	static EHitDirection NativeGetHitDirection(AActor* Attacker,AActor* Victim);

	UFUNCTION(BlueprintPure,Category = "MyFunctionLibrary",meta = (DisplayName = "Get Hit Direction"))
	static EHitDirection BP_GetHitDirection(AActor* Attacker,AActor* Victim);

	UFUNCTION(BlueprintPure,Category = "MyFunctionLibrary")
	static bool IsBlockValid(AActor* Attacker,AActor* Victim);

	UFUNCTION(BlueprintCallable, Category = "MyFunctionLibrary", meta = (Latent, LatentInfo = "LatentInfo", WorldContext = "WorldContextObject", ExpandEnumAsExecs = "OutCompleteType", DisplayName = "Count Down Timer"))
	static void CountDownTimer(const UObject* WorldContextObject, float Time, struct FLatentActionInfo LatentInfo, ECountDownCompleteType& OutCompleteType, float& OutRemainingTime);
	//获取游戏实例
	UFUNCTION(BlueprintPure,Category = "MyFunctionLibrary",meta = (WorldContext = "WorldContextObject"))
	static UWarriorGameInstance* GetWarriorGameInstance(const UObject* WorldContextObject);
	//切换输入模式
	UFUNCTION(BlueprintCallable,Category = "MyFunctionLibrary",meta = (WorldContext = "WorldContextObject"))
	static void ToggleInputMode(const UObject* WorldContextObject,EGameModeInput InInputMode);
	//保存当前游戏难度
	UFUNCTION(BlueprintCallable,Category = "MyFunctionLibrary")
	static void SaveCurrentGameDifficuty(ESurvivalGameDifficuty InGameDifficuty);
	//加载保存的游戏难度
	UFUNCTION(BlueprintCallable,Category = "MyFunctionLibrary")
	static bool LoadSavedGameDifficuty(ESurvivalGameDifficuty &OutGameDifficuty);
	//倒计时函数
	UFUNCTION(BlueprintCallable,Category = "MyFunctionLibrary",meta = (Latent, WorldContext = "WorldContextObject",LatentInfo = "LatentInfo",ExpandEnumAsExecs = "CountDownInput|CountDownOutput",TotalTime = "1.0",UpdateInterval = "0.1"))
	static void CountDown(const UObject* WorldContextObject,float TotalTime,float UpdateInterval,float& OutRemainingTime,
		EWarriorCountDownActionInput CountDownInput,
		UPARAM(DisplayName = "Output") EWarriorCountDownActionOutput& CountDownOutput,
		FLatentActionInfo LatentInfo);
	//向目标应用GameplayEffect
	UFUNCTION(BlueprintCallable,Category = "MyFunctionLibrary")
	static bool ApplyGameplayEffectSpacHandleToTarget(AActor* SourceActor,AActor* TargetActor,const FGameplayEffectSpecHandle& InSpecHandle);
	

};
