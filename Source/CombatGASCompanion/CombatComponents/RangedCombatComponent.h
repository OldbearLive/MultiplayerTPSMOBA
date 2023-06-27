// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RangedCombatComponent.generated.h"

#define TRACE_LENGTH  80000.f

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COMBATGASCOMPANION_API URangedCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URangedCombatComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	friend class ACombatCharacter;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


public:
	void EquipWeapon(class ACombatRangedWeapon*WeaponToEquip);


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	
	void SetAiming(bool bIsAiming);

	UFUNCTION(Server,Reliable)
	void Server_SetAiming(bool bIsAiming);

	UFUNCTION()
	void OnRep_EquippedWeapon();

	void FireButtonPressed(bool bPressed);

	UFUNCTION(Server,Reliable)
	void Server_Fire( const FVector_NetQuantize& HitResult);

	UFUNCTION(NetMulticast,Reliable)
	void MultiCast_Fire(const FVector_NetQuantize& HitResult);

	void TraceUnderCrosshairs(FHitResult& TraceHitResult);
	
private:
	class ACombatCharacter* Character;

	UPROPERTY(ReplicatedUsing = OnRep_EquippedWeapon)
	ACombatRangedWeapon* EquippedWeapon;

	UPROPERTY(Replicated)
	bool bAiming;

	UPROPERTY(EditAnywhere)
	float BaseWalkSpeed;
	UPROPERTY(EditAnywhere)
	float AimWalkSpeed;


	bool bFireButtonPressed;

	FVector HitTarget;
};
