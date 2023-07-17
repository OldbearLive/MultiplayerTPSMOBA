// Copyright SuspiciousDinosaur @ Youtube

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "CombatGASCompanion/Interfaces/InteractWithCrosshairsInterface.h"
#include "MinionCharacter.generated.h"

/**
 * 
 */
UCLASS()
class COMBATGASCOMPANION_API AMinionCharacter : public ABaseCharacter,public IInteractWithCrosshairsInterface
{
	GENERATED_BODY()

public:
	
	AMinionCharacter();

	virtual void BeginPlay() override;

	// Crosshair Interface Functions
	virtual void HighLightActor() override;
	virtual void UnHighLightActor() override;

virtual void InitAbilityActorInfo() override;
	//End Crosshair Interface Section
};
