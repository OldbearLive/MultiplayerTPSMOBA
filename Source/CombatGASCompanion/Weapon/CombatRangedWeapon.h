// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "CombatRangedWeapon.generated.h"

UENUM(BlueprintType)
enum class ERangedWeaponStates:uint8
{
	ERWS_Initial UMETA(DisplayName = "InitialState"),
	ERWS_Equipped UMETA(DisplayName = "EquippedState"),
	ERWS_Dropped UMETA(DisplayName = "DroppedState"),

	ERWS_MAX UMETA(DisplayName = "Default_MAX")
};

UCLASS()
class COMBATGASCOMPANION_API ACombatRangedWeapon : public AActor
{
	GENERATED_BODY()

public:
	ACombatRangedWeapon();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void ShowPickupWidget(bool bShowWidget);


protected:
	virtual void BeginPlay() override;

	// OVERLAP FUNCTIONS FOR PICKUP WIDGET
	UFUNCTION()
	virtual void OnSphereOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void OnSphereEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);
	///END OVERLAP FUNCTION SECTION


private:
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	class USphereComponent* AreaSphere;

	
	
	UPROPERTY(ReplicatedUsing = OnRep_WeaponStates, VisibleAnywhere, Category = "Weapon Properties")
	ERangedWeaponStates WeaponStates;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	class UWidgetComponent* PickupWidget;

	UFUNCTION()
	void OnRep_WeaponStates();

	

public:
	void SetWeaponState(ERangedWeaponStates State);

	
	FORCEINLINE USphereComponent* GetSphereComponent() const { return AreaSphere; }
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; }
};
