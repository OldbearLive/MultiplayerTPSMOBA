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
	GameplayTags.InputTag_Jump = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Jump"),
		FString(
			"InputTag for Jump"));
	GameplayTags.InputTag_Jet = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Jet"),
		FString(
			"InputTag for Jet"));
	GameplayTags.InputTag_Dash = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Dash"),
		FString(
			"InputTag for Dash"));

	//HIT REACT AND DEATH ABILITY


	GameplayTags.HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effects.HitReact"),
		FString(
			"HitReact"));

	GameplayTags.Death = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Event.DeathEvent"),
		FString(
			"DeathTag"));

	//DAMAGE TYPES
	
	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Magnitude"),
		FString(
			"Set by Called Damage"));

	GameplayTags.Damage_PhysicalDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Damage.Physical"),
			FString(
				"Physical Damage Property"));

	GameplayTags.Damage_EnergyDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Energy"),
		FString(
			"Energy Damage Property"));

	//DAMAGE RESISTANCES

	GameplayTags.Attrributes_Resistance_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attrributes.Resistance.Physical"),
		FString(
			"Physical Resistance"));
	GameplayTags.Attrributes_Resistance_Energy = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attrributes.Resistance.Energy"),
		FString(
			"Energy Resistance"));

	//Ability Tags

	GameplayTags.Abilities_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Attack"),
		FString(
			"Attack Ability Tag"));

	//Meta Tags
	GameplayTags.Attributes_Meta_IncomingXP = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Attributes.Meta.IncomingXP"),
	FString(
		"Attack Ability Tag"));
	//MAP OF DAMAGETYPES TO RESISTANCES

	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_PhysicalDamage,GameplayTags.Attrributes_Resistance_Physical);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_EnergyDamage,GameplayTags.Attrributes_Resistance_Energy);

	
}
