// Fill out your copyright notice in the Description page of Project Settings.


#include "RangedCombatComponent.h"

#include "CombatGASCompanion/Character/CombatCharacter.h"
#include "CombatGASCompanion/PlayerController/CombatPlayerController.h"
#include "CombatGASCompanion/Weapon/CombatRangedWeapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
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
	if(Character&& Character->IsLocallyControlled())
	{
	FHitResult HitResult;
	TraceUnderCrosshairs(HitResult);
	HitTarget = HitResult.ImpactPoint;
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
	}
	// ...
}

void URangedCombatComponent::SetHUDCrosshairs(float DeltaTime)
{
	if (Character == nullptr || Character->Controller == nullptr)return;
	PlayerController = PlayerController == nullptr
		                   ? Cast<ACombatPlayerController>(Character->Controller)
		                   : PlayerController;

	if (PlayerController)
	{
		HUD = HUD == nullptr ? Cast<ACombatHUD>(PlayerController->GetHUD()) : HUD;
		if (HUD)
		{
			FHUDPackage HUDPackage;
			if (EquippedWeapon)
			{
				HUDPackage.CrosshairsCenter = EquippedWeapon->CrosshairsCenter;
				HUDPackage.CrosshairsTop = EquippedWeapon->CrosshairsTop;
				HUDPackage.CrosshairsBottom = EquippedWeapon->CrosshairsBottom;
				HUDPackage.CrosshairsLeft = EquippedWeapon->CrosshairsLeft;
				HUDPackage.CrosshairsRight = EquippedWeapon->CrosshairsRight;
			}
			else
			{
				HUDPackage.CrosshairsCenter = nullptr;
				HUDPackage.CrosshairsTop = nullptr;
				HUDPackage.CrosshairsBottom = nullptr;
				HUDPackage.CrosshairsLeft = nullptr;
				HUDPackage.CrosshairsRight = nullptr;
			}
			//DYNAMIC CROSSHAIR SPREAD
			FVector2d WalkSpeedRange (0.f,Character->GetCharacterMovement()->MaxWalkSpeed);
			FVector2d VelocityMultiplierRange (0.f,1.f);

			FVector Velocity = Character->GetVelocity();
			Velocity.Z = 0.f;
			
			CrosshairVelocityFactor = FMath::GetMappedRangeValueClamped(WalkSpeedRange,VelocityMultiplierRange,Velocity.Size());

			if(Character->GetCharacterMovement()->IsFalling())
			{
				CrosshairInAirFactor = FMath::FInterpTo(CrosshairInAirFactor,2.25f,DeltaTime,2.25f);
			}
			else
			{
				CrosshairInAirFactor = FMath::FInterpTo(CrosshairInAirFactor,0.f,DeltaTime,20.f);
			}
		
			HUDPackage.CrosshairSpreadX = CrosshairVelocityFactor+ CrosshairInAirFactor;
			HUDPackage.CrosshairSpreadY = CrosshairVelocityFactor+ CrosshairInAirFactor;
			
			HUD->SetHUDPackage(HUDPackage);
		}
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
		FVector End = Start + (CrosshairWorldDirection * TRACE_LENGTH);

		GetWorld()->LineTraceSingleByChannel(TraceHitResult, Start, End, ECollisionChannel::ECC_Visibility);

		if (!TraceHitResult.bBlockingHit)
		{
			TraceHitResult.ImpactPoint = End;
		}
		else
		{
			DrawDebugSphere(GetWorld(), TraceHitResult.ImpactPoint, 12.f, 12, FColor::Red);
		}
	}
}


void URangedCombatComponent::Server_Fire_Implementation(const FVector_NetQuantize& HitResult)
{
	MultiCast_Fire(HitResult);
}

void URangedCombatComponent::MultiCast_Fire_Implementation(const FVector_NetQuantize& HitResult)
{
	if (EquippedWeapon == nullptr)return;
	if (Character)
	{
		Character->PlayFireMontage(bAiming);

		EquippedWeapon->Fire(HitResult);
	}
}


void URangedCombatComponent::EquipWeapon(ACombatRangedWeapon* WeaponToEquip)
{
	if (Character == nullptr || WeaponToEquip == nullptr)
		return;
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
