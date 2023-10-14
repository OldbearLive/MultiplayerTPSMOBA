// Copyright SuspiciousDinosaur @ Youtube

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "CombatDamageExecCalc.generated.h"

/**
 * 
 */
UCLASS()
class COMBATGASCOMPANION_API UCombatDamageExecCalc : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UCombatDamageExecCalc();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	                                    FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
