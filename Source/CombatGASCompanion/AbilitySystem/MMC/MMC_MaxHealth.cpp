// Copyright SuspiciousDinosaur @ Youtube


#include "MMC_MaxHealth.h"

#include "CombatGASCompanion/AbilitySystem/CombatAttributeSet.h"
#include "CombatGASCompanion/Interfaces/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	ToughnessDefinition.AttributeToCapture = UCombatAttributeSet::GetToughnessAttribute();
	ToughnessDefinition.AttributeSource= EGameplayEffectAttributeCaptureSource::Target;
	ToughnessDefinition.bSnapshot = false;

	RelevantAttributesToCapture.Add(ToughnessDefinition);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	//gather tags from Source and Target

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;

	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;
	float Toughness = 0.f;
	
	GetCapturedAttributeMagnitude(ToughnessDefinition,Spec,EvaluateParameters,Toughness);
	Toughness = FMath::Max<float>(Toughness,0.f);

	ICombatInterface*CombatInterface =Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 PlayerLevel = CombatInterface->GetPlayerLevel();

	
	//DO CUSTOM CALCULATION TO RETURN FLOAT
	return 100.f + (PlayerLevel*50.f)+(Toughness*22.f);
}


