// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "CombatGASCompanion/CombatTypes/TurnInPlace.h"
#include "CombatGASCompanion/Interfaces/InteractWithCrosshairsInterface.h"
#include "CombatCharacter.generated.h"


UCLASS()
class COMBATGASCOMPANION_API ACombatCharacter : public ABaseCharacter, public IInteractWithCrosshairsInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACombatCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Handles Replicated Variables
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//Set ASC and Attributeset from Playerstate
	virtual void PossessedBy(AController* NewController) override;

	//Set ASC and Attributeset from Playerstate
	virtual void OnRep_PlayerState() override;

	//Get Level From CombatInterface
	virtual int32 GetPlayerLevel() override;

	

	
	virtual void PostInitializeComponents() override;


	virtual void Jump() override;
	

	virtual void HighLightActor() override;
	virtual void UnHighLightActor() override;

	virtual void OnRep_ReplicatedMovement() override;


	
	UPROPERTY(EditAnywhere, Category = "CROSSHAIRS")
    	class UTexture2D* DefaultCrosshair;

	UPROPERTY(EditAnywhere, Category = "CROSSHAIRS")
	FLinearColor CrosshairDefaultColor;

	UPROPERTY(BlueprintReadWrite,Replicated, Category = "Weapon")
	bool bIsWeaponEquipped;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
	
	
	//AimVariables

	void AimOffset(float DeltaTime);
	void FTurnInPlace(float DeltaTime);
	void SimProxiesTurn();
	void CalculateAO_Pitch();


	//Highlight

	

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

	UPROPERTY(VisibleAnywhere)
	class URangedCombatComponent* CombatComponent;
	
	
	//CameraHideFunction
	void HideCamIfCharacterClose();

	UPROPERTY(EditAnywhere, Category = Combat)
	float HideCamThreshhold;

	//Simproxy Turns
	bool bRotateRootBone;
	float TurnThreshhold = 0.5f;
	FRotator ProxyRotationLastFrame;
	FRotator ProxyRotation;
	float ProxyYaw;
	float TimeSinceLastMovementReplication;

	float CalculateSpeed();


	/**
	 * GAS Function to initialize AbilityActorInfo 
	 */
	void InitAbilityActorInfo();


	UPROPERTY(EditAnywhere,Category = "Abilities|DefaultAbilities")
	TArray<TSubclassOf<UGameplayAbility>> WeaponStartupAbilities;

	
	
public:

	
	//MC RPCS
	


	//GETTERS


	FORCEINLINE float GetAO_Yaw() { return AO_Yaw; }
	FORCEINLINE bool GetWeaponEquip() { return bIsWeaponEquipped; }
	FORCEINLINE float GetAO_Pitch() { return AO_Pitch; }
	FORCEINLINE ETurningInPlace GetTurningInPlace() const { return TurnInPlace; }
	FORCEINLINE UCameraComponent* GetCameraComponent() const { return CameraComponent; }
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE bool ShouldRotateRootBone() const { return bRotateRootBone; }

	FVector GetHitTarget() const;
};
