// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CombatPlayerController.generated.h"


class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class IInteractWithCrosshairsInterface;

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

protected:

	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	

private:

	//InputMappingContext -> Set in BP
	UPROPERTY(EditAnywhere,Category = "Input")
	TObjectPtr<UInputMappingContext>CombatContext;

	/*
	 *
	 **INPUTACTIONS SECTION
	 * 
	 */
	
	//InputActions  Move -> Set in BP
	UPROPERTY(EditAnywhere,Category = "Input")
	TObjectPtr<UInputAction>MoveAction;
	
	//InputMappingContext Look -> Set in BP
	UPROPERTY(EditAnywhere,Category = "Input")
	TObjectPtr<UInputAction>LookAction;

	//InputMappingContext Jump -> Set in BP
	UPROPERTY(EditAnywhere,Category = "Input")
	TObjectPtr<UInputAction>JumpAction;
	
	//InputMappingContext Equip -> Set in BP
	UPROPERTY(EditAnywhere,Category = "Input")
	TObjectPtr<UInputAction>EquipAction;

	//InputMappingContext Aim -> Set in BP
	UPROPERTY(EditAnywhere,Category = "Input")
	TObjectPtr<UInputAction>AimAction;
	
	//InputMappingContext Fire -> Set in BP
	UPROPERTY(EditAnywhere,Category = "Input")
	TObjectPtr<UInputAction>FireAction;
	
	/*
	 *
	 **InputActionFunctions SECTION
	 * 
	 */

	//InputFunction Move -> Bound to InputAction of Same name
	void Move(const FInputActionValue& InputActionValue);

	//InputFunction Move -> Bound to InputAction of Same name
	void Look(const FInputActionValue& InputActionValue);



	//
	//Tracing on Crosshair position
	//
	//
	void TraceUnderCrosshairs(FHitResult& TraceHitResult);
	
	TObjectPtr<IInteractWithCrosshairsInterface>  LastActor;
	TObjectPtr<IInteractWithCrosshairsInterface> ThisActor;
	

};



