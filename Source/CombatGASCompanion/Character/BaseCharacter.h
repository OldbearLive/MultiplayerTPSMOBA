// Copyright SuspiciousDinosaur @ Youtube

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"


class UAbilitySystemComponent;
class UAttributeSet;

UCLASS()
class COMBATGASCOMPANION_API ABaseCharacter : public ACharacter,public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const {return AttributeSet;}
	
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

	virtual  void InitAbilityActorInfo();

	
};
