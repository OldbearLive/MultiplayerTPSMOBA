// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "CombatGASCompanion/CombatGASCompanion.h"
#include "CombatGASCompanion/AbilitySystem/CombatAbilitySystemComponent.h"
#include "CombatGASCompanion/CombatComponents/RangedCombatComponent.h"
#include "CombatGASCompanion/PlayerController/CombatPlayerController.h"
#include "CombatGASCompanion/PlayerController/CombatPlayerState.h"

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

	GetMesh()->SetCollisionObjectType(ECC_SkeletalMesh);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

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

	DOREPLIFETIME_CONDITION(ACombatCharacter,bIsWeaponEquipped,COND_None);

	
}

void ACombatCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//Init ASC on Server
	InitAbilityActorInfo();

	//Give StartupAbilites
	AddCharacterAbilities();
}

void ACombatCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	//Init ASC on Client
	InitAbilityActorInfo();
}

int32 ACombatCharacter::GetPlayerLevel()
{
	ACombatPlayerState* CombatPlayerState = GetPlayerState<ACombatPlayerState>();
	if (CombatPlayerState)
	{
		return CombatPlayerState->GetPlayerLevel();
	}
	else
	{
		Super::GetPlayerLevel();
	}
	return 0;
}

void ACombatCharacter::InitAbilityActorInfo()
{
	ACombatPlayerState* CombatPlayerState = GetPlayerState<ACombatPlayerState>();
	check(CombatPlayerState);
	CombatPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(CombatPlayerState, this);


	Cast<UCombatAbilitySystemComponent>(CombatPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	AbilitySystemComponent = CombatPlayerState->GetAbilitySystemComponent();
	AttributeSet = CombatPlayerState->GetAttributeSet();

	if (ACombatPlayerController* CombatPlayerController = Cast<ACombatPlayerController>(GetController()))
	{
		ACombatHUD* CombatHUD = Cast<ACombatHUD>(CombatPlayerController->GetHUD());
		if (CombatHUD)
		{
			CombatHUD->InitOverlay(CombatPlayerController, CombatPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
	InitializeDefaultAttributes();
	UCombatAbilitySystemComponent*CombatAbilitySystemComponent =Cast<UCombatAbilitySystemComponent>(AbilitySystemComponent);
	CombatAbilitySystemComponent->AddWeaponEquipAbilities(WeaponStartupAbilities);
}

void ACombatCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (CombatComponent)
	{
		CombatComponent->Character = this;
	}
}

// Called every frame
void ACombatCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GetLocalRole() > ENetRole::ROLE_SimulatedProxy && IsLocallyControlled())
	{
		AimOffset(DeltaTime);
	}
	else
	{
		TimeSinceLastMovementReplication += DeltaTime;
		if (TimeSinceLastMovementReplication > 0.25f)
		{
			OnRep_ReplicatedMovement();
		}
		CalculateAO_Pitch();
	}
	HideCamIfCharacterClose();
}

// Called when the game starts or when spawned
void ACombatCharacter::BeginPlay()
{
	Super::BeginPlay();
}


void ACombatCharacter::Jump()
{
	Super::Jump();
	GetCharacterMovement()->bNotifyApex = true;
}


void ACombatCharacter::HighLightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(1);
}

void ACombatCharacter::UnHighLightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
}

void ACombatCharacter::OnRep_ReplicatedMovement()
{
	Super::OnRep_ReplicatedMovement();

	SimProxiesTurn();

	TimeSinceLastMovementReplication = 0.f;
}


float ACombatCharacter::CalculateSpeed()
{
	FVector Velocity = GetVelocity();
	Velocity.Z = 0.f;
	return Velocity.Size();
}





FVector ACombatCharacter::GetHitTarget() const
{
	if (CombatComponent == nullptr)
	{
		return FVector();
	}
	return CombatComponent->HitTarget;
}


