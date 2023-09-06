// Copyright SuspiciousDinosaur @ Youtube

#pragma once

#include "CoreMinimal.h"
#include "CombatGameplayAbility.h"
#include "CombatProjectileAbility.generated.h"

/**
 * 
 */
UCLASS()
class COMBATGASCOMPANION_API UCombatProjectileAbility : public UCombatGameplayAbility
{
	GENERATED_BODY()

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;

	
};
