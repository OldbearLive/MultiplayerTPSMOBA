// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "InputAction.h"
#include "CombatGASCompanion/Weapon/CombatRangedWeapon.h"
#include "ModularGameplayActors/GSCModularCharacter.h"
#include "CombatCharacter.generated.h"

class UInputAction;

UCLASS()
class COMBATGASCOMPANION_API ACombatCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACombatCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>&OutLifetimeProps)const override;

	void SetOverlappingWeapon(ACombatRangedWeapon*Weapon);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	void FMove(const FInputActionValue& Value);

	void FLook(const FInputActionValue& Value);


	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* PilotInputMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* Move;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* Look;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* JumpAction;


private:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,meta = (AllowPrivateAccess ="true"))
	class UWidgetComponent*OverheadWidget;

	UPROPERTY(ReplicatedUsing=OnRep_OverlappingWeapon)
	class ACombatRangedWeapon*OverlappingWeapon;

	UFUNCTION()
	void OnRep_OverlappingWeapon(ACombatRangedWeapon*LastWeapon);
};
