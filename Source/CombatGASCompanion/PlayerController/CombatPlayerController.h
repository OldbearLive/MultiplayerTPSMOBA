// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "CombatPlayerController.generated.h"


class ACombatCharacter;
class UDamageTextComponent;
class UCombatInputConfig;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class IInteractWithCrosshairsInterface;
class UCombatAbilitySystemComponent;


#define TRACE_LENGTH  50000.f
/**
 * 
 */
UCLASS()
class COMBATGASCOMPANION_API ACombatPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ACombatPlayerController();

	virtual void PlayerTick(float DeltaTime) override;
	void CreateDamageWidget(float DamageAmount, bool bIsShieldHit, bool IsOverloadHit,
	                        UDamageTextComponent* DamageText, const FVector inWorldPos);

	FVector HitTarget;

	FHitResult HitResult;

	UPROPERTY(EditAnywhere, Category = "Trace")
	float TraceLength = 2000;

	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(float DamageAmount, AActor* TargetActor, bool bIsShieldHit,
	                      bool IsOverloadHit);

protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	virtual void OnPossess(APawn* InPawn) override;

private:
	ACharacter* CombatCharacter;

	//InputMappingContext -> Set in BP
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> CombatContext;


	/*
	 *
	 **INPUTACTIONS SECTION
	 * 
	 */

	//InputActions  Move -> Set in BP
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	//InputMappingContext Look -> Set in BP
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> LookAction;

	/*
	 *
	 **InputActionFunctions SECTION
	 * 
	 */

	//InputFunction Move -> Bound to InputAction of Same name
	void Move(const FInputActionValue& InputActionValue);

	//InputFunction Move -> Bound to InputAction of Same name
	void Look(const FInputActionValue& InputActionValue);

	//ABILITY INPUT ACTIONS

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UCombatAbilitySystemComponent> CombatAbilitySystemComponent;

	UCombatAbilitySystemComponent* GetCombatASC();

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UCombatInputConfig> InputConfig;

	void AbilityInputTagsPressed(FGameplayTag InputTag);
	void AbilityInputTagsReleased(FGameplayTag InputTag);
	void AbilityInputTagsHeld(FGameplayTag InputTag);

	//
	//
	//HUD Crosshairs
	//
	//

	//
	//
	//Tracing on Crosshair position
	//
	//
	void TraceUnderCrosshairs(FHitResult& TraceHitResult);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> StatusTextComponentClass;


	TObjectPtr<IInteractWithCrosshairsInterface> LastActor;
	TObjectPtr<IInteractWithCrosshairsInterface> ThisActor;
};
