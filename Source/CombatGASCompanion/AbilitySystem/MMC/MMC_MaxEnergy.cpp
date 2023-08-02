// Copyright SuspiciousDinosaur @ Youtube


#include "MMC_MaxEnergy.h"

#include "CombatGASCompanion/AbilitySystem/CombatAttributeSet.h"
#include "CombatGASCompanion/Interfaces/CombatInterface.h"


UMMC_MaxEnergy::UMMC_MaxEnergy()
{
	TechniqueDefinition.AttributeToCapture = UCombatAttributeSet::GetTechniqueAttribute();
	TechniqueDefinition.AttributeSource= EGameplayEffectAttributeCaptureSource::Target;
	TechniqueDefinition.bSnapshot = false;

	RelevantAttributesToCapture.Add(TechniqueDefinition);
}

float UMMC_MaxEnergy::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	//gather tags from Source and Target

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;

	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;
	float Technique = 0.f;
	
	GetCapturedAttributeMagnitude(TechniqueDefinition,Spec,EvaluateParameters,Technique);
	Technique = FMath::Max<float>(Technique,0.f);

	ICombatInterface*CombatInterface =Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 PlayerLevel = CombatInterface->GetPlayerLevel();

	
	//DO CUSTOM CALCULATION TO RETURN FLOAT
	return 100.f + (PlayerLevel*25.f)+(Technique*11.f);
}
