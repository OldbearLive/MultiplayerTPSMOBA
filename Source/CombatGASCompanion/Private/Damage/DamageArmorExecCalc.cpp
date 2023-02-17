// Fill out your copyright notice in the Description page of Project Settings.


#include "Damage/DamageArmorExecCalc.h"
#include "AttributeSets/CombatBaseAttributeSet.h"

//Defining the relevant Attribute Captures for calculation
struct DamageCapture
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health)
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor)

	DamageCapture()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCombatBaseAttributeSet, Health, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCombatBaseAttributeSet, Armor, Target, false)
	}
};
//HelperFunction to Get Damage Capture Attributes
static DamageCapture& GetDamageCapture()
{
	static DamageCapture DamageCaptureVar;
	return DamageCaptureVar;
}
//Relevant Attributes to Capture for this ExecCalculation
UDamageArmorExecCalc::UDamageArmorExecCalc()
{
	RelevantAttributesToCapture.Add(GetDamageCapture().ArmorDef);
	RelevantAttributesToCapture.Add(GetDamageCapture().HealthDef);
	ValidTransientAggregatorIdentifiers.AddTag(FGameplayTag::RequestGameplayTag("Effects.Calculations.DamageCalc"));
}

void UDamageArmorExecCalc::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
//Get References to Target and Source Ability System Component 
	UAbilitySystemComponent* TargetASC= ExecutionParams.GetTargetAbilitySystemComponent();
	AActor*TargetActor = TargetASC ? TargetASC->GetAvatarActor():nullptr;
	UAbilitySystemComponent* SourceASC= ExecutionParams.GetSourceAbilitySystemComponent();
	AActor*SourceActor = SourceASC ? SourceASC->GetAvatarActor():nullptr;
	
//Get the Owning Gameplay Effect which uses this Calculation class to use it's Variables and Tags
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	const FGameplayTagContainer*SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer*TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;
	
//Capture Weapon Damage
	float BaseDamage = FMath::Max<float>
	(Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage.DamageMagnitude")),false,-1.0f),0.0f);
//Capture Target Armor
	float Armor = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageCapture().ArmorDef,EvaluationParameters,Armor);
//Capture Target Health
	float Health = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageCapture().HealthDef,EvaluationParameters,Health);
//Output Parameters for Calculation and Output

	float OutHealth=0.0f;
	float OutArmor=0.0f;

	OutArmor = Armor-BaseDamage;
	if(OutArmor<0.0f)
	{
		OutHealth= Health-fabs(OutArmor);
		OutArmor=0.0f;

		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageCapture().HealthProperty,EGameplayModOp::Override,OutHealth));
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageCapture().ArmorProperty,EGameplayModOp::Override,OutArmor));
	}
	else
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageCapture().ArmorProperty,EGameplayModOp::Override,OutArmor));		
	}
	
}
