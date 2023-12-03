// Copyright SuspiciousDinosaur @ Youtube

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CombatCharacterMovementComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndJet)
;

UENUM(BlueprintType)
enum ECombatCustomMovementMode
{
	CMOVE_NONE UMETA(Hidden),
	CMOVE_JET UMETA(DisplayName = "Jet"),
	CMOVE_MAX UMETA(Hidden),
};

class ACombatCharacter;

/**
 * 
 */
UCLASS()
class COMBATGASCOMPANION_API UCombatCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
#pragma region Saved Move_ NetworkPredictionData Classes
	//CUSTOM SAVED MOVE TO SYNC Member Variables and Bitflags
	class FSavedMove_Combat : public FSavedMove_Character
	{
		typedef FSavedMove_Character Super;

		//FLAG
		uint8 Saved_bWantsToSprint : 1;

		uint8 Saved_bWantsToJetPack : 1;

	public:
		FSavedMove_Combat();

		virtual bool
		CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const override;
		virtual void Clear() override;
		virtual uint8 GetCompressedFlags() const override;
		virtual void SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel,
		                        FNetworkPredictionData_Client_Character& ClientData) override;
		virtual void PrepMoveFor(ACharacter* C) override;
	};

	// SetDefault Saved move class to Custom saved move declared Above.
	class FNetworkPredictionData_Client_Combat : public FNetworkPredictionData_Client_Character
	{
	public:
		FNetworkPredictionData_Client_Combat(const UCharacterMovementComponent& ClientMovement);

		typedef FNetworkPredictionData_Client_Character Super;
		virtual FSavedMovePtr AllocateNewMove() override;
	};
#pragma endregion

public:
	UCombatCharacterMovementComponent();

#pragma region BP Parameters
	//SPRINT
	UPROPERTY(EditDefaultsOnly)
	float Sprint_MaxWalkSpeed;
	UPROPERTY(EditDefaultsOnly)
	float Walk_MaxWalkSpeed;

	//JETPACK
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float JetPack_MaxSpeed = 1000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float JetPack_BoosMaxSpeed = 2000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float JetPack_Impulse = 5000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float JetPack_Gravity = 5000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float JetThrustUpCoeff = 2.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float JetPack_Friction = 1.5f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float JetPack_BrakingDeceleration = 512.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MaxJetAltitude = 3000.0f;

	UPROPERTY(BlueprintAssignable)
	FOnEndJet EndJet;

#pragma endregion

#pragma region Transient

	bool Safe_bWantsToSprint;

	bool Safe_bWantsToJetPack;

	UPROPERTY(Transient)
	ACombatCharacter* CombatOwnerCharacter;
#pragma endregion


	virtual bool IsMovingOnGround() const override;

#pragma region Networking
	virtual FNetworkPredictionData_Client* GetPredictionData_Client() const override;
#pragma endregion

protected:
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;

	virtual void InitializeComponent() override;

	virtual void UpdateCharacterStateBeforeMovement(float DeltaSeconds) override;

	virtual void PhysCustom(float deltaTime, int32 Iterations) override;


#pragma region Sprinting BP Functions
	//BLueprintExposed Functions
public:
	UFUNCTION(BlueprintCallable)
	void SprintPressed();
	UFUNCTION(BlueprintCallable)
	void SprintReleased();
#pragma endregion
#pragma region JetPacking BP Functions_Variables
	//BLueprintExposed Functions
public:
	UPROPERTY(BlueprintReadWrite)
	bool bBoost = false;
	UPROPERTY(BlueprintReadWrite)
	bool bDash = false;
	UPROPERTY(BlueprintReadWrite)
	bool bThrust = false;
	UFUNCTION(BlueprintCallable)
	void JetPressed();
	UFUNCTION(BlueprintCallable)
	void JetReleased();
#pragma endregion

	UFUNCTION(BlueprintCallable)
	bool IsCustomMovementMode(ECombatCustomMovementMode InCustomMovementMode);

private:
#pragma region JetPack c++ Funtions

	void EnterJet();
	void ExitJet();
	void PhysJet(float DeltaTime, int32 Iterations);
	bool GetJetSurface(FHitResult& Hit);

#pragma endregion
};


#pragma region Networking
#pragma endregion
