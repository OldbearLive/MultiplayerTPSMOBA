// Copyright SuspiciousDinosaur @ Youtube


#include "CombatCharacterMovementComponent.h"

#include "CombatCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"

#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"


UCombatCharacterMovementComponent::UCombatCharacterMovementComponent()
{
}

UCombatCharacterMovementComponent::FSavedMove_Combat::FSavedMove_Combat()
{
}

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
	Saved_bWantsToJetPack = 0;
}

uint8 UCombatCharacterMovementComponent::FSavedMove_Combat::GetCompressedFlags() const
{
	uint8 Result = Super::GetCompressedFlags();
	if (Saved_bWantsToSprint)
	{
		Result |= FLAG_Custom_0;
	}
	if (Saved_bWantsToJetPack)
	{
		Result |= FLAG_Custom_1;
	}
	return Result;
}

//////////////////////////////////////////////////////////////////////
//
//
//SET MOVE FOR
//
//
/////////////////////////////////////////////////////////////////////////

void UCombatCharacterMovementComponent::FSavedMove_Combat::SetMoveFor(ACharacter* C, float InDeltaTime,
                                                                      FVector const& NewAccel,
                                                                      FNetworkPredictionData_Client_Character&
                                                                      ClientData)
{
	FSavedMove_Character::SetMoveFor(C, InDeltaTime, NewAccel, ClientData);

	UCombatCharacterMovementComponent* CharacterMovementComponent = Cast<UCombatCharacterMovementComponent>(
		C->GetCharacterMovement());

	Saved_bWantsToSprint = CharacterMovementComponent->Safe_bWantsToSprint;
	Saved_bWantsToJetPack = CharacterMovementComponent->Safe_bWantsToJetPack;
}

//////////////////////////////////////////////////////////////////////
//
//
//PREP MOVE FOR
//
//
/////////////////////////////////////////////////////////////////////////
void UCombatCharacterMovementComponent::FSavedMove_Combat::PrepMoveFor(ACharacter* C)
{
	Super::PrepMoveFor(C);
	UCombatCharacterMovementComponent* CharacterMovementComponent = Cast<UCombatCharacterMovementComponent>(
		C->GetCharacterMovement());
	CharacterMovementComponent->Safe_bWantsToSprint = Saved_bWantsToSprint;
	CharacterMovementComponent->Safe_bWantsToJetPack = Saved_bWantsToJetPack;
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
		MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 200;
		MutableThis->ClientPredictionData->NoSmoothNetUpdateDist = 600.0f;
	}
	return ClientPredictionData;
}

void UCombatCharacterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);

	Safe_bWantsToSprint = (Flags & FSavedMove_Character::FLAG_Custom_0) != 0;
	Safe_bWantsToJetPack = (Flags & FSavedMove_Character::FLAG_Custom_1) != 0;
}

void UCombatCharacterMovementComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
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

bool UCombatCharacterMovementComponent::IsMovingOnGround() const
{
	//ADD More CUstom Movement modes which are Ground Based
	return Super::IsMovingOnGround();
}

void UCombatCharacterMovementComponent::UpdateCharacterStateBeforeMovement(float DeltaSeconds)
{
	if (!IsCustomMovementMode(CMOVE_JET) && Safe_bWantsToJetPack)
	{
		EnterJet();
	}
	if (IsCustomMovementMode(CMOVE_JET) && !Safe_bWantsToJetPack)
	{
		ExitJet();
	}
	Super::UpdateCharacterStateBeforeMovement(DeltaSeconds);
}

void UCombatCharacterMovementComponent::PhysCustom(float deltaTime, int32 Iterations)
{
	Super::PhysCustom(deltaTime, Iterations);

	switch (CustomMovementMode)
	{
	case CMOVE_JET:
		PhysJet(deltaTime, Iterations);
		break;

	default:
		UE_LOG(LogTemp, Fatal, TEXT("INVALID MOVEMENT MODE"))
	}
}


void UCombatCharacterMovementComponent::EnterJet()
{
	Safe_bWantsToJetPack = true;


	CombatOwnerCharacter->Server_SetJetPacking(true);

	if (bDash) // If Dashing, SetVelocity for Dash and Enter Jet Movement Mode
	{
		if (Acceleration.Size2D() < 0.1)
		{
			Velocity = FVector::UpVector * JetPack_Impulse;
			//ON Dash no Movement and Activate Jet dash Upward
		}
		else
		{
			//ON Dash Just Dash and Activate Jet
			Velocity = Acceleration.GetSafeNormal2D() * JetPack_Impulse;
			//JetInitial Impulse on PreviousAccel
		}
	}
	SetMovementMode(MOVE_Custom, CMOVE_JET);
}

void UCombatCharacterMovementComponent::ExitJet()
{
	Safe_bWantsToJetPack = false;

	CombatOwnerCharacter->Server_SetJetPacking(false);

	EndJet.Broadcast();

	FQuat NewRotation = FRotationMatrix::MakeFromXZ(UpdatedComponent->GetForwardVector().GetSafeNormal2D(),
	                                                FVector::UpVector).ToQuat();
	FHitResult HitResult;
	SafeMoveUpdatedComponent(FVector::ZeroVector, NewRotation, true, HitResult);


	SetMovementMode(MOVE_Falling);
}

