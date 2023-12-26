// Copyright SuspiciousDinosaur @ Youtube

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "CombatAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

	FEffectProperties(){}

	FGameplayEffectContextHandle EffectContextHandle;

	//Source Variables
	UPROPERTY()
	UAbilitySystemComponent* SourceASC = nullptr;
	UPROPERTY()
	AActor* SourceAvatarActor = nullptr;
	UPROPERTY()
	AController* SourceController = nullptr;
	UPROPERTY()
	ACharacter* SourceCharacter = nullptr;


	//Target Variables
	UPROPERTY()
	UAbilitySystemComponent* TargetASC = nullptr;
	UPROPERTY()
	AActor* TargetAvatarActor = nullptr;
	UPROPERTY()
	AController* TargetController = nullptr;
	UPROPERTY()
	ACharacter* TargetCharacter = nullptr;
	
};

template<class T>
using TStaticFunctionPtr = typename TBaseStaticDelegateInstance<T,FDefaultDelegateUserPolicy>::FFuncPtr;

/**
 * 
 */
UCLASS()
class COMBATGASCOMPANION_API UCombatAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UCombatAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;


	TMap<FGameplayTag,TStaticFunctionPtr<FGameplayAttribute()>> TagsToAttributes;
	
	/*
    	*Primary Attributes
    	*/
    
    	// Determines your ability to equip certain weapons and items and Deals with energy related actions
    	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Technique, Category = "Primary Attributes")
    	FGameplayAttributeData Technique;
    	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, Technique);
    
    	// The Sheer Force of Will. Increases and decreases Health Regeneration and Critical Chance. is a Dynamic Force 
    	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Spirit, Category = "Primary Attributes")
    	FGameplayAttributeData Spirit;
    	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, Spirit);
    
    	// Your Toughness Determines how much Health and Damage you do in melee combat.
    	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Toughness, Category = "Primary Attributes")
    	FGameplayAttributeData Toughness;
    	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, Toughness);

	/*
	*Secondary Attributes
	*/

	// Scales by Technique
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxEnergy, Category = "Secondary Attributes")
	FGameplayAttributeData MaxEnergy;
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, MaxEnergy);

	// Scales by Toughness
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Secondary Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, MaxHealth);
	
	// Determines Overall Energy efficiency used to dissipate damage when Shields are hit. Scaled by Technique
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_EnergyEfficiency, Category = "Secondary Attributes")
	FGameplayAttributeData EnergyEfficiency;
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, EnergyEfficiency);

	// Scales with Spirit. Determines Overall Energy Ignored when Shields are hit. Increases OverloadChance 
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_EnergyPenetration, Category = "Secondary Attributes")
	FGameplayAttributeData EnergyPenetration;
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, EnergyPenetration);

	// Increases Chances of Dealing Overload Damage. Scales with Energy Penetration
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_OverloadChance, Category = "Secondary Attributes")
	FGameplayAttributeData OverloadChance;
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, OverloadChance);

	// Bonus Damage when Overload happens. Scales with MaxEnergy
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_OverloadDamage, Category = "Secondary Attributes")
	FGameplayAttributeData OverloadDamage;
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, OverloadDamage);

	// Scales with Toughness and Spirit
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthRegeneration, Category = "Secondary Attributes")
	FGameplayAttributeData HealthRegeneration;
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, HealthRegeneration);

	// Scales with Technique and Energy Efficiency
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_EnergyRegeneration, Category = "Secondary Attributes")
	FGameplayAttributeData EnergyRegeneration;
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, EnergyRegeneration);

	/*
	*Resistance Attributes
	*/

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_EnergyResistance, Category = "Resistance Attributes")
	FGameplayAttributeData EnergyResistance;
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, EnergyResistance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_PhysicalResistance, Category = "Resistance Attributes")
	FGameplayAttributeData PhysicalResistance;
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, PhysicalResistance);
	

	
	/*
	*Battery Attributes
	*/
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Battery, Category = "Battery Attributes")
	FGameplayAttributeData Battery;
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, Battery);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxBattery, Category = "Battery Attributes")
	FGameplayAttributeData MaxBattery;
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, MaxBattery);
	
	

	/*
	*Vital Attributes
	*/
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Vital Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, Health);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Energy, Category = "Vital Attributes")
	FGameplayAttributeData Energy;
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet, Energy);


	
	/*
	 *Meta Attributes
	 */
	UPROPERTY(BlueprintReadOnly, Category = "Meta Attributes")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet,IncomingDamage);

	UPROPERTY(BlueprintReadOnly, Category = "Meta Attributes")
	FGameplayAttributeData IncomingXP;
	ATTRIBUTE_ACCESSORS(UCombatAttributeSet,IncomingXP);

	

	

private:


	/*
	 *FUNTION TEMPLATE FOR REPNOTIFY ATTRIBUTE
	 *
	 *UFUNCTION()
	 *void OnRep_Stat(const FGameplayAttributeData OldStat) const;
	 * 
	 */
	
	/*
	*Vital Attributes
	*/
	
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData OldHealth) const;
	
	UFUNCTION()
	void OnRep_Energy(const FGameplayAttributeData OldEnergy) const;

	

	/*
	*Battery Attributes
	*/
	UFUNCTION()
	void OnRep_Battery(const FGameplayAttributeData OldBattery) const;

	UFUNCTION()
	void OnRep_MaxBattery(const FGameplayAttributeData OldMaxBattery) const;

	/*
	*Stat Attributes
	*/
	
	UFUNCTION()
	void OnRep_Toughness(const FGameplayAttributeData OldToughness) const;

	UFUNCTION()
	void OnRep_Technique(const FGameplayAttributeData OldTechnique) const;

	UFUNCTION()
	void OnRep_Spirit(const FGameplayAttributeData OldSpirit) const;


	
	
	/*
	*Secondary Attributes
	*/

	UFUNCTION()
	void OnRep_MaxEnergy(const FGameplayAttributeData OldMaxEnergy) const;

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData OldMaxHealth) const;

	UFUNCTION()
	void OnRep_EnergyEfficiency(const FGameplayAttributeData OldEnergyEfficiency) const;

	UFUNCTION()
	void OnRep_EnergyPenetration(const FGameplayAttributeData OldEnergyPenetration) const;

	UFUNCTION()
	void OnRep_OverloadChance(const FGameplayAttributeData OldOverloadChance) const;

	UFUNCTION()
	void OnRep_OverloadDamage(const FGameplayAttributeData OldOverloadDamage) const;

	UFUNCTION()
	void OnRep_HealthRegeneration(const FGameplayAttributeData OldHealthRegeneration) const;

	UFUNCTION()
	void OnRep_EnergyRegeneration(const FGameplayAttributeData OldEnergyRegeneration) const;

	UFUNCTION()
	void OnRep_EnergyResistance(const FGameplayAttributeData OldEnergyResistance) const;

	UFUNCTION()
	void OnRep_PhysicalResistance(const FGameplayAttributeData OldPhysicalResistance) const;

private:

	void SetEffectProperties(const FGameplayEffectModCallbackData& Data,  FEffectProperties& Props);

public:
	
	void ShowFloatingText(const FEffectProperties& Properties,float Damage,bool bIsShieldHit,bool IsOverloadHit) const;
};
