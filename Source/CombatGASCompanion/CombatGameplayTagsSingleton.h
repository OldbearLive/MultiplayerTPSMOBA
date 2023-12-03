// Copyright SuspiciousDinosaur @ Youtube

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"


/*
 * SINGLETON CLASS
 * CONTAINS NATIVE GAMEPLAYTAGS
 */
struct FCombatGameplayTags
{
public:

 static const FCombatGameplayTags& Get(){return GameplayTags;}

 static void InitializeNativeGameplayTags();

 //ATTRIBUTE TAG SECTION

 //PRIMARY
 FGameplayTag Attributes_Primary_Technique;
 FGameplayTag Attributes_Primary_Spirit;
 FGameplayTag Attributes_Primary_Toughness;
 //VITAL
 FGameplayTag Attributes_Vital_Health;
 FGameplayTag Attributes_Vital_Energy;
 //SECONDARY
 FGameplayTag Attributes_Secondary_MaxEnergy;
 FGameplayTag Attributes_Secondary_MaxHealth;
 FGameplayTag Attributes_Secondary_EnergyEfficiency;
 FGameplayTag Attributes_Secondary_EnergyPenetration;
 FGameplayTag Attributes_Secondary_OverloadChance;
 FGameplayTag Attributes_Secondary_OverloadDamage;
 FGameplayTag Attributes_Secondary_HealthRegeneration;
 FGameplayTag Attributes_Secondary_EnergyRegeneration;
 //BATTERY
 FGameplayTag Attributes_Tertiary_Battery;
 FGameplayTag Attributes_Tertiary_MaxBattery;
 
 
//InputTags

 FGameplayTag InputTag_Primary;
 FGameplayTag InputTag_Secondary;
 FGameplayTag InputTag_1;
 FGameplayTag InputTag_2;
 FGameplayTag InputTag_3;
 FGameplayTag InputTag_4;
 FGameplayTag InputTag_Jump;
 FGameplayTag InputTag_Dash;
 FGameplayTag InputTag_Jet;

 //DAMAGE

 FGameplayTag HitReact;
 FGameplayTag Death;
 
 //DAMAGETYPES

 FGameplayTag Damage;
 FGameplayTag Damage_PhysicalDamage;
 FGameplayTag Damage_EnergyDamage;

 //DAMAGE RESISTANCES
 FGameplayTag  Attrributes_Resistance_Physical;
 FGameplayTag  Attrributes_Resistance_Energy;

 TMap<FGameplayTag,FGameplayTag>DamageTypesToResistances;

 //ABILITY TAGS

 FGameplayTag Abilities_Attack;


 
 
 

private:
 static FCombatGameplayTags GameplayTags;
};
