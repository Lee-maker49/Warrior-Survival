// Fill out your copyright notice in the Description page of Project Settings.


#include "DA/Input/DA_InputConfig.h"

UInputAction* UDA_InputConfig::FindInputActionByTag(const FGameplayTag &InputTag) const
{
    for(const FInputDataStruct& InputStruct : NativeInputStructActions)
    {
        if(InputStruct.InputTag == InputTag && InputStruct.InputAction)
        {
            return InputStruct.InputAction;
        }
    }
    return nullptr;
}