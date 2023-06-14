// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "InputAction.h"
#include "CombatGASCompanion/Weapon/CombatRangedWeapon.h"
#include "ModularGameplayActors/GSCModularCharacter.h"
#include "CombatGASCompanion/CombatTypes/TurnInPlace.h"
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

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PostInitializeComponents() override;

	virtual void Jump() override;

	virtual void  PlayFireMontage(bool bIsAiming);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void FMove(const FInputActionValue& Value);

	void FLook(const FInputActionValue& Value);

	void FEquip();

	void FAimPressed();
	void FAimReleased();
	void FFirePressed();
	void FFireReleased();

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* PilotInputMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* Move;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* Look;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* Equip;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* Aim;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* Fire;

	//AimVariables

	void AimOffset(float DeltaTime);
	void FTurnInPlace(float DeltaTime);


private:
	//AimPrivate Variables

	float AO_Yaw;
	float InterpAO_Yaw;
	float AO_Pitch;
	FRotator StartingAimRotation;

	//TurnInPlace

	ETurningInPlace TurnInPlace;


	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess ="true"))
	class UWidgetComponent* OverheadWidget;

	UPROPERTY(ReplicatedUsing=OnRep_OverlappingWeapon)
	class ACombatRangedWeapon* OverlappingWeapon;

	UFUNCTION()
	void OnRep_OverlappingWeapon(ACombatRangedWeapon* LastWeapon);

	UPROPERTY(VisibleAnywhere)
	class URangedCombatComponent* CombatComponent;

	UFUNCTION(Server, Reliable)
	void ServerEquipButtonPressed();

	//Animation
	UPROPERTY(EditAnywhere, Category = Combat)
	class UAnimMontage* FireWeaponMontage;


public:
	void SetOverlappingWeapon(ACombatRangedWeapon* Weapon);

	bool IsWeaponEquipped();
	bool IsAiming();

	FORCEINLINE float GetAO_Yaw() { return AO_Yaw; }
	FORCEINLINE float GetAO_Pitch() { return AO_Pitch; }
	FORCEINLINE ETurningInPlace GetTurningInPlace() const { return TurnInPlace; }
	ACombatRangedWeapon* GetEquippedWeapon();
};
