// Copyright SuspiciousDinosaur @ Youtube


#include "CombatDamageExecCalc.h"

#include "AbilitySystemComponent.h"
#include "CombatGASCompanion/CombatGameplayTagsSingleton.h"
#include "CombatGASCompanion/AbilitySystem/CombatAttributeSet.h"
#include "CombatGASCompanion/AbilitySystem/CombatBlueprintFunctionLibrary.h"

struct CombatDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Energy)
	DECLARE_ATTRIBUTE_CAPTUREDEF(EnergyEfficiency)
	DECLARE_ATTRIBUTE_CAPTUREDEF(EnergyPenetration)
	DECLARE_ATTRIBUTE_CAPTUREDEF(OverloadChance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(OverloadDamage)
	DECLARE_ATTRIBUTE_CAPTUREDEF(EnergyResistance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance)

	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;

	CombatDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCombatAttributeSet, Energy, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCombatAttributeSet, EnergyEfficiency, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCombatAttributeSet, EnergyPenetration, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCombatAttributeSet, OverloadChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCombatAttributeSet, OverloadDamage, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCombatAttributeSet, EnergyResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCombatAttributeSet, PhysicalResistance, Target, false);

		//Mapping CaptureDefinitions to GameplayTags
		const FCombatGameplayTags& Tags = FCombatGameplayTags::Get();
		TagsToCaptureDefs.Add(Tags.Attributes_Vital_Energy, EnergyDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_EnergyEfficiency, EnergyEfficiencyDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_EnergyPenetration, EnergyPenetrationDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_OverloadChance, OverloadChanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_OverloadDamage, OverloadDamageDef);
		TagsToCaptureDefs.Add(Tags.Attrributes_Resistance_Energy, EnergyResistanceDef);
		TagsToCaptureDefs.Add(Tags.Attrributes_Resistance_Physical, PhysicalResistanceDef);
	}
};

static const CombatDamageStatics& GetCombatDamageStatics()
{
	static CombatDamageStatics DamageStatics;
	return DamageStatics;
}

UCombatDamageExecCalc::UCombatDamageExecCalc()
{
	RelevantAttributesToCapture.Add(GetCombatDamageStatics().EnergyEfficiencyDef);
	RelevantAttributesToCapture.Add(GetCombatDamageStatics().EnergyPenetrationDef);
	RelevantAttributesToCapture.Add(GetCombatDamageStatics().EnergyDef);
	RelevantAttributesToCapture.Add(GetCombatDamageStatics().OverloadChanceDef);
	RelevantAttributesToCapture.Add(GetCombatDamageStatics().OverloadDamageDef);
	RelevantAttributesToCapture.Add(GetCombatDamageStatics().EnergyResistanceDef);
	RelevantAttributesToCapture.Add(GetCombatDamageStatics().PhysicalResistanceDef);
}

void UCombatDamageExecCalc::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                                   FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	const AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	const AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParams;
	EvaluationParams.SourceTags = SourceTags;
	EvaluationParams.TargetTags = TargetTags;

	//Get Context from ContextHandle from Effect Spec
	FGameplayEffectContextHandle LocalEffectContextHandle = Spec.GetContext();


	//Get SetByCaller Damage Magnitude

	float Damage = 0.f;

	//Get Damage Aggregation from DamageType Tag Magnitudes
	for (auto Pair : FCombatGameplayTags::Get().DamageTypesToResistances)
	{
		const FGameplayTag DamageTypeTag = Pair.Key;
		const FGameplayTag DamageResistanceTag = Pair.Value;
		const FGameplayEffectAttributeCaptureDefinition CaptureDef = CombatDamageStatics().TagsToCaptureDefs[
			DamageResistanceTag];

		float DamageTypeValue = Spec.GetSetByCallerMagnitude(DamageTypeTag);

		float DamageTypeResistance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDef, EvaluationParams, DamageTypeResistance);
		DamageTypeResistance = FMath::Max(0.f, DamageTypeResistance);

		DamageTypeValue = DamageTypeValue - DamageTypeResistance;
		if (DamageTypeValue > 0.f)
		{
			Damage += DamageTypeValue;
		}
	}

	//GetEnergy on target 
	float Energy = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetCombatDamageStatics().EnergyDef, EvaluationParams,
	                                                           Energy);
	Energy = FMath::Max(0.f, Energy);

	//GetOverloadChance on Source(Instigator of this effect)
	float SourceOverloadChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetCombatDamageStatics().OverloadChanceDef,
	                                                           EvaluationParams,
	                                                           SourceOverloadChance);
	SourceOverloadChance = FMath::Max(0.f, SourceOverloadChance);

	//GetOverloadDamage on Source(Instigator of this effect)
	float SourceOverloadDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetCombatDamageStatics().OverloadDamageDef,
	                                                           EvaluationParams,
	                                                           SourceOverloadDamage);
	SourceOverloadDamage = FMath::Max(0.f, SourceOverloadDamage);

	//GetEnergyEffeciency on target 

	float TargetEnergyEfficiency = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetCombatDamageStatics().EnergyEfficiencyDef,
	                                                           EvaluationParams, TargetEnergyEfficiency);
	TargetEnergyEfficiency = FMath::Max(0.f, TargetEnergyEfficiency);

	const bool bOverloadHit = FMath::RandRange(1, 100) < SourceOverloadChance;

	//SetDamage to be Critical based on bOverloadHit
	Damage = bOverloadHit ? 1.2f * Damage + SourceOverloadDamage : Damage;

	UCombatBlueprintFunctionLibrary::SetIsOverloadHit(LocalEffectContextHandle, bOverloadHit);

	//CalculateDamage done to Energy, and Health if Energy is zero.

	float EnergyDamage = 0.f;
	const bool bIsShieldHit = Energy > 0;
	if (bIsShieldHit)
	{
		UCombatBlueprintFunctionLibrary::SetIsShieldHit(LocalEffectContextHandle, bIsShieldHit);
		//IsEnergyDamage
		Damage = FMath::Max(1, Damage - TargetEnergyEfficiency*0.25f);
		EnergyDamage = Damage;
		FGameplayModifierEvaluatedData EvaluatedData(UCombatAttributeSet::GetIncomingDamageAttribute(),
		                                             EGameplayModOp::Additive, Damage);
		FGameplayModifierEvaluatedData EnergyData(GetCombatDamageStatics().EnergyProperty,
		                                          EGameplayModOp::Additive, -EnergyDamage);
		//Execute Damage on Target's Energy
		OutExecutionOutput.AddOutputModifier(EvaluatedData);
		//Execute Remaining Health Damage if Shield hit 0 with the delta amount
		if ((Energy - Damage) < 0.f)
		{
			//Only if Energy is 0
			Damage = FMath::Abs(Energy - Damage);
		}
		Damage = 0.f;
	}

	//Execute Damage on Enemy (Will be 0 if Energy - Damage >0)
	FGameplayModifierEvaluatedData EvaluatedData(UCombatAttributeSet::GetIncomingDamageAttribute(),
	                                             EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
