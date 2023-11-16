// Copyright SuspiciousDinosaur @ Youtube

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CombatCharacterMovementComponent.generated.h"

UENUM(BlueprintType)
enum ECustomMovementMode
{
	CMOVE_NONE UMETA(Hidden),
	CMOVE_GLIDE UMETA(DisplayName = "Slide"),
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

	//CUSTOM SAVED MOVE TO SYNC Member Variables and Bitflags
	class FSavedMove_Combat : public FSavedMove_Character
	{
		typedef FSavedMove_Character Super;

		uint8 Saved_bWantsToSprint:1;

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


public:
	UCombatCharacterMovementComponent();

	//PARAMETERS
	UPROPERTY(EditDefaultsOnly)
	float Sprint_MaxWalkSpeed;
	UPROPERTY(EditDefaultsOnly)
	float Walk_MaxWalkSpeed;

	//Transient
	//
	//
	//CUSTOMSPRINT
	bool Safe_bWantsToSprint;
	//
	//CHARACTERREF
	UPROPERTY(Transient)
	ACombatCharacter* CombatOwnerCharacter;
	//
	//
	//End Transient

#pragma region Networking
	virtual FNetworkPredictionData_Client* GetPredictionData_Client() const override;
#pragma endregion

protected:
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;

	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;

	virtual void InitializeComponent() override;

public:
	UFUNCTION(BlueprintCallable)
	void SprintPressed();
	UFUNCTION(BlueprintCallable)
	void SprintReleased();

	UFUNCTION(BlueprintCallable)
	bool IsCustomMovementMode(ECustomMovementMode InCustomMovementMode);
};

