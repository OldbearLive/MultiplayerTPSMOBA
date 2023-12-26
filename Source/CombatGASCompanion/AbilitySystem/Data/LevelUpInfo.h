// Copyright SuspiciousDinosaur @ Youtube

#pragma once

#include "CoreMinimal.h"
#include "ScalableFloat.h"
#include "Engine/DataAsset.h"
#include "LevelUpInfo.generated.h"

USTRUCT(BlueprintType)
struct FCombatLevelUpInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	int32 RequiredEXP = 0;

	UPROPERTY(EditDefaultsOnly)
	int32 RewardedAttributePoint = 1;

	UPROPERTY(EditDefaultsOnly)
	int32 RewardedCurrency = 100;

};

/**
 * 
 */
UCLASS()
class COMBATGASCOMPANION_API ULevelUpInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TArray<FCombatLevelUpInfo> LevelUpInfos;

	int32 FindLevelforExp(int32 inExp);
};
