// Copyright SuspiciousDinosaur @ Youtube

#pragma once

#include "CoreMinimal.h"
#include "CombatGASCompanion/AbilitySystem/Data/CombatCharacterClassInfo.h"
#include "CombatGASCompanion/Interfaces/InteractWithCrosshairsInterface.h"
#include "CombatGASCompanion/Pawn/EnemyPawnBase.h"
#include "CombatGASCompanion/PlayerController/CombatPlayerController.h"
#include "CombatGASCompanion/UI/OverlayWidgetController.h"
#include "MinionCharacter.generated.h"

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
	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override;
	virtual AActor* GetCombatTarget_Implementation() const override;
	virtual ECharacterClass GetCharacterClass_Implementation() override;
	
	virtual void Die() override;

	virtual void MulticastHandleDeath() override;
	
	virtual bool IsDead_Implementation() const override;

	virtual AActor* GetAvatar_Implementation() override;
	// End Combat Interface Functions
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Combat")
	TArray<FTaggedMontage> AttackMontages;

	
	UPROPERTY(BlueprintReadOnly, Category= "Combat")
	bool bHitReacting = false;
	UPROPERTY(BlueprintReadOnly, Category= "Combat")
	bool bDead = false;
	UPROPERTY(EditDefaultsOnly, Category= "Combat")
	float DefaultMaxSpeed = 1500;
	UPROPERTY(EditDefaultsOnly, Category= "Combat")
	float StaggerSpeed = 1500;

	UPROPERTY(BlueprintReadWrite,Category = "Combat")
	TObjectPtr<AActor> CombatTarget;

protected:
	virtual void BeginPlay() override;

	virtual void InitializeDefaultAttributes() const override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void InitAbilityActorInfo() override;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> RemoteStatsBar;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level = 1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Class Defaults")
	ECharacterClass CharacterClass = ECharacterClass::Walker;

	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 TagCount);
	void DeathTagChanged(const FGameplayTag CallbackTag, int32 TagCount);

	//Behaviour TreeSection

	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<ACombatAIController> CombatAIController;

	UPROPERTY()
	TObjectPtr<ACombatPlayerController> CombatPlayerController;

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
