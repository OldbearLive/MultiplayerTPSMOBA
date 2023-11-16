// Copyright SuspiciousDinosaur @ Youtube

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "CombatGASCompanion/Interfaces/CombatInterface.h"
#include "GameFramework/Character.h"
#include "GameFramework/Pawn.h"
#include "EnemyPawnBase.generated.h"

class UGameplayAbility;
class UGameplayEffect;
class UAbilitySystemComponent;
class UAttributeSet;

UCLASS()
class COMBATGASCOMPANION_API AEnemyPawnBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

	// Sets default values for this pawn's properties
public:
	AEnemyPawnBase();
	
	

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	virtual void Die() override;

	UFUNCTION(NetMulticast,Reliable)
	virtual void MulticastHandleDeath();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//
	//Pointers for ASC and Attribute set for base character. Will be constructed in either a Player state or the Owning Character in case of NPCs
	//
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	virtual void InitAbilityActorInfo();
	
	virtual  void InitializeDefaultAttributes() const;

	void ApplyEffectSpecToSelf(TSubclassOf<UGameplayEffect> GameplayEffect, float Level) const;

	void AddCharacterAbilities();

	

protected:
	// Called to bind functionality to input
private:
	UPROPERTY(EditAnywhere, Category = "Abilities|DefaultAbilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbiity;

public:
	
};
