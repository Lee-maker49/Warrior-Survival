// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "DA/Input/DA_InputConfig.h"
#include "EIC_Hero.generated.h"

/**
 * 这个类为角色输入绑定提供方法
 */
UCLASS()
class GASPRE_API UEIC_Hero : public UEnhancedInputComponent
{
	GENERATED_BODY()
	
public:
	template<class UserObject,typename CallbackFunc>
	void BindInputActions(const UDA_InputConfig* InputConfig,const FGameplayTag& InputTag,ETriggerEvent Event,UserObject* ContextObject,CallbackFunc Funcname);

	template<class UserObject,typename CallbackFunc>
	void BindAbilityInputAction(const UDA_InputConfig* InputConfig,UserObject* ContextObject,CallbackFunc PressedFuncname,CallbackFunc ReleasedFuncname);
};
//提供一个Tag就可以绑定到InputConfig中的对应的Action
template <class UserObject, typename CallbackFunc>
inline void UEIC_Hero::BindInputActions(const UDA_InputConfig* InputConfig, const FGameplayTag &InputTag, ETriggerEvent Event, UserObject* ContextObject, CallbackFunc Funcname)
{
	checkf(InputConfig,TEXT("not find InputConfig"));

	UInputAction* InputAction = InputConfig->FindInputActionByTag(InputTag);
	if(InputAction)
	{
		BindAction(InputAction,Event,ContextObject,Funcname);
	}
}

//为每一个需要输入激活的GA绑定事件
template <class UserObject, typename CallbackFunc>
inline void UEIC_Hero::BindAbilityInputAction(const UDA_InputConfig *InputConfig, UserObject *ContextObject, CallbackFunc PressedFuncname, CallbackFunc ReleasedFuncname)
{
	checkf(InputConfig,TEXT("not find InputConfig"));

	for(const FInputDataStruct &Data : InputConfig->AbilityInputStructAbilityActions)
	{
		if(!Data.IsValid())
			continue;

		BindAction(Data.InputAction,ETriggerEvent::Started,ContextObject,PressedFuncname,Data.InputTag);
		BindAction(Data.InputAction,ETriggerEvent::Completed,ContextObject,ReleasedFuncname,Data.InputTag);
	}
}