void UCombatCharacterMovementComponent::PhysJet(float DeltaTime, int32 Iterations)
{
	if (DeltaTime < MIN_TICK_TIME)return;
	RestorePreAdditiveRootMotionVelocity();

	FHitResult SurfaceHit;

	const bool bCanBoost = bBoost;
	const bool bCanDash = bDash && !bBoost;
	const bool bCanThrust = bThrust && !bDash && !bBoost;
	const bool bCanHover = !bDash && !bBoost;
	const bool bUseGravity = !bDash && !bBoost;
	if (bCanDash)
	{
		if (Acceleration.Size2D() < 0.1)
		{
			Velocity = FVector::UpVector * JetPack_Impulse;

			//ON Dash no Movement and Activate Jet dash Upward
		}
		else
		{
			//ON Dash Just Dash and Activate Jet
			Velocity = Acceleration.GetSafeNormal2D() * JetPack_Impulse;
			//JetInitial Impulse on PreviousAccel
		}
	}
	//JetPack movement
	if (Acceleration.Length() > 0.001 && bCanHover)
	{
		Velocity += JetPack_MaxSpeed * Acceleration.GetSafeNormal2D() * DeltaTime;
	}
	//Jetpack Gravity
	if (bUseGravity)
	{
		Velocity += JetPack_Gravity * FVector::DownVector * DeltaTime;
	}
	if (bCanThrust)
	{
		Velocity += JetPack_MaxSpeed * JetThrustUpCoeff * FVector::UpVector * DeltaTime;
	}
	//JetPackHover Acceleration Read
	//Jetpack Steer
	//JetPack Left Right
	if (!bBoost)
	{
		if (FMath::Abs(FVector::DotProduct(Acceleration.GetSafeNormal2D(), UpdatedComponent->GetRightVector())) > 0.5)
		{
			Acceleration = Acceleration.ProjectOnTo(UpdatedComponent->GetRightVector());
		}
		//Jetpack Move Forward
		if (FMath::Abs(FVector::DotProduct(Acceleration.GetSafeNormal2D(), UpdatedComponent->GetForwardVector())) > 0.5)
		{
			Acceleration = Acceleration.ProjectOnTo(UpdatedComponent->GetForwardVector());
		}
	}
	if (bBoost)
	{
		Velocity += JetPack_BoosMaxSpeed * CombatOwnerCharacter->GetCameraComponent()->GetForwardVector() * DeltaTime;
		if (FMath::Abs(FVector::DotProduct(Acceleration.GetSafeNormal2D(), UpdatedComponent->GetRightVector())) > 0.5)
		{
			Acceleration = Acceleration.ProjectOnTo(UpdatedComponent->GetRightVector());
		}
		if (FMath::Abs(FVector::DotProduct(Acceleration.GetSafeNormal2D(), UpdatedComponent->GetForwardVector())) > 0.5)
		{
			Acceleration = FVector::ZeroVector;
		}
	}


	//Calc Velocity
	if (!HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity())
	{
		CalcVelocity(DeltaTime, JetPack_Friction, true, JetPack_BrakingDeceleration);
	}
	ApplyRootMotionToVelocity(DeltaTime);

	//PerformMove
	Iterations++;
	bJustTeleported = false;

	FVector OldLocation = UpdatedComponent->GetComponentLocation();
	FQuat OldRotation = UpdatedComponent->GetComponentRotation().Quaternion();
	FHitResult HitResult(1.f);
	FVector Adjusted = Velocity * DeltaTime;
	FVector VelPlaneDir = FVector::VectorPlaneProject(Velocity, SurfaceHit.Normal).GetSafeNormal();
	FQuat NewRotation = FRotationMatrix::MakeFromXZ(VelPlaneDir, SurfaceHit.Normal).ToQuat();

	SafeMoveUpdatedComponent(Adjusted, OldRotation, true, HitResult);

	if (HitResult.Time < 1.0f)
	{
		HandleImpact(HitResult, DeltaTime, Adjusted);
		SlideAlongSurface(Adjusted, (1.0f - HitResult.Time), HitResult.Normal, HitResult, true);
	}

	//Update OutGoingVel and Accelleration
	if (!bJustTeleported && !HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity())
	{
		Velocity = (UpdatedComponent->GetComponentLocation() - OldLocation) / DeltaTime;
	}
}

bool UCombatCharacterMovementComponent::GetJetSurface(FHitResult& Hit)
{
	FVector Start = UpdatedComponent->GetComponentLocation();
	FVector End = Start + FVector::DownVector * MaxJetAltitude;
	FName ProfileName = TEXT("BlockAll");

	auto result = GetWorld()->LineTraceSingleByProfile(Hit, Start, End, ProfileName,
	                                                   CombatOwnerCharacter->GetIgnoreCharacterParams());

	// Surface Check for Jet Conditions
	/*DrawDebugLine(GetWorld(), Hit.TraceStart, Hit.TraceEnd, FColor::Magenta, true, 5.0f, 0, 5);*/
	return result;
}

void UCombatCharacterMovementComponent::SprintPressed()
{
	Safe_bWantsToSprint = true;
}

void UCombatCharacterMovementComponent::SprintReleased()
{
	Safe_bWantsToSprint = false;
}

void UCombatCharacterMovementComponent::JetPressed()
{
	Safe_bWantsToJetPack = true;
}

void UCombatCharacterMovementComponent::JetReleased()
{
	Safe_bWantsToJetPack = false;
}

bool UCombatCharacterMovementComponent::IsCustomMovementMode(ECombatCustomMovementMode InCustomMovementMode)
{
	return MovementMode == MOVE_Custom && CustomMovementMode == InCustomMovementMode;
}
