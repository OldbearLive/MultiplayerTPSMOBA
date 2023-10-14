// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatGASCompanion/Interfaces/CombatInterface.h"
#include "GameFramework/Actor.h"
#include "CombatRangedWeapon.generated.h"


class UGameplayAbility;
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
USTRUCT(BlueprintType)
struct FWeaponProperties
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText DisplayName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsAutoFire = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsShotgun = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float FireRate = 1.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Ammo_Clip = 10;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Ammo_Stock= 100;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UGameplayAbility> WeaponPrimary = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UGameplayAbility> WeaponSecondary = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UAnimMontage* FireMontage = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UAnimMontage* EquipMontage = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UAnimMontage* ReloadMontage = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UAnimSequence* WeaponFireMontage = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName InventorySlot;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UAnimInstance> WeaponAnimInstance;
};


UCLASS()
class COMBATGASCOMPANION_API ACombatRangedWeapon : public AActor, public ICombatInterface
{
	GENERATED_BODY()

public:
	ACombatRangedWeapon();

	// Called every frame
	virtual void Tick(float DeltaTime) override;


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	/*
    *WEAPON FIRE ANIMATION
    */
	UFUNCTION(BlueprintCallable)
	virtual void Fire();


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

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly,Replicated, Category = "Weapon Properties")
	FWeaponProperties WeaponPropertiesStructure;

protected:
	virtual void BeginPlay() override;


	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	FName WeaponTipSocket;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Weapon Properties")
	USceneComponent* Root;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Weapon Properties")
	USkeletalMeshComponent* WeaponMesh;

	FVector GetCombatSocketLocation_Implementation() override;


public:
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; }
};
