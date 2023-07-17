// Copyright SuspiciousDinosaur @ Youtube


#include "OverlayWidgetController.h"

#include "CombatGASCompanion/AbilitySystem/CombatAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	UCombatAttributeSet* CombatAttributeSet = CastChecked<UCombatAttributeSet>(AttributeSet);

	OnHealthChangedSignature.Broadcast(CombatAttributeSet->GetHealth());
	OnMaxHealthChangedSignature.Broadcast(CombatAttributeSet->GetMaxHealth());
	OnEnergyChangedSignature.Broadcast(CombatAttributeSet->GetEnergy());
	OnMaxEnergyChangedSignature.Broadcast(CombatAttributeSet->GetMaxEnergy());

	/*
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(CombatAttributeSet->GetHealthAttribute()).AddUObject(this,);
*/
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const UCombatAttributeSet* CombatAttributeSet = CastChecked<UCombatAttributeSet>(AttributeSet);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(CombatAttributeSet->GetHealthAttribute()).AddUObject
	(
		this,
	 &UOverlayWidgetController::HealthChanged
	 );
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(CombatAttributeSet->GetMaxHealthAttribute()).AddUObject
	(
		this,
	 &UOverlayWidgetController::MaxHealthChanged
	 );
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(CombatAttributeSet->GetEnergyAttribute()).AddUObject
	(
		this,
	 &UOverlayWidgetController::EnergyChanged
	 );
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(CombatAttributeSet->GetMaxEnergyAttribute()).AddUObject
	(
		this,
	 &UOverlayWidgetController::MaxEnergyChanged
	 );
}

void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChangedSignature.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChangedSignature.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::EnergyChanged(const FOnAttributeChangeData& Data) const
{
	OnEnergyChangedSignature.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxEnergyChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxEnergyChangedSignature.Broadcast(Data.NewValue);
}
