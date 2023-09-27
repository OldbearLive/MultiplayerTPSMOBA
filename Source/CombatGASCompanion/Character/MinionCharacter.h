// Copyright SuspiciousDinosaur @ Youtube

#pragma once

#include "CoreMinimal.h"
#include "CombatGASCompanion/AbilitySystem/Data/CombatCharacterClassInfo.h"
#include "CombatGASCompanion/Interfaces/InteractWithCrosshairsInterface.h"
#include "CombatGASCompanion/Pawn/EnemyPawnBase.h"
#include "CombatGASCompanion/UI/OverlayWidgetController.h"
#include "MinionCharacter.generated.h"

class UWidgetComponent;
/**
 * 
 */
UCLASS()
class COMBATGASCOMPANION_API AMinionCharacter : public AEnemyPawnBase, public IInteractWithCrosshairsInterface
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

	virtual void InitializeDefaultAttributes() const override;

	virtual void InitAbilityActorInfo() override;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> RemoteStatsBar;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level = 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	ECharacterClass CharacterClass= ECharacterClass::BasicMinion;

private:

	//HEALTH AND ENERGY DELEGATES
public:
	UPROPERTY(BlueprintAssignable,Category = "GAS|UI|Attributes")
	FOnAttributeChangedSignature OnHealthChangedSignature;

	UPROPERTY(BlueprintAssignable,Category = "GAS|UI|Attributes")
	FOnAttributeChangedSignature OnMaxHealthChangedSignature;
	
	UPROPERTY(BlueprintAssignable,Category = "GAS|UI|Attributes")
	FOnAttributeChangedSignature OnEnergyChangedSignature;
	
	UPROPERTY(BlueprintAssignable,Category = "GAS|UI|Attributes")
	FOnAttributeChangedSignature OnMaxEnergyChangedSignature;
};
