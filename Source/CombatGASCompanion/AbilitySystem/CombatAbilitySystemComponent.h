// Copyright SuspiciousDinosaur @ Youtube

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "CombatAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer&/*AssetTags*/);

DECLARE_MULTICAST_DELEGATE_OneParam(FAbilitiesGiven, UCombatAbilitySystemComponent*);

DECLARE_DELEGATE_OneParam(FForEachAbility, const FGameplayAbilitySpec&);

/**
 * 
 */
UCLASS()
class COMBATGASCOMPANION_API UCombatAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void AbilityActorInfoSet();

	FEffectAssetTags EffectAssetTags;

	FAbilitiesGiven AbilitiesGiven;

	UFUNCTION(BlueprintCallable, Category="Character Abilities")
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilites);

	bool bStartupAbilitiesGiven = false;

	UFUNCTION(BlueprintCallable, Category="Character Abilities")
	void AddWeaponEquipAbilities(const TSubclassOf<UGameplayAbility>& StartupWeaponEquipAbilities);

	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	void ForEachAbility(const FForEachAbility& Delegate);

	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);

	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);

	
	virtual void OnRep_ActivateAbilities() override;

protected:
	UFUNCTION(Client, Reliable)
	void ClientEffectsApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec,
	                          FActiveGameplayEffectHandle ActiveGameplayEffectHandle);
};
