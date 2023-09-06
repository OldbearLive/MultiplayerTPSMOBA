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


	if (Character && Character->IsLocallyControlled())
	{
		SetHUDCrosshairs(DeltaTime);
		FHitResult HitResult;
		TraceUnderCrosshairs(HitResult);
		HitTarget = HitResult.ImpactPoint;
		
	}
	// ...
}

void URangedCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	
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

			HUDPackage.CrosshairsCenter = Character->DefaultCrosshair;
			HUDPackage.CrosshairsTop = nullptr;
			HUDPackage.CrosshairsBottom = nullptr;
			HUDPackage.CrosshairsLeft = nullptr;
			HUDPackage.CrosshairsRight = nullptr;
			WeaponSpreadX = 1.f;
			WeaponSpreadY = 1.f;
			CrosshairAim = 1.f;
		}

		HUD->SetHUDPackage(HUDPackage);
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
		if (Character)
		{
			float DistanceToCharacter = (Character->GetActorLocation() - Start).Size();
			Start += CrosshairWorldDirection * (DistanceToCharacter + 75.f);
			//ShowDebugSphereForCrosshairTraceStart

			//DrawDebugSphere(GetWorld(), Start, 12.f, 12, FColor::Red);
		}

		FVector End = Start + (CrosshairWorldDirection * TRACE_LENGTH);


		GetWorld()->LineTraceSingleByChannel(TraceHitResult, Start, End, ECC_Visibility);


		HUDPackage.CrosshairColor = Character->CrosshairDefaultColor;


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
