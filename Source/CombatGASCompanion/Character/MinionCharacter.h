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
class COMBATGASCOMPANION_API AMinionCharacter : public ABaseCharacter, public IInteractWithCrosshairsInterface
{
	GENERATED_BODY()

public:
	AMinionCharacter();


	// Crosshair Interface Functions
	virtual void HighLightActor() override;
	virtual void UnHighLightActor() override;
	//End Crosshair Interface Section

	// Combat Interface Functions
	virtual int32 GetPlayerLevel() override;
	// End Combat Interface Functions
protected:
	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level = 1;

private:
};
