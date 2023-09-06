// Copyright SuspiciousDinosaur @ Youtube


#include "CombatGameplayTagsSingleton.h"
#include "GameplayTagsManager.h"

FCombatGameplayTags FCombatGameplayTags::GameplayTags;

void FCombatGameplayTags::InitializeNativeGameplayTags()
{
	/*PRIMARY ATTRIBUTES*/
	
	GameplayTags.Attributes_Primary_Technique = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Technique"),
		FString(
			"L(Native) Determines your ability to equip certain weapons and items and Deals with energy related actions"));

	GameplayTags.Attributes_Primary_Spirit = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Spirit"),
		FString(
			"L(Native) The Sheer Force of Will. Increases and decreases Health Regeneration and Critical Chance. is a Dynamic Force"));

	GameplayTags.Attributes_Primary_Toughness = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Toughness"),
		FString(
			"L(Native) Your Toughness Determines how much Health and Damage you do in melee combat."));

	/*VITAL ATTRIBUTES*/

	GameplayTags.Attributes_Vital_Health = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Vital.Health"),
		FString(
			"L(Native) Health"));
	GameplayTags.Attributes_Vital_Energy = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Vital.Energy"),
		FString(
			"L(Native) Energy"));

	/*SECONDARY ATTRIBUTES*/
	
	GameplayTags.Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxHealth"),
		FString(
			"L(Native) MAX Health"));
	GameplayTags.Attributes_Secondary_MaxEnergy = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxEnergy"),
		FString(
			"L(Native) MAX Energy"));
	GameplayTags.Attributes_Secondary_EnergyEfficiency = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.EnergyEfficiency"),
		FString(
			"L(Native) Determines Overall Energy efficiency used to dissipate damage when Shields are hit. Scaled by Technique"));
	GameplayTags.Attributes_Secondary_EnergyPenetration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.EnergyPenetration"),
		FString(
			"L(Native) EnergyPenetration"));
	GameplayTags.Attributes_Secondary_OverloadChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.OverloadChance"),
		FString(
			"L(Native) OverloadChance"));
	GameplayTags.Attributes_Secondary_OverloadDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.OverloadDamage"),
		FString(
			"L(Native) OverloadDamage"));
	GameplayTags.Attributes_Secondary_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.HealthRegeneration"),
		FString(
			"L(Native) HealthRegeneration"));
	GameplayTags.Attributes_Secondary_EnergyRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.EnergyRegeneration"),
		FString(
			"L(Native) EnergyRegeneration"));

	/* BATTERY/TERTIARY ATTRIBUTES */
	GameplayTags.Attributes_Tertiary_Battery = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Tertiary.Battery"),
		FString(
			"L(Native) Battery"));
	GameplayTags.Attributes_Tertiary_MaxBattery = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Tertiary.MaxBattery"),
		FString(
			"L(Native) MaxBattery"));

	/* InputTags */
	GameplayTags.InputTag_Primary = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Primary"),
		FString(
			"InputTag for Primary"));
	GameplayTags.InputTag_Secondary = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Secondary"),
		FString(
			"InputTag for Secondary"));
	GameplayTags.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.1"),
		FString(
			"InputTag for 1"));
	GameplayTags.InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.2"),
		FString(
			"InputTag for 2"));
	GameplayTags.InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.3"),
		FString(
			"InputTag for 3"));
	GameplayTags.InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.4"),
		FString(
			"InputTag for 4"));
}
