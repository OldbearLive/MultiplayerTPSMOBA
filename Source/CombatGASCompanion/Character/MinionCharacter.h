// Copyright SuspiciousDinosaur @ Youtube

#pragma once

#include "CoreMinimal.h"
#include "CombatGASCompanion/AbilitySystem/Data/CombatCharacterClassInfo.h"
#include "CombatGASCompanion/Interfaces/InteractWithCrosshairsInterface.h"
#include "CombatGASCompanion/Pawn/EnemyPawnBase.h"
#include "CombatGASCompanion/UI/OverlayWidgetController.h"
#include "MinionCharacter.generated.h"

class UFloatingPawnMovement;
class UWidgetComponent;
class UBehaviorTree;
class ACombatAIController;
/**
 * 
 */
UCLASS()
class COMBATGASCOMPANION_API AMinionCharacter : public AEnemyPawnBase, public IInteractWithCrosshairsInterface
{
	GENERATED_BODY()

public:
	AMinionCharacter();

	virtual void PossessedBy(AController* NewController) override;


	// Crosshair Interface Functions
	virtual void HighLightActor() override;
	virtual void UnHighLightActor() override;
	//End Crosshair Interface Section

	// Combat Interface Functions
	virtual int32 GetPlayerLevel() override;
	// End Combat Interface Functions

	virtual UPawnMovementComponent* GetMovementComponent() const override;

	UPROPERTY(BlueprintReadOnly, Category= "Combat")
	bool bHitReacting = false;
	UPROPERTY(BlueprintReadOnly, Category= "Combat")
	bool bDead = false;
	UPROPERTY(EditDefaultsOnly, Category= "Combat")
	float DefaultMaxSpeed = 1500;
	UPROPERTY(EditDefaultsOnly, Category= "Combat")
	float StaggerSpeed = 1500;

	

protected:
	virtual void BeginPlay() override;

	virtual void InitializeDefaultAttributes() const override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void InitAbilityActorInfo() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UFloatingPawnMovement* PawnMovement;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> RemoteStatsBar;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level = 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	ECharacterClass CharacterClass = ECharacterClass::BasicMinion;

	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 TagCount);
	void DeathTagChanged(const FGameplayTag CallbackTag, int32 TagCount);

	//Behaviour TreeSection

	UPROPERTY(EditAnywhere,Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;
	
	UPROPERTY()
	TObjectPtr<ACombatAIController> CombatAIController;

private:
	//HEALTH AND ENERGY DELEGATES
public:
	UPROPERTY(BlueprintAssignable, Category = "GAS|UI|Attributes")
	FOnAttributeChangedSignature OnHealthChangedSignature;

	UPROPERTY(BlueprintAssignable, Category = "GAS|UI|Attributes")
	FOnAttributeChangedSignature OnMaxHealthChangedSignature;

	UPROPERTY(BlueprintAssignable, Category = "GAS|UI|Attributes")
	FOnAttributeChangedSignature OnEnergyChangedSignature;

	UPROPERTY(BlueprintAssignable, Category = "GAS|UI|Attributes")
	FOnAttributeChangedSignature OnMaxEnergyChangedSignature;
};
