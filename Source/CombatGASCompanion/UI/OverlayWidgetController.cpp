// Copyright SuspiciousDinosaur @ Youtube


#include "OverlayWidgetController.h"

#include "CombatGASCompanion/AbilitySystem/CombatAbilitySystemComponent.h"
#include "CombatGASCompanion/AbilitySystem/CombatAttributeSet.h"
#include "CombatGASCompanion/AbilitySystem/Data/AbilityInfo.h"
#include "CombatGASCompanion/AbilitySystem/Data/LevelUpInfo.h"
#include "CombatGASCompanion/PlayerController/CombatPlayerState.h"

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
	ACombatPlayerState* CombatPlayerState = CastChecked<ACombatPlayerState>(PlayerState);
	CombatPlayerState->OnXPChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnXPChanged);


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

	UCombatAbilitySystemComponent* CombatASC = Cast<UCombatAbilitySystemComponent>(AbilitySystemComponent);
	if (CombatASC)
	{
		if (CombatASC->bStartupAbilitiesGiven)
		{
			OnInitializeStartupAbilities(CombatASC);
		}
		else
		{
			CombatASC->AbilitiesGiven.AddUObject(this, &UOverlayWidgetController::OnInitializeStartupAbilities);
		}

		//Effect Asset Tag Lambdas
		CombatASC->EffectAssetTags.AddLambda
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
}

void UOverlayWidgetController::OnInitializeStartupAbilities(UCombatAbilitySystemComponent* CombatASC)
{
	//TODO GetInformation AboutAll Given Abilities and Broadcast to Widget.
	if (!CombatASC->bStartupAbilitiesGiven)return;

	FForEachAbility BroadCastDelegate;
	BroadCastDelegate.BindLambda([this,CombatASC](const FGameplayAbilitySpec& AbilitySpec)
		{
			//TODO Need to figure out how to get Ability tag from GameplaySpec

			FCombatAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(CombatASC->GetAbilityTagFromSpec(AbilitySpec));

			Info.InputTag = CombatASC->GetInputTagFromSpec(AbilitySpec);
			AbilityInfoDelegate.Broadcast(Info);
		}
	);
	CombatASC->ForEachAbility(BroadCastDelegate);
}

void UOverlayWidgetController::OnXPChanged(int32 NewXP)
{
	ACombatPlayerState* CombatPlayerState = CastChecked<ACombatPlayerState>(PlayerState);
	ULevelUpInfo* LevelUpInfo = CombatPlayerState->LevelUpInfo;

	checkf(LevelUpInfo, TEXT("Unable to Find Level up Info, Please fill Player StateBP"))

	int32 Level = LevelUpInfo->FindLevelforExp(NewXP);

	int32 MaxLevel = LevelUpInfo->LevelUpInfos.Num();

	if (Level <= MaxLevel && Level > 0)
	{
		const int32 XPRequirement = LevelUpInfo->LevelUpInfos[Level].RequiredEXP;
		const int32 PreviousXPRequirement = LevelUpInfo->LevelUpInfos[Level - 1].RequiredEXP;

		const int32 DeltaLevelRequirement = XPRequirement - PreviousXPRequirement;

		const int32 XPForThisLevel = NewXP - PreviousXPRequirement;
		const float XPBarPercent = static_cast<float>(XPForThisLevel) / static_cast<float>(DeltaLevelRequirement);

		OnXPChangedDelegate.Broadcast(XPBarPercent);
	}
}