void ACombatCharacter::AimOffset(float DeltaTime)
{
	if (CombatComponent == nullptr)
	{
		return;
	}

	float Speed = CalculateSpeed();
	bool bIsInAir = GetCharacterMovement()->IsFalling();

	if (Speed == 0.f && !bIsInAir) //Standing Still and not jumping
	{
		bRotateRootBone = true;
		FRotator CurrentAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		FRotator DeltaAimRotation = UKismetMathLibrary::NormalizedDeltaRotator(CurrentAimRotation, StartingAimRotation);

		AO_Yaw = DeltaAimRotation.Yaw;

		if (TurnInPlace == ETurningInPlace::ETIP_NotTurning)
		{
			InterpAO_Yaw = AO_Yaw;
		}
		bUseControllerRotationYaw = true;
		FTurnInPlace(DeltaTime);
	}
	if (Speed > 0.f || bIsInAir)
	{
		bRotateRootBone = false;
		StartingAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		AO_Yaw = 0.f;
		bUseControllerRotationYaw = true;
		TurnInPlace = ETurningInPlace::ETIP_NotTurning;
	}

	CalculateAO_Pitch();
}

void ACombatCharacter::SimProxiesTurn()
{
	if (CombatComponent == nullptr)return;

	bRotateRootBone = false;

	float Speed = CalculateSpeed();
	if (Speed > 0.f)
	{
		TurnInPlace = ETurningInPlace::ETIP_NotTurning;
		return;
	}

	ProxyRotationLastFrame = ProxyRotation;
	ProxyRotation = GetActorRotation();
	ProxyYaw = UKismetMathLibrary::NormalizedDeltaRotator(ProxyRotation, ProxyRotationLastFrame).Yaw;

	if (FMath::Abs(ProxyYaw) > TurnThreshhold)
	{
		bUseControllerRotationYaw = true;
		if (ProxyYaw > TurnThreshhold)
		{
			TurnInPlace = ETurningInPlace::ETIP_Right;
		}
		else if (ProxyYaw < -TurnThreshhold)
		{
			TurnInPlace = ETurningInPlace::ETIP_Left;
		}
		else
		{
			TurnInPlace = ETurningInPlace::ETIP_NotTurning;
		}
		return;
	}
	//TurnInPlace = ETurningInPlace::ETIP_NotTurning;
}

void ACombatCharacter::CalculateAO_Pitch()
{
	AO_Pitch = GetBaseAimRotation().Pitch;
	if (AO_Pitch > 90.f && !IsLocallyControlled())
	{
		FVector2d InRange(270.f, 360.f);
		FVector2d OutRange(-90.f, 0.f);
		AO_Pitch = FMath::GetMappedRangeValueClamped(InRange, OutRange, AO_Pitch);
	}
}

void ACombatCharacter::FTurnInPlace(float DeltaTime)
{
	if (AO_Yaw > 90.f)
	{
		TurnInPlace = ETurningInPlace::ETIP_Right;
	}
	if (AO_Yaw < -90.f)
	{
		TurnInPlace = ETurningInPlace::ETIP_Left;
	}
	if (TurnInPlace != ETurningInPlace::ETIP_NotTurning)
	{
		InterpAO_Yaw = FMath::FInterpTo(InterpAO_Yaw, 0.0f, DeltaTime, 3.0f);
		AO_Yaw = InterpAO_Yaw;
		if (FMath::Abs(AO_Yaw) < 15.0f)
		{
			TurnInPlace = ETurningInPlace::ETIP_NotTurning;
			StartingAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		}
	}
}


void ACombatCharacter::HideCamIfCharacterClose()
{
	if (!IsLocallyControlled())return;
	if ((CameraComponent->GetComponentLocation() - GetActorLocation()).Size() < HideCamThreshhold)
	{
		GetMesh()->SetVisibility(false);
	}
	else
	{
		GetMesh()->SetVisibility(true);
	}
}

//MC RPC IMPLEMENTATIONS
