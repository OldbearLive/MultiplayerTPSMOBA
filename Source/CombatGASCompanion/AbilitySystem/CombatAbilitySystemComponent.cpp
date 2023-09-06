// Copyright SuspiciousDinosaur @ Youtube


#include "CombatAbilitySystemComponent.h"

#include "Abilities/CombatGameplayAbility.h"
#include "CombatGASCompanion/CombatGameplayTagsSingleton.h"

void UCombatAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UCombatAbilitySystemComponent::ClientEffectsApplied);
}

void UCombatAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilites)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilites)
	{
		FGameplayAbilitySpec GameplayAbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		if (const UCombatGameplayAbility* CombatAbility = Cast<UCombatGameplayAbility>(GameplayAbilitySpec.Ability))
		{
			GameplayAbilitySpec.DynamicAbilityTags.AddTag(CombatAbility->StartupInputTag);
			GiveAbility(GameplayAbilitySpec);
		}
	}
}

void UCombatAbilitySystemComponent::AddWeaponEquipAbilities(
	const TArray<TSubclassOf<UGameplayAbility>>& StartupWeaponEquipAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupWeaponEquipAbilities)
	{
		FGameplayAbilitySpec GameplayAbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		if (const UCombatGameplayAbility* CombatAbility = Cast<UCombatGameplayAbility>(GameplayAbilitySpec.Ability))
		{
			GiveAbility(GameplayAbilitySpec);
		}
	}
}

void UCombatAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())return;

	for (auto& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void UCombatAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())return;

	for (auto& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(AbilitySpec);
		}
	}
}

void UCombatAbilitySystemComponent::ClientEffectsApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
                                                   const FGameplayEffectSpec& EffectSpec,
                                                   FActiveGameplayEffectHandle ActiveGameplayEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);

	EffectAssetTags.Broadcast(TagContainer);
}
