// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Attributes/GSCAttributeSetBase.h"
#include "AbilitySystemComponent.h"
#include "CombatBaseAttributeSet.generated.h"

UCLASS()
class COMBATGASCOMPANION_API UCombatBaseAttributeSet : public UGSCAttributeSetBase
{
	GENERATED_BODY()

public:

	// Sets default values for this AttributeSet attributes
	UCombatBaseAttributeSet();

    // AttributeSet Overrides
    virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
    virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
        
    UPROPERTY(BlueprintReadOnly, Category = "", ReplicatedUsing = OnRep_Health)
    FGameplayAttributeData Health = 100.0;
    ATTRIBUTE_ACCESSORS(UCombatBaseAttributeSet, Health)
	UPROPERTY(BlueprintReadOnly, Category = "", ReplicatedUsing = OnRep_MaxHealth)
    FGameplayAttributeData MaxHealth = 100.0;
    ATTRIBUTE_ACCESSORS(UCombatBaseAttributeSet, MaxHealth)    
    
    UPROPERTY(BlueprintReadOnly, Category = "", ReplicatedUsing = OnRep_Armor)
    FGameplayAttributeData Armor = 100.0;
    ATTRIBUTE_ACCESSORS(UCombatBaseAttributeSet, Armor)   

	UPROPERTY(BlueprintReadOnly, Category = "", ReplicatedUsing = OnRep_MaxArmor)
    FGameplayAttributeData MaxArmor = 100.0;
    ATTRIBUTE_ACCESSORS(UCombatBaseAttributeSet, MaxArmor)    


protected:
    
    UFUNCTION()
    virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);

    UFUNCTION()
    virtual void OnRep_Armor(const FGameplayAttributeData& OldArmor);

	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

	UFUNCTION()
	virtual void OnRep_MaxArmor(const FGameplayAttributeData& OldMaxArmor);
	
	
};
