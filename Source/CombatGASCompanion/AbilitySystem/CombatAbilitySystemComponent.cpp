// Copyright SuspiciousDinosaur @ Youtube


#include "CombatAbilitySystemComponent.h"

void UCombatAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this,&UCombatAbilitySystemComponent::EffectsApplied);
}

void UCombatAbilitySystemComponent::EffectsApplied(UAbilitySystemComponent* AbilitySystemComponent,
                                                   const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle)
{
	GEngine->AddOnScreenDebugMessage(1,7.f,FColor::Blue,FString("EffectsApplied!"));
}
