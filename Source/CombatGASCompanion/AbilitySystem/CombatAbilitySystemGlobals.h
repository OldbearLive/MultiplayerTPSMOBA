// Copyright SuspiciousDinosaur @ Youtube

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "CombatAbilitySystemGlobals.generated.h"

/**
 * 
 */
UCLASS()
class COMBATGASCOMPANION_API UCombatAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()

	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
	
};
