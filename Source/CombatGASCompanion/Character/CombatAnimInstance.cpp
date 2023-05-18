// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatAnimInstance.h"

#include "CombatCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UCombatAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	CombatCharacter = Cast<ACombatCharacter>(TryGetPawnOwner());
}

void UCombatAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
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
}
