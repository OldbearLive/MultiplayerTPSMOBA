// Copyright SuspiciousDinosaur @ Youtube

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameplayEffect.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "WaitCoolDownChange.generated.h"


class UAbilitySystemComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCoolDownChangeSignature, float, TimeRemaining);

/**
 * 
 */
UCLASS(Blueprintable,meta = (ExposedAsyncProxy = "AsyncTask"))
class COMBATGASCOMPANION_API UWaitCoolDownChange : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FCoolDownChangeSignature CoolDownStartDelegate;
	UPROPERTY(BlueprintAssignable)
	FCoolDownChangeSignature CoolDownEndDelegate;

	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly = "true"))
	static UWaitCoolDownChange* WaitForCoolDownChange(UAbilitySystemComponent* ASC, const FGameplayTag& InCooldownTag);

	UFUNCTION(BlueprintCallable)
	void EndTask();

protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ASC;

	UPROPERTY()
	FGameplayTag CooldownTag;

	
	void CooldownTagChanged(const FGameplayTag InCoolDownTag, int32 NewCount);

	void OnActiveEffectAdded(UAbilitySystemComponent* TargetASC, const FGameplayEffectSpec& EffectSpecApplied,
	                         FActiveGameplayEffectHandle ActiveEffectHandle);
};
