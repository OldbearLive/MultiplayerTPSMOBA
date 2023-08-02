// Copyright SuspiciousDinosaur @ Youtube

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxHealth.generated.h"

/**
 * 
 */
UCLASS()
class COMBATGASCOMPANION_API UMMC_MaxHealth : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:

    //Constructor of the MMC Class which can set the Attribute Definition
	UMMC_MaxHealth();

	//Function to calculate the custom features overridden from MMC which passes a Spec which can be used to gather data about Source and Target and their tags
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:

	FGameplayEffectAttributeCaptureDefinition ToughnessDefinition;
	
};
