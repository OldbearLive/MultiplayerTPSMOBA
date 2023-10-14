// Copyright SuspiciousDinosaur @ Youtube

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "CombatGASCompanion/Interfaces/CombatInterface.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"


class UGameplayAbility;
class UGameplayEffect;
class UAbilitySystemComponent;
class UAttributeSet;

UCLASS()
class COMBATGASCOMPANION_API ABaseCharacter : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	

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

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	void InitializeDefaultAttributes() const;

	void ApplyEffectSpecToSelf(TSubclassOf<UGameplayEffect> GameplayEffect, float Level) const;

	void AddCharacterAbilities();

private:
	UPROPERTY(EditAnywhere, Category = "Abilities|DefaultAbilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbiity;
};
