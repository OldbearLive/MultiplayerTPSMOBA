// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CombatGASCompanion/CombatTypes/TurnInPlace.h"
#include "CombatAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class COMBATGASCOMPANION_API UCombatAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	//virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;
	

protected:


private:
	UPROPERTY(BlueprintReadOnly, Category="Character", meta = (AllowPrivateAccess="true"))
	class ACombatCharacter* CombatCharacter;

	UPROPERTY(BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess="true"))
	float Speed;

	UPROPERTY(BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess="true"))
	float Direction;

	UPROPERTY(BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess="true"))
	bool bIsInAir;

	UPROPERTY(BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess="true"))
	bool bIsAccelerating;

	UPROPERTY(BlueprintReadOnly, Category="Weapon", meta = (AllowPrivateAccess="true"))
	bool bWeaponEquipped;

	UPROPERTY(BlueprintReadOnly, Category="Weapon", meta = (AllowPrivateAccess="true"))
	class ACombatRangedWeapon* EquippedWeapon;

	UPROPERTY(BlueprintReadOnly, Category="Weapon", meta = (AllowPrivateAccess="true"))
	int32 SelectedWeaponIndex;

	UPROPERTY(BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess="true"))
	bool bAiming;

	UPROPERTY(BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess="true"))
	float YawOffset;

	UPROPERTY(BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess="true"))
	float Lean;

	UPROPERTY(BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess="true"))
	float AO_Yaw;

	UPROPERTY(BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess="true"))
	float AO_Pitch;

	UPROPERTY(BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess="true"))
	bool ApexReached;

	UPROPERTY(BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess="true"))
	FTransform LeftHandTransform;


	UPROPERTY(BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess="true"))
	ETurningInPlace TurnInPlace;


	UPROPERTY(BlueprintReadOnly, Category="Weapon", meta = (AllowPrivateAccess="true"))
	FRotator RightHandRotation;

	UPROPERTY(BlueprintReadOnly, Category="Weapon", meta = (AllowPrivateAccess="true"))
	bool bIsLocallyControlled;

	FRotator CharacterRotationLastFrame;
	FRotator CharacterRotation;

	UPROPERTY(BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess="true"))
	FRotator DeltaRotation;

	UPROPERTY(BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess="true"))
	bool bRotateRootBone;
};
