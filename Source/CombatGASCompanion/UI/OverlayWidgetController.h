// Copyright SuspiciousDinosaur @ Youtube

#pragma once

#include "CoreMinimal.h"
#include "CombatWidgetController.h"
#include "OverlayWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature,float,NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHealthChangedSignature,float,NewMaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnergyChangedSignature,float,NewEnergy);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxEnergyChangedSignature,float,NewMaxEnergy);

/**
 * 
 */
UCLASS(BlueprintType,Blueprintable)
class COMBATGASCOMPANION_API UOverlayWidgetController : public UCombatWidgetController
{
	GENERATED_BODY()
public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable,Category = "GAS|UI|Attributes")
	FOnHealthChangedSignature OnHealthChangedSignature;

	UPROPERTY(BlueprintAssignable,Category = "GAS|UI|Attributes")
	FOnMaxHealthChangedSignature OnMaxHealthChangedSignature;
	
	UPROPERTY(BlueprintAssignable,Category = "GAS|UI|Attributes")
	FOnEnergyChangedSignature OnEnergyChangedSignature;
	
	UPROPERTY(BlueprintAssignable,Category = "GAS|UI|Attributes")
	FOnMaxEnergyChangedSignature OnMaxEnergyChangedSignature;

protected:

	void HealthChanged(const FOnAttributeChangeData&Data) const;
	void MaxHealthChanged(const FOnAttributeChangeData&Data) const;
	void EnergyChanged(const FOnAttributeChangeData&Data) const;
	void MaxEnergyChanged(const FOnAttributeChangeData&Data) const;
};
