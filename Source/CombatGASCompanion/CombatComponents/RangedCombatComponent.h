// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatGASCompanion/HUD/CombatHUD.h"
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





protected:
	// Called when the game starts
	virtual void BeginPlay() override;


	

	void TraceUnderCrosshairs(FHitResult& TraceHitResult);

	void SetHUDCrosshairs(float DeltaTime);

private:
	class ACombatCharacter* Character;

	class ACombatPlayerController* PlayerController;

	class ACombatHUD* HUD;
	

	UPROPERTY(Replicated)
	bool bAiming;

	UPROPERTY(EditAnywhere)
	float BaseWalkSpeed;
	UPROPERTY(EditAnywhere)
	float AimWalkSpeed;


	bool bFireButtonPressed;

	//CROSSHAIR STUFF

	FHUDPackage HUDPackage;

	float CrosshairVelocityFactor;
	float CrosshairInAirFactor;
	float CrosshairAimFactor;
	float CrosshairShootingFactor;
	FVector CameraSocketOffset;

	FVector HitTarget;

	/*
	 *Zooming and Aiming
	 */

	//DefaultZoom Set in BeginPlay
	float DefaultFOV;

	FVector DefaultCameraSocketOffset;
	//CurrentFOV Set in BeginPlay
	float CurrentFOV;

	FVector CurrentCameraSocketOffset;
	//ZoomFOV
	

	void InterpFOV(float DeltaTime);
};
