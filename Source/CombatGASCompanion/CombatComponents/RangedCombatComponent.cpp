// Fill out your copyright notice in the Description page of Project Settings.


#include "RangedCombatComponent.h"

#include "Camera/CameraComponent.h"
#include "CombatGASCompanion/Character/CombatCharacter.h"
#include "CombatGASCompanion/PlayerController/CombatPlayerController.h"
#include "CombatGASCompanion/Weapon/CombatRangedWeapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
URangedCombatComponent::URangedCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	BaseWalkSpeed = 600.0f;
	AimWalkSpeed = 300.0f;

	// ...
}


// Called every frame
void URangedCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	SetHUDCrosshairs(DeltaTime);
	if (Character && Character->IsLocallyControlled())
	{
		FHitResult HitResult;
		TraceUnderCrosshairs(HitResult);
		HitTarget = HitResult.ImpactPoint;
		if(EquippedWeapon)
		{
		InterpFOV(DeltaTime);
		}
	}
	// ...
}

void URangedCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(URangedCombatComponent, EquippedWeapon);
	DOREPLIFETIME(URangedCombatComponent, bAiming);
}

// Called when the game starts
void URangedCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	if (Character)
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;

		if (Character->GetCameraComponent() && Character->GetCameraBoom())
		{
			DefaultFOV = Character->GetCameraComponent()->FieldOfView;
			CurrentFOV = DefaultFOV;
			DefaultCameraSocketOffset = Character->GetCameraBoom()->SocketOffset;
			CurrentCameraSocketOffset = DefaultCameraSocketOffset;
		}
	}
	// ...
}

void URangedCombatComponent::SetHUDCrosshairs(float DeltaTime)
{
	if (Character == nullptr || Character->Controller == nullptr)
	{
		return;
	}
	PlayerController = PlayerController == nullptr
		                   ? Cast<ACombatPlayerController>(Character->Controller)
		                   : PlayerController;

	if (PlayerController)
	{
		HUD = HUD == nullptr ? Cast<ACombatHUD>(PlayerController->GetHUD()) : HUD;
		if (HUD)
		{
			float WeaponSpreadX;
			float WeaponSpreadY;
			float CrosshairAim;
			if (EquippedWeapon)
			{
				HUDPackage.CrosshairsCenter = EquippedWeapon->CrosshairsCenter;
				HUDPackage.CrosshairsTop = EquippedWeapon->CrosshairsTop;
				HUDPackage.CrosshairsBottom = EquippedWeapon->CrosshairsBottom;
				HUDPackage.CrosshairsLeft = EquippedWeapon->CrosshairsLeft;
				HUDPackage.CrosshairsRight = EquippedWeapon->CrosshairsRight;
				WeaponSpreadX = EquippedWeapon->CrosshairSpreadX;
				WeaponSpreadY = EquippedWeapon->CrosshairSpreadY;
				CrosshairAim = EquippedWeapon->CrosshairAim;
			}
			else
			{
				HUDPackage.CrosshairsCenter = Character->DefaultCrosshair;
				HUDPackage.CrosshairsTop = nullptr;
				HUDPackage.CrosshairsBottom = nullptr;
				HUDPackage.CrosshairsLeft = nullptr;
				HUDPackage.CrosshairsRight = nullptr;
				WeaponSpreadX = 1.f;
				WeaponSpreadY = 1.f;
				CrosshairAim = 1.f;
			}
			//DYNAMIC CROSSHAIR SPREAD
			FVector2d WalkSpeedRange(0.f, Character->GetCharacterMovement()->MaxWalkSpeed);
			FVector2d VelocityMultiplierRange(0.f, 1.f);

			FVector Velocity = Character->GetVelocity();
			Velocity.Z = 0.f;

			CrosshairVelocityFactor = FMath::GetMappedRangeValueClamped(WalkSpeedRange, VelocityMultiplierRange,
			                                                            Velocity.Size());

			if (Character->GetCharacterMovement()->IsFalling())
			{
				CrosshairInAirFactor = FMath::FInterpTo(CrosshairInAirFactor, 2.25f, DeltaTime, 2.25f);
			}
			else
			{
				CrosshairInAirFactor = FMath::FInterpTo(CrosshairInAirFactor, 0.f, DeltaTime, 20.f);
			}

			if (bAiming)
			{
				CrosshairAimFactor = FMath::FInterpTo(CrosshairAimFactor, CrosshairAim, DeltaTime, 30.f);
			}
			else
			{
				CrosshairAimFactor = FMath::FInterpTo(CrosshairAimFactor, 0.f, DeltaTime, 30.f);
			}

			CrosshairShootingFactor = FMath::FInterpTo(CrosshairShootingFactor, 0.f, DeltaTime, 10.f);

			HUDPackage.CrosshairSpreadX = WeaponSpreadX + CrosshairVelocityFactor +
				CrosshairInAirFactor - CrosshairAimFactor + CrosshairShootingFactor;
			HUDPackage.CrosshairSpreadY = WeaponSpreadY + CrosshairVelocityFactor +
				CrosshairInAirFactor - CrosshairAimFactor + CrosshairShootingFactor;

			HUD->SetHUDPackage(HUDPackage);
		}
	}
}

