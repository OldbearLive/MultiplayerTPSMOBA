// Copyright SuspiciousDinosaur @ Youtube


#include "CombatCharacterMovementComponent.h"

#include "CombatCharacter.h"
#include "GameFramework/Character.h"

bool UCombatCharacterMovementComponent::FSavedMove_Combat::CanCombineWith(const FSavedMovePtr& NewMove,
                                                                          ACharacter* InCharacter, float MaxDelta) const
{
	FSavedMove_Combat* CombatMove = static_cast<FSavedMove_Combat*>(NewMove.Get());
	if (Saved_bWantsToSprint != CombatMove->Saved_bWantsToSprint)
	{
		return false;
	}
	return FSavedMove_Character::CanCombineWith(NewMove, InCharacter, MaxDelta);
}

void UCombatCharacterMovementComponent::FSavedMove_Combat::Clear()
{
	FSavedMove_Character::Clear();

	Saved_bWantsToSprint = 0;
}

uint8 UCombatCharacterMovementComponent::FSavedMove_Combat::GetCompressedFlags() const
{
	uint8 Result = Super::GetCompressedFlags();
	if (Saved_bWantsToSprint)
	{
		Result |= FLAG_Custom_0;
	}
	return Result;
}

void UCombatCharacterMovementComponent::FSavedMove_Combat::SetMoveFor(ACharacter* C, float InDeltaTime,
                                                                      FVector const& NewAccel,
                                                                      FNetworkPredictionData_Client_Character&
                                                                      ClientData)
{
	FSavedMove_Character::SetMoveFor(C, InDeltaTime, NewAccel, ClientData);

	UCombatCharacterMovementComponent* CharacterMovementComponent = Cast<UCombatCharacterMovementComponent>(
		C->GetCharacterMovement());

	Saved_bWantsToSprint = CharacterMovementComponent->Safe_bWantsToSprint;
}

void UCombatCharacterMovementComponent::FSavedMove_Combat::PrepMoveFor(ACharacter* C)
{
	Super::PrepMoveFor(C);
	UCombatCharacterMovementComponent* CharacterMovementComponent = Cast<UCombatCharacterMovementComponent>(
		C->GetCharacterMovement());
	CharacterMovementComponent->Safe_bWantsToSprint = Saved_bWantsToSprint;
}

UCombatCharacterMovementComponent::FNetworkPredictionData_Client_Combat::FNetworkPredictionData_Client_Combat(
	const UCharacterMovementComponent& ClientMovement) : Super(ClientMovement)
{
}

FSavedMovePtr UCombatCharacterMovementComponent::FNetworkPredictionData_Client_Combat::AllocateNewMove()
{
	return FSavedMovePtr(new FSavedMove_Combat());
}

FNetworkPredictionData_Client* UCombatCharacterMovementComponent::GetPredictionData_Client() const
{
	check(PawnOwner != nullptr)
	if (ClientPredictionData == nullptr)
	{
		UCombatCharacterMovementComponent* MutableThis = const_cast<UCombatCharacterMovementComponent*>(this);

		MutableThis->ClientPredictionData = new FNetworkPredictionData_Client_Combat(*this);
		MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 100.0f;
		MutableThis->ClientPredictionData->NoSmoothNetUpdateDist = 200.0f;
	}
	return ClientPredictionData;
}

void UCombatCharacterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);

	Safe_bWantsToSprint = (Flags & FSavedMove_Character::FLAG_Custom_0) != 0;
}

void UCombatCharacterMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation,
                                                          const FVector& OldVelocity)
{
	Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);

	if (MovementMode == MOVE_Walking)
	{
		if (Safe_bWantsToSprint)
		{
			MaxWalkSpeed = Sprint_MaxWalkSpeed;
		}
		else
		{
			MaxWalkSpeed = Walk_MaxWalkSpeed;
		}
	}
}

void UCombatCharacterMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();

	CombatOwnerCharacter = Cast<ACombatCharacter>(GetOwner());
}

void UCombatCharacterMovementComponent::SprintPressed()
{
	Safe_bWantsToSprint = true;
}

void UCombatCharacterMovementComponent::SprintReleased()
{
	Safe_bWantsToSprint = false;
}

bool UCombatCharacterMovementComponent::IsCustomMovementMode(ECustomMovementMode InCustomMovementMode)
{
	return MovementMode == MOVE_Custom && CustomMovementMode == InCustomMovementMode;
}

UCombatCharacterMovementComponent::UCombatCharacterMovementComponent()
{
}
