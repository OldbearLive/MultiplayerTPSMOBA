// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputActionValue.h"
#include "CombatGASCompanion/CombatComponents/RangedCombatComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ACombatCharacter::ACombatCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->TargetArmLength = 600.0f;
	CameraBoom->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Follow Camera"));
	CameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(RootComponent);

	CombatComponent = CreateDefaultSubobject<URangedCombatComponent>(TEXT("CombatComponent"));
	CombatComponent->SetIsReplicated(true);

	TurnInPlace = ETurningInPlace::ETIP_NotTurning;

	NetUpdateFrequency = 66.0f;
	MinNetUpdateFrequency = 33.0f;
}


//ReplicatedVariables
void ACombatCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ACombatCharacter, OverlappingWeapon, COND_OwnerOnly);
}

void ACombatCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (CombatComponent)
	{
		CombatComponent->Character = this;
	}
}

// Called to bind functionality to input
void ACombatCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(Move, ETriggerEvent::Triggered, this, &ACombatCharacter::FMove);
		EnhancedInputComponent->BindAction(Look, ETriggerEvent::Triggered, this, &ACombatCharacter::FLook);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACombatCharacter::Jump);
		EnhancedInputComponent->BindAction(Equip, ETriggerEvent::Triggered, this, &ACombatCharacter::FEquip);
		EnhancedInputComponent->BindAction(Aim, ETriggerEvent::Started, this, &ACombatCharacter::FAimPressed);
		EnhancedInputComponent->BindAction(Aim, ETriggerEvent::Completed, this, &ACombatCharacter::FAimReleased);
		EnhancedInputComponent->BindAction(Fire, ETriggerEvent::Started, this, &ACombatCharacter::FFirePressed);
		EnhancedInputComponent->BindAction(Fire, ETriggerEvent::Completed, this, &ACombatCharacter::FFireReleased);
	}
}



// Called every frame
void ACombatCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AimOffset(DeltaTime);
}

// Called when the game starts or when spawned
void ACombatCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PilotInputMappingContext, 0);
		}
	}
}


void ACombatCharacter::FMove(const FInputActionValue& Value)
{
	const FVector2d MovementValue = Value.Get<FVector2d>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
	const FVector ForwardDirection(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X));
	AddMovementInput(ForwardDirection, MovementValue.Y);
	const FVector RightDirection(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y));
	AddMovementInput(RightDirection, MovementValue.X);
}

void ACombatCharacter::FLook(const FInputActionValue& Value)
{
	const FVector2d LookAxisVector = Value.Get<FVector2d>();
	AddControllerPitchInput(LookAxisVector.Y);
	AddControllerYawInput(LookAxisVector.X);
}


void ACombatCharacter::Jump()
{
	Super::Jump();
	GetCharacterMovement()->bNotifyApex = true;
}

void ACombatCharacter::PlayFireMontage(bool bIsAiming)
{
	if(CombatComponent == nullptr|| CombatComponent->EquippedWeapon == nullptr) return;

	UAnimInstance*AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && FireWeaponMontage)
	{
		AnimInstance->Montage_Play(FireWeaponMontage);
		FName SectionName;
		SectionName = bIsAiming? FName("RifleHip"):FName("RifleAim");
		AnimInstance->Montage_JumpToSection(SectionName);
	}
		
}


void ACombatCharacter::FEquip()
{
	if (CombatComponent)
	{
		if (HasAuthority())
		{
		CombatComponent->EquipWeapon(OverlappingWeapon);
		}
		else
		{
			ServerEquipButtonPressed();
		}
	}
}


void ACombatCharacter::FAimPressed()
{
	if(CombatComponent)
	{
		CombatComponent->SetAiming(true);
	}
}

void ACombatCharacter::FAimReleased()
{
	if(CombatComponent)
	{
		CombatComponent->SetAiming(false);
	}
}

void ACombatCharacter::FFirePressed()
{

	if(CombatComponent)
	{
		CombatComponent->FireButtonPressed(true);
	}
}

