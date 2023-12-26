// Copyright SuspiciousDinosaur @ Youtube


#include "WaitCoolDownChange.h"

#include "AbilitySystemComponent.h"

UWaitCoolDownChange* UWaitCoolDownChange::WaitForCoolDownChange(UAbilitySystemComponent* ASC,
                                                                const FGameplayTag& InCooldownTag)
{
	UWaitCoolDownChange* WaitCoolDownChange = NewObject<UWaitCoolDownChange>();
	WaitCoolDownChange->ASC = ASC;
	WaitCoolDownChange->CooldownTag = InCooldownTag;
	if (!IsValid(ASC) || !InCooldownTag.IsValid())
	{
		WaitCoolDownChange->EndTask();
		return nullptr;
	}

	//To know when CoolDownEffect Applied
	ASC->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(WaitCoolDownChange,
	                                                          &UWaitCoolDownChange::OnActiveEffectAdded);

	//To know When CoolDownTag is Applied

	ASC->RegisterGameplayTagEvent(InCooldownTag, EGameplayTagEventType::NewOrRemoved).AddUObject(
		WaitCoolDownChange, &UWaitCoolDownChange::CooldownTagChanged);

	return WaitCoolDownChange;
}


void UWaitCoolDownChange::EndTask()
{
	if (!IsValid(ASC))return;
	ASC->RegisterGameplayTagEvent(CooldownTag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);

	SetReadyToDestroy();
	MarkAsGarbage();
}

void UWaitCoolDownChange::CooldownTagChanged(const FGameplayTag InCoolDownTag, int32 NewCount)
{
	if (NewCount == 0)
	{
		CoolDownEndDelegate.Broadcast(0.f);
	}
}

void UWaitCoolDownChange::OnActiveEffectAdded(UAbilitySystemComponent* TargetASC,
                                              const FGameplayEffectSpec& EffectSpecApplied,
                                              FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer AssetTags;
	FGameplayTagContainer GrantedTags;
	EffectSpecApplied.GetAllAssetTags(AssetTags);
	EffectSpecApplied.GetAllGrantedTags(GrantedTags);
	if (AssetTags.HasTagExact(CooldownTag) || GrantedTags.HasTagExact(CooldownTag))
	{
		FGameplayEffectQuery GameplayEffectQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(
			CooldownTag.GetSingleTagContainer());
		TArray<float> TimesRemaining =
			ASC->GetActiveEffectsTimeRemaining(GameplayEffectQuery);
		if (TimesRemaining.Num() > 0)
		{
			int32 Index = 0;
			float LongestTimeRemaining = TimesRemaining[0];
			for (int32 i = 0; i < TimesRemaining.Num(); i++)
			{
				if (TimesRemaining[i] > LongestTimeRemaining)
				{
					LongestTimeRemaining = TimesRemaining[i];
				}
			}
			CoolDownStartDelegate.Broadcast(LongestTimeRemaining);
		}
	}
}
