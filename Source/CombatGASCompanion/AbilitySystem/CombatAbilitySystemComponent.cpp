// Copyright SuspiciousDinosaur @ Youtube


#include "CombatAbilitySystemComponent.h"

void UCombatAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this,&UCombatAbilitySystemComponent::EffectsApplied);
}

void UCombatAbilitySystemComponent::EffectsApplied(UAbilitySystemComponent* AbilitySystemComponent,
                                                   const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);

	EffectAssetTags.Broadcast(TagContainer);
	
}
