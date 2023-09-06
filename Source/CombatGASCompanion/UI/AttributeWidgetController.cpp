// Copyright SuspiciousDinosaur @ Youtube


#include "AttributeWidgetController.h"
#include "CombatGASCompanion/AbilitySystem/CombatAttributeSet.h"
#include "CombatGASCompanion/AbilitySystem/Data/AttributeInfo.h"

void UAttributeWidgetController::BroadcastInitialValues()
{
	UCombatAttributeSet* AS = CastChecked<UCombatAttributeSet>(AttributeSet);

	check(AttributeInfo)

	for (auto& Pair : AS->TagsToAttributes)
	{
		FCombatAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(Pair.Key);
		Info.AttributeValue = Pair.Value().GetNumericValue(AS);
		AttributeInfoDelegate.Broadcast(Info);
	}
}

void UAttributeWidgetController::BindCallbacksToDependencies()
{
	UCombatAttributeSet* AS = CastChecked<UCombatAttributeSet>(AttributeSet);

	for (auto& Pair : AS->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this,Pair,AS]
		(const FOnAttributeChangeData& Data)
			{
				FCombatAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(Pair.Key);
				Info.AttributeValue = Pair.Value().GetNumericValue(AS);
				AttributeInfoDelegate.Broadcast(Info);
			}
		);
	}
}
