// Copyright SuspiciousDinosaur @ Youtube


#include "CombatAbilitySystemGlobals.h"

#include "CombatAbilityTypes.h"

FGameplayEffectContext* UCombatAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FCombatGameplayEffectContext();
}