void URangedCombatComponent::InterpFOV(float DeltaTime)
{
	if (EquippedWeapon == nullptr && Character->GetCameraBoom() == nullptr)
	{
		return;
	}
	if (bAiming)
	{
		CurrentFOV = FMath::FInterpTo(CurrentFOV, EquippedWeapon->GetZoomFOV(), DeltaTime,
		                              EquippedWeapon->GetZoomInterp());
		CurrentCameraSocketOffset = FMath::VInterpTo(Character->GetCameraBoom()->SocketOffset, EquippedWeapon->CameraAimSocketOffset,
		                                             DeltaTime, 10.f);
	}
	else
	{
		CurrentFOV = FMath::FInterpTo(CurrentFOV, DefaultFOV, DeltaTime, 30.f);
		CurrentCameraSocketOffset = FMath::VInterpTo(Character->GetCameraBoom()->SocketOffset, DefaultCameraSocketOffset,
		                                             DeltaTime, 10.f);
	}
	if (Character && Character->GetCameraComponent() && Character->GetCameraBoom()&& EquippedWeapon)
	{
		Character->GetCameraComponent()->SetFieldOfView(CurrentFOV);
		Character->GetCameraBoom()->SocketOffset = CurrentCameraSocketOffset;
	}
}

void URangedCombatComponent::SetAiming(bool bIsAiming)
{
	bAiming = bIsAiming;
	Server_SetAiming(bIsAiming);
	if (Character)
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = bIsAiming ? AimWalkSpeed : BaseWalkSpeed;
	}
}

void URangedCombatComponent::Server_SetAiming_Implementation(bool bIsAiming)
{
	bAiming = bIsAiming;
	if (Character)
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = bIsAiming ? AimWalkSpeed : BaseWalkSpeed;
	}
}

void URangedCombatComponent::OnRep_EquippedWeapon()
{
	if (EquippedWeapon && Character)
	{
		Character->GetCharacterMovement()->bOrientRotationToMovement = false;
		Character->bUseControllerRotationYaw = true;
	}
}

void URangedCombatComponent::FireButtonPressed(bool bPressed)
{
	bFireButtonPressed = bPressed;
	if (bFireButtonPressed)
	{
		FHitResult HitResult;
		TraceUnderCrosshairs(HitResult);
		Server_Fire(HitResult.ImpactPoint);
		if (EquippedWeapon)
		{
			CrosshairShootingFactor = EquippedWeapon->CrosshairShoot;
		}
	}
}


void URangedCombatComponent::TraceUnderCrosshairs(FHitResult& TraceHitResult)
{
	FVector2d ViewportSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}
	FVector2d CrosshairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;

	bool bScreentoWorld = UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(this, 0),
	                                                               CrosshairLocation,
	                                                               CrosshairWorldPosition, CrosshairWorldDirection);

	if (bScreentoWorld)
	{
		FVector Start = CrosshairWorldPosition;
		if(Character)
		{
			float DistanceToCharacter = (Character->GetActorLocation() - Start).Size();
			Start+= CrosshairWorldDirection*(DistanceToCharacter+75.f);
			//ShowDebugSphereForCrosshairTraceStart
			
			//DrawDebugSphere(GetWorld(), Start, 12.f, 12, FColor::Red);
		}
		
		FVector End = Start + (CrosshairWorldDirection * TRACE_LENGTH);
		

		GetWorld()->LineTraceSingleByChannel(TraceHitResult, Start, End, ECC_Visibility);

		if (EquippedWeapon)
		{
			if (TraceHitResult.GetActor() && TraceHitResult.GetActor()->GetClass()->ImplementsInterface(UInteractWithCrosshairsInterface::StaticClass()))
			{
				HUDPackage.CrosshairColor = EquippedWeapon->CrosshairEnemyColor;
			}
			else
			{
				HUDPackage.CrosshairColor = EquippedWeapon->CrosshairDefaultColor;
			}
		}
		else
		{
			HUDPackage.CrosshairColor = Character->CrosshairDefaultColor;
		}
		
		if (!TraceHitResult.bBlockingHit)
		{
			TraceHitResult.ImpactPoint = End;
		}
		/*else
		{
			DrawDebugSphere(GetWorld(), TraceHitResult.ImpactPoint, 12.f, 12, FColor::Red);
		}*/
	}
}


void URangedCombatComponent::Server_Fire_Implementation(const FVector_NetQuantize& HitResult)
{
	MultiCast_Fire(HitResult);
}

void URangedCombatComponent::MultiCast_Fire_Implementation(const FVector_NetQuantize& HitResult)
{
	if (EquippedWeapon == nullptr)
	{
		return;
	}
	if (Character)
	{
		Character->PlayFireMontage(bAiming);

		EquippedWeapon->Fire(HitResult);
	}
}


void URangedCombatComponent::EquipWeapon(ACombatRangedWeapon* WeaponToEquip)
{
	if (Character == nullptr || WeaponToEquip == nullptr)
	{
		return;
	}
	EquippedWeapon = WeaponToEquip;
	EquippedWeapon->SetWeaponState(ERangedWeaponStates::ERWS_Equipped);
	const USkeletalMeshSocket* HandSocket = Character->GetMesh()->GetSocketByName(FName("RightHandSocket"));
	if (HandSocket)
	{
		HandSocket->AttachActor(EquippedWeapon, Character->GetMesh());
	}
	EquippedWeapon->SetOwner(Character);
	Character->GetCharacterMovement()->bOrientRotationToMovement = false;
	Character->bUseControllerRotationYaw = true;
}
