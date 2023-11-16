// Copyright SuspiciousDinosaur @ Youtube

#pragma once

#include "CoreMinimal.h"
#include "CombatGameplayAbility.h"
#include "CombatDamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class COMBATGASCOMPANION_API UCombatDamageGameplayAbility : public UCombatGameplayAbility
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FGameplayTag,FScalableFloat>DamageTypes;


	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);
};
