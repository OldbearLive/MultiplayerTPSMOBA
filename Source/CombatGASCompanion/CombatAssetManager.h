// Copyright SuspiciousDinosaur @ Youtube

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "CombatAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class COMBATGASCOMPANION_API UCombatAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:

	static UCombatAssetManager& Get();

protected:
	virtual void StartInitialLoading() override;
};
