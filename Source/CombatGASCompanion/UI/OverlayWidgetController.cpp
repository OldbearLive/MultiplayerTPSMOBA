// Copyright SuspiciousDinosaur @ Youtube


#include "OverlayWidgetController.h"

#include "CombatGASCompanion/AbilitySystem/CombatAbilitySystemComponent.h"
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

	//Lambdas to bind to Delegates
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(CombatAttributeSet->GetHealthAttribute()).
	                        AddLambda(
		                        [this](const FOnAttributeChangeData& Data)
		                        {
			                        OnHealthChangedSignature.Broadcast(Data.NewValue);
		                        }
	                        );

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(CombatAttributeSet->GetMaxHealthAttribute()).
	                        AddLambda(
		                        [this](const FOnAttributeChangeData& Data)
		                        {
			                        OnMaxHealthChangedSignature.Broadcast(Data.NewValue);
		                        }
	                        );

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(CombatAttributeSet->GetEnergyAttribute()).
	                        AddLambda(
		                        [this](const FOnAttributeChangeData& Data)
		                        {
			                        OnEnergyChangedSignature.Broadcast(Data.NewValue);
		                        }
	                        );

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(CombatAttributeSet->GetMaxEnergyAttribute()).
	                        AddLambda(
		                        [this](const FOnAttributeChangeData& Data)
		                        {
			                        OnMaxEnergyChangedSignature.Broadcast(Data.NewValue);
		                        }
	                        );

	//Effect Asset Tag Lambdas
	Cast<UCombatAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda
	(
		[this](const FGameplayTagContainer& AssetTags)
		{
			for (const FGameplayTag& Tag : AssetTags)
			{
				// if Tag.Matches (Parent) a.1 matches (a), it returns true, argument taken in brackets needs to be less
				// in the heirarchy of tags to match
				FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
				if (Tag.MatchesTag(MessageTag))
				{
					const FUIWidgetRow* UIRow = GetDataTableByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);

					MessageWudgetRowDelegate.Broadcast(*UIRow);
				}
			}
		}
	);
}
