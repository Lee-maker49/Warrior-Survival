// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GEExecCalc/GEExecCalc_DamageTaken.h"
#include "AbilitySystem/AttributeSet_Base.h"
#include "Tags_My.h"

#include "DebugHelpper.h"

//declare a Attribute Capture for Applying Damage
struct FDamageCapture
{
    DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower)
    DECLARE_ATTRIBUTE_CAPTUREDEF(DefensePower)
    DECLARE_ATTRIBUTE_CAPTUREDEF(DamageTaken)

    FDamageCapture()
    {
        DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSet_Base,AttackPower,Source,false)
        DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSet_Base,DefensePower,Target,false)
        DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSet_Base,DamageTaken,Target,false)
    }

};
//instaned a Attributes Capture for applying damage 
static const FDamageCapture& GetDamageCapture()
{
    static FDamageCapture DamageCapture;
    return DamageCapture;
}

UGEExecCalc_DamageTaken::UGEExecCalc_DamageTaken()
{
    //A tarry in the UGEExecCalc_DamageTaken for saving attributes from source and target in order to use them 
    RelevantAttributesToCapture.Add(GetDamageCapture().AttackPowerDef);
    RelevantAttributesToCapture.Add(GetDamageCapture().DefensePowerDef);
    RelevantAttributesToCapture.Add(GetDamageCapture().DamageTakenDef);
}

void UGEExecCalc_DamageTaken::Execute_Implementation(const FGameplayEffectCustomExecutionParameters &ExecutionParams, FGameplayEffectCustomExecutionOutput &OutExecutionOutput) const
{
    //get the effectspec
    const FGameplayEffectSpec &EffectSpec = ExecutionParams.GetOwningSpec();

    //set the params to use in getting count of atrributes
    FAggregatorEvaluateParameters EvaluateParameters;
    EvaluateParameters.SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
    EvaluateParameters.TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags();

    //get the count of the atrributes
    float SourceAttackPower = 0.f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageCapture().AttackPowerDef,EvaluateParameters,SourceAttackPower);
    //Debug::Print(TEXT("SourceAttackPower"),SourceAttackPower);

    float DamageBase = 0.f;
    int32 LightComboCount = 0;
    int32 HeavyComboCount = 0;

    for(const TPair<FGameplayTag,float> &TagMagnitude : EffectSpec.SetByCallerTagMagnitudes)
    {
        if(TagMagnitude.Key.MatchesTagExact(MyGameplayTags::Shared_SetByCaller_BaseDamage))
        {
            DamageBase = TagMagnitude.Value;
            //Debug::Print(TEXT("DamageBase"),DamageBase);
        }
        if(TagMagnitude.Key.MatchesTagExact(MyGameplayTags::Player_SetByCaller_AttackType_Light))
        {
            LightComboCount = TagMagnitude.Value;
            //Debug::Print(TEXT("LightComboCount"),LightComboCount);
        }
        if(TagMagnitude.Key.MatchesTagExact(MyGameplayTags::Player_SetByCaller_AttackType_Heavy))
        {
            HeavyComboCount = TagMagnitude.Value;
            //Debug::Print(TEXT("HeavyComboCount"),HeavyComboCount);
        }
 
    }

    float TargetDefensePower = 0.f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageCapture().DefensePowerDef,EvaluateParameters,TargetDefensePower);
    //Debug::Print(TEXT("TargetDefensePower"),TargetDefensePower);

    //to calculate the final damage
    if(LightComboCount != 0)
    {
        const float DamageIncreasePercentLight = (LightComboCount - 1) * 0.05 + 1.0f;
        DamageBase *= DamageIncreasePercentLight;
        //Debug::Print(TEXT("DamageIncreasePercentLight"),DamageIncreasePercentLight);
    }
    if(HeavyComboCount != 0)
    {
        const float DamageIncreasePercentheavy = HeavyComboCount * 0.15 + 1.0f;
        DamageBase *= DamageIncreasePercentheavy; 
        //Debug::Print(TEXT("DamageIncreasePercentheavy"),DamageIncreasePercentheavy);
    }

    const float FinalDamage = DamageBase * SourceAttackPower / TargetDefensePower;
    //Debug::Print(TEXT("FinalDamage"),FinalDamage);

    //exec to modify the attribute of damage taking
    if(FinalDamage > 0.f)
    {
        OutExecutionOutput.AddOutputModifier(
            FGameplayModifierEvaluatedData(
                GetDamageCapture().DamageTakenProperty,
                EGameplayModOp::Override,
                FinalDamage
            )
        );
    }

}