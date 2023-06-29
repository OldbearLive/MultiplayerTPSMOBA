// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "CombatRangedWeapon.generated.h"


/*
 *WEAPON STATES ENUM
 *
 */
UENUM(BlueprintType)
enum class ERangedWeaponStates:uint8
{
	ERWS_Initial UMETA(DisplayName = "InitialState"),
	ERWS_Equipped UMETA(DisplayName = "EquippedState"),
	ERWS_Dropped UMETA(DisplayName = "DroppedState"),

	ERWS_MAX UMETA(DisplayName = "Default_MAX")
};

/*
 *WEAPON STATES ENUM
 *
 */
UCLASS()
class COMBATGASCOMPANION_API ACombatRangedWeapon : public AActor
{
	GENERATED_BODY()

public:
	ACombatRangedWeapon();

	// Called every frame
	virtual void Tick(float DeltaTime) override;


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/*
	 *   PICKUPWIDGET
	*/
	void ShowPickupWidget(bool bShowWidget);


	/*
    *WEAPON FIRE
    */
	virtual void Fire(const FVector& HitTarget);


	/*
	 *   WEAPON CROSSHAIR
	*/
	UPROPERTY(EditAnywhere, Category = "CROSSHAIRS")
	class UTexture2D* CrosshairsCenter;

	UPROPERTY(EditAnywhere, Category = "CROSSHAIRS")
	UTexture2D* CrosshairsTop;

	UPROPERTY(EditAnywhere, Category = "CROSSHAIRS")
	UTexture2D* CrosshairsBottom;

	UPROPERTY(EditAnywhere, Category = "CROSSHAIRS")
	UTexture2D* CrosshairsLeft;

	UPROPERTY(EditAnywhere, Category = "CROSSHAIRS")
	UTexture2D* CrosshairsRight;

	UPROPERTY(EditAnywhere, Category = "CROSSHAIRS")
	float CrosshairSpreadX = 0.5f;

	UPROPERTY(EditAnywhere, Category = "CROSSHAIRS")
	float CrosshairSpreadY = 0.5f;

	UPROPERTY(EditAnywhere, Category = "CROSSHAIRS")
	float CrosshairShoot = 0.5f;

	UPROPERTY(EditAnywhere, Category = "CROSSHAIRS")
	float CrosshairAim = 0.5f;

	UPROPERTY(EditAnywhere, Category = "CROSSHAIRS")
	FVector CameraAimSocketOffset;

	UPROPERTY(EditAnywhere, Category = "CROSSHAIRS")
	FLinearColor CrosshairDefaultColor;

	UPROPERTY(EditAnywhere, Category = "CROSSHAIRS")
	FLinearColor CrosshairEnemyColor;

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


	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	UAnimationAsset* FireAnimation;

	UFUNCTION()
	void OnRep_WeaponStates();

	/*
	 *
	 *ZOOM Params
	 */
	UPROPERTY(EditAnywhere)
	float ZoomedFOV;

	UPROPERTY(EditAnywhere)
	float ZoomInterpSpeed;


public:
	void SetWeaponState(ERangedWeaponStates State);


	FORCEINLINE USphereComponent* GetSphereComponent() const { return AreaSphere; }
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; }
	FORCEINLINE float GetZoomFOV() const { return ZoomedFOV; }
	FORCEINLINE float GetZoomInterp() const { return ZoomInterpSpeed; }
};
