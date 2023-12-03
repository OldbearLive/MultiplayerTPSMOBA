// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "CombatCharacterMovementComponent.h"
#include "CombatGASCompanion/CombatTypes/TurnInPlace.h"
#include "CombatGASCompanion/Interfaces/InteractWithCrosshairsInterface.h"
#include "CombatGASCompanion/UI/OverlayWidgetController.h"
#include "Components/WidgetComponent.h"
#include "CombatCharacter.generated.h"


class ACombatRangedWeapon;
class ACombatPlayerController;


UCLASS()
class COMBATGASCOMPANION_API ACombatCharacter : public ABaseCharacter, public IInteractWithCrosshairsInterface
{
	GENERATED_BODY()


	// Sets default values for this character's properties

public:
	ACombatCharacter(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Movement")
	UCombatCharacterMovementComponent* CombatCharacterMovementComponent;

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

	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Weapon")
	bool bIsWeaponEquipped;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Jet")
	bool bIsJet;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Jet")
	bool bIsDash;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Jet")
	bool bIsBoost;

	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Weapon")
	TArray<TObjectPtr<ACombatRangedWeapon>> CombatRangedWeaponsArray;

	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Weapon")
	int32 ActiveWeaponIndex;

	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Weapon")
	TArray<int32> AmmoClipArray;

	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Weapon")
	TArray<int32> MaxAmmoClipArray;

	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Weapon")
	TArray<int32> ReserveAmmoArray;

	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Weapon")
	TArray<int32> MaxReserveAmmoArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float CameraSensitivity = 1.0f;;


	UPROPERTY(EditDefaultsOnly, Category= "Combat")
	float DefaultMaxSpeed = 600;
	UPROPERTY(EditDefaultsOnly, Category= "Combat")
	float StaggerSpeed = 100;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 TagCount);
	void DeathTagChanged(const FGameplayTag CallbackTag, int32 TagCount);


	//
	//REMOTE STATSBAR FOR PLAYER
	//
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> RemoteStatsBar;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName InSocketNameCam = FName("NAME_None");

public:
	UPROPERTY(BlueprintAssignable, Category = "GAS|UI|Attributes")
	FOnAttributeChangedSignature OnHealthChangedSignature;

	UPROPERTY(BlueprintAssignable, Category = "GAS|UI|Attributes")
	FOnAttributeChangedSignature OnMaxHealthChangedSignature;

	UPROPERTY(BlueprintAssignable, Category = "GAS|UI|Attributes")
	FOnAttributeChangedSignature OnEnergyChangedSignature;

	UPROPERTY(BlueprintAssignable, Category = "GAS|UI|Attributes")
	FOnAttributeChangedSignature OnMaxEnergyChangedSignature;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	class USpringArmComponent* CameraBoom;

protected:
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
	class UCameraComponent* CameraComponent;


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


	UPROPERTY(EditAnywhere, Category = "Abilities|DefaultAbilities")
	TArray<TSubclassOf<UGameplayAbility>> WeaponStartupAbilities;

public:
	//MC RPCS


	//GETTERS

	FCollisionQueryParams GetIgnoreCharacterParams() const;


	FORCEINLINE float GetAO_Yaw() { return AO_Yaw; }
	FORCEINLINE bool GetWeaponEquip() { return bIsWeaponEquipped; }
	FORCEINLINE float GetAO_Pitch() { return AO_Pitch; }
	FORCEINLINE ETurningInPlace GetTurningInPlace() const { return TurnInPlace; }
	FORCEINLINE UCameraComponent* GetCameraComponent() const { return CameraComponent; }
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE ACombatRangedWeapon* GetCurrentWeapon() const
	{
		if (!CombatRangedWeaponsArray.IsEmpty() && CombatRangedWeaponsArray.IsValidIndex(ActiveWeaponIndex))
		{
			return CombatRangedWeaponsArray[ActiveWeaponIndex];
		}
		else
		{
			return nullptr;
		}
	}

	FORCEINLINE bool ShouldRotateRootBone() const { return bRotateRootBone; }

	FVector GetHitTarget() const;

	//Setters

	UFUNCTION(Server, Reliable)
	void Server_SetJetPacking(bool bJet);
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_SetDashing(bool bDash);
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_SetBoosting(bool bBoost);
};
