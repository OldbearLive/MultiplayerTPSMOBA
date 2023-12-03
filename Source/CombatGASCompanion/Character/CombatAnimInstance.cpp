// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatAnimInstance.h"

#include "CombatCharacter.h"
#include "AnimGraphRuntime/Public/KismetAnimationLibrary.h"
#include "CombatGASCompanion/Weapon/CombatRangedWeapon.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

void UCombatAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	CombatCharacter = Cast<ACombatCharacter>(TryGetPawnOwner());
}

void UCombatAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	if (CombatCharacter == nullptr)
	{
		CombatCharacter = Cast<ACombatCharacter>(TryGetPawnOwner());
	}
	if (CombatCharacter == nullptr)
	{
		return;
	}

	FVector Velocity = CombatCharacter->GetVelocity();
	Velocity.Z = 0.f;
	Speed = Velocity.Size();


	//Direction = UKismetAnimationLibrary::CalculateDirection(Velocity,CombatCharacter->GetActorRotation());

	bIsInAir = CombatCharacter->CombatCharacterMovementComponent->IsFalling() || CombatCharacter->
		CombatCharacterMovementComponent->IsCustomMovementMode(CMOVE_JET);

	bIsAccelerating = CombatCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size2D() > 0.f ? true : false;

	bWeaponEquipped = CombatCharacter->GetWeaponEquip();
	bJet = CombatCharacter->bIsJet;
	bDash = CombatCharacter->bIsDash;
	bBoost = CombatCharacter->bIsBoost;

	SelectedWeaponIndex = CombatCharacter->ActiveWeaponIndex;

	//OFfset Yaw for Strafing
	FRotator AimRotation = CombatCharacter->GetBaseAimRotation();
	FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(CombatCharacter->GetVelocity());
	FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation);
	DeltaRotation = FMath::RInterpTo(DeltaRotation, DeltaRot, DeltaSeconds, 15.f);
	YawOffset = DeltaRotation.Yaw;

	Direction = UKismetAnimationLibrary::CalculateDirection(Velocity, AimRotation);

	CharacterRotationLastFrame = CharacterRotation;
	CharacterRotation = CombatCharacter->GetActorRotation();
	const FRotator RotationDelta = UKismetMathLibrary::NormalizedDeltaRotator(
		CharacterRotation, CharacterRotationLastFrame);
	const float Target = RotationDelta.Yaw / DeltaSeconds;
	const float Interp = FMath::FInterpTo(Lean, Target, DeltaSeconds, 15.0f);
	Lean = FMath::Clamp(Interp, -90.0f, 90.0f);

	ApexReached = CombatCharacter->GetCharacterMovement()->bNotifyApex;
	AO_Yaw = CombatCharacter->GetAO_Yaw();
	AO_Pitch = CombatCharacter->GetAO_Pitch();
	TurnInPlace = CombatCharacter->GetTurningInPlace();
	bRotateRootBone = CombatCharacter->ShouldRotateRootBone();

	EquippedWeapon = CombatCharacter->GetCurrentWeapon();
	if (EquippedWeapon && bWeaponEquipped && CombatCharacter->GetMesh())
	{
		LeftHandTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(
			FName("LeftHandSocket"), RTS_World);

		FVector OutPosition;
		FRotator OutRotation;
		CombatCharacter->GetMesh()->TransformToBoneSpace(FName("hand_r"), LeftHandTransform.GetLocation(),
		                                                 FRotator::ZeroRotator, OutPosition, OutRotation);
		LeftHandTransform.SetLocation(OutPosition);
		LeftHandTransform.SetRotation(FQuat(OutRotation));

		if (CombatCharacter->IsLocallyControlled())
		{
			bIsLocallyControlled = true;
			FTransform RightHandTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(
				FName("hand_r"), RTS_World);

			FRotator LookatRotation = UKismetMathLibrary::FindLookAtRotation(RightHandTransform.GetLocation(),
			                                                                 RightHandTransform.GetLocation() + (
				                                                                 RightHandTransform.GetLocation() -
				                                                                 CombatCharacter->GetHitTarget()));

			RightHandRotation = UKismetMathLibrary::RInterpTo(RightHandRotation, LookatRotation, DeltaSeconds, 20.f);
			/*//DEBUG LINES TO SEE WEaponROtation

			FTransform MuzzleTipTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(
				FName("MuzzleFlash"), ERelativeTransformSpace::RTS_World);

			FVector MuzzleX(FRotationMatrix(MuzzleTipTransform.GetRotation().Rotator()).GetUnitAxis(EAxis::X));
			DrawDebugLine(GetWorld(), MuzzleTipTransform.GetLocation(),
			              MuzzleTipTransform.GetLocation() + MuzzleX * 10000,
			              FColor::Red);

			DrawDebugLine(GetWorld(), MuzzleTipTransform.GetLocation(), CombatCharacter->GetHitTarget(), FColor::Blue);*/
		}
	}
}


;

/*
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (CombatCharacter == nullptr)
	{
		CombatCharacter = Cast<ACombatCharacter>(TryGetPawnOwner());
	}
	if (CombatCharacter == nullptr)return;

	FVector Velocity = CombatCharacter->GetVelocity();
	Velocity.Z = 0.f;
	Speed = Velocity.Size();

	bIsInAir = CombatCharacter->GetCharacterMovement()->IsFalling();

	bIsAccelerating = CombatCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true : false;

	bWeaponEquipped = CombatCharacter->IsWeaponEquipped();

	bAiming = CombatCharacter->IsAiming();


	//OFfset Yaw for Strafing
	FRotator AimRotation = CombatCharacter->GetBaseAimRotation();
	FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(CombatCharacter->GetVelocity());
	FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation,AimRotation);
	DeltaRotation =FMath::RInterpTo(DeltaRotation,DeltaRot,DeltaSeconds,15.f);
	YawOffset= DeltaRotation.Yaw;
	

	CharacterRotationLastFrame =CharacterRotation;
	CharacterRotation = CombatCharacter->GetActorRotation();
	const FRotator RotationDelta = UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation,CharacterRotationLastFrame);
	const float Target = RotationDelta.Yaw/DeltaSeconds;
	const float Interp = FMath::FInterpTo(Lean,Target,DeltaSeconds,6.0f);
	Lean = FMath::Clamp(Interp,-90.0f,90.0f);
	
}*/