void ACombatCharacter::FFireReleased()
{
	if(CombatComponent)
	{
		CombatComponent->FireButtonPressed(false);
	}
	
}


void ACombatCharacter::ServerEquipButtonPressed_Implementation()
{
	if(CombatComponent)
	{
		CombatComponent->EquipWeapon(OverlappingWeapon);
	}
}




void ACombatCharacter::SetOverlappingWeapon(ACombatRangedWeapon* Weapon)
{
	if (OverlappingWeapon)
	{
		OverlappingWeapon->ShowPickupWidget(false);
	}
	OverlappingWeapon = Weapon;

	if (IsLocallyControlled())
	{
		if (OverlappingWeapon)
		{
			OverlappingWeapon->ShowPickupWidget(true);
		}
	}
}

bool ACombatCharacter::IsWeaponEquipped()
{
	return(CombatComponent&&CombatComponent->EquippedWeapon);
}

bool ACombatCharacter::IsAiming()
{
	return (CombatComponent&&CombatComponent->bAiming);
}

ACombatRangedWeapon* ACombatCharacter::GetEquippedWeapon()
{
	if(CombatComponent == nullptr) return nullptr;

	return CombatComponent->EquippedWeapon;
}

void ACombatCharacter::AimOffset(float DeltaTime)
{

	if(CombatComponent== nullptr)return;
	FVector Velocity = GetVelocity();
	Velocity.Z = 0.f;
	float Speed = Velocity.Size();

	bool bIsInAir = GetCharacterMovement()->IsFalling();

	if(Speed==0.f &&!bIsInAir)//Standing Still and not jumping
	{
		
		FRotator CurrentAimRotation = FRotator(0.f,GetBaseAimRotation().Yaw,0.f);
		FRotator DeltaAimRotation = UKismetMathLibrary::NormalizedDeltaRotator(CurrentAimRotation,StartingAimRotation);

		AO_Yaw = DeltaAimRotation.Yaw;

		if(TurnInPlace == ETurningInPlace::ETIP_NotTurning)
		{
			InterpAO_Yaw=AO_Yaw;
		}
		bUseControllerRotationYaw = true;
		FTurnInPlace(DeltaTime);
		
	}
	if(Speed>0.f || bIsInAir)
	{
		StartingAimRotation = FRotator(0.f,GetBaseAimRotation().Yaw,0.f);
		AO_Yaw = 0.f;
		bUseControllerRotationYaw = true;
		TurnInPlace = ETurningInPlace::ETIP_NotTurning;
	}

	AO_Pitch = GetBaseAimRotation().Pitch;
	if(AO_Pitch>90.f && !IsLocallyControlled())
	{
		FVector2d InRange(270.f,360.f);
		FVector2d OutRange(-90.f,0.f);
		AO_Pitch= FMath::GetMappedRangeValueClamped(InRange,OutRange,AO_Pitch);
	}
	
	
}

void ACombatCharacter::FTurnInPlace(float DeltaTime)
{	
	if(AO_Yaw>90.f)
	{
		TurnInPlace = ETurningInPlace::ETIP_Right;
		
	}
	if(AO_Yaw<-90.f)
	{
		TurnInPlace = ETurningInPlace::ETIP_Left;
	}
	if(TurnInPlace != ETurningInPlace::ETIP_NotTurning)
	{
		InterpAO_Yaw = FMath::FInterpTo(InterpAO_Yaw,0.0f,DeltaTime,3.0f);
		AO_Yaw = InterpAO_Yaw;
		if(FMath::Abs(AO_Yaw)<15.0f)
		{
			TurnInPlace = ETurningInPlace::ETIP_NotTurning;
			StartingAimRotation = FRotator(0.f,GetBaseAimRotation().Yaw,0.f);
		}
	}
}

void ACombatCharacter::OnRep_OverlappingWeapon(ACombatRangedWeapon* LastWeapon)
{
	if (OverlappingWeapon)
	{
		OverlappingWeapon->ShowPickupWidget(true);
	}

	if (LastWeapon)
	{
		LastWeapon->ShowPickupWidget(false);
	}
}




