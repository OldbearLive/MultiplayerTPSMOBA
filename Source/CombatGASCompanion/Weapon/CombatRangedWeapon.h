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
 *WEAPON STRUCTS
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


private:
	
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	USkeletalMeshComponent* WeaponMesh;
	
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	UAnimationAsset* FireAnimation;


	

public:
	
	
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; }
	
};
