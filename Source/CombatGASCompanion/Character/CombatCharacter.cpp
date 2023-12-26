// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "CombatGASCompanion/CombatGASCompanion.h"
#include "CombatGASCompanion/AbilitySystem/CombatAbilitySystemComponent.h"
#include "CombatGASCompanion/AbilitySystem/CombatAttributeSet.h"
#include "CombatGASCompanion/HUD/CombatHUD.h"
#include "CombatGASCompanion/PlayerController/CombatPlayerController.h"
#include "CombatGASCompanion/PlayerController/CombatPlayerState.h"


#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"


// Sets default values
ACombatCharacter::ACombatCharacter(const FObjectInitializer& ObjectInitializer): Super(
	ObjectInitializer.SetDefaultSubobjectClass<UCombatCharacterMovementComponent>(
		ACharacter::CharacterMovementComponentName))
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CombatCharacterMovementComponent = Cast<UCombatCharacterMovementComponent>(GetCharacterMovement());

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	CameraBoom->SetupAttachment(GetMesh(), InSocketNameCam);
	CameraBoom->TargetArmLength = 600.0f;
	CameraBoom->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Follow Camera"));
	CameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;


	RemoteStatsBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("Multiplayer State Widget"));
	RemoteStatsBar->SetupAttachment(GetRootComponent());
	RemoteStatsBar->SetVisibility(false);


	GetMesh()->SetCollisionObjectType(ECC_SkeletalMesh);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->MaxWalkSpeed = DefaultMaxSpeed;

	CharacterClass = ECharacterClass::Humanoid;

	TurnInPlace = ETurningInPlace::ETIP_NotTurning;

	NetUpdateFrequency = 66.0f;
	MinNetUpdateFrequency = 33.0f;
}


//ReplicatedVariables
void ACombatCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ACombatCharacter, bIsWeaponEquipped, COND_None);
	DOREPLIFETIME_CONDITION(ACombatCharacter, CombatRangedWeaponsArray, COND_None);
	DOREPLIFETIME_CONDITION(ACombatCharacter, ActiveWeaponIndex, COND_None);
	DOREPLIFETIME_CONDITION(ACombatCharacter, AmmoClipArray, COND_None);
	DOREPLIFETIME_CONDITION(ACombatCharacter, MaxAmmoClipArray, COND_None);
	DOREPLIFETIME_CONDITION(ACombatCharacter, ReserveAmmoArray, COND_None);
	DOREPLIFETIME_CONDITION(ACombatCharacter, MaxReserveAmmoArray, COND_None);
	DOREPLIFETIME_CONDITION(ACombatCharacter, bIsJet, COND_None);
	DOREPLIFETIME_CONDITION(ACombatCharacter, bIsDash, COND_None);
	DOREPLIFETIME_CONDITION(ACombatCharacter, bIsBoost, COND_None);
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

ECharacterClass ACombatCharacter::GetCharacterClass_Implementation()
{
	return CharacterClass;
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
	
	if (UCombatAttributeSet* CombatAttributeSet = Cast<UCombatAttributeSet>(AttributeSet))
	{
		//Lambdas to bind to Delegates
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(CombatAttributeSet->GetHealthAttribute()).
		                        AddLambda(
			                        [this](const FOnAttributeChangeData& Data)
			                        {
				                        OnHealthChangedSignature.Broadcast(Data.NewValue);
			                        }
		                        );

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(CombatAttributeSet->GetMaxHealthAttribute()).
		                        AddLambda(
			                        [this](const FOnAttributeChangeData& Data)
			                        {
				                        OnMaxHealthChangedSignature.Broadcast(Data.NewValue);
			                        }
		                        );

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(CombatAttributeSet->GetEnergyAttribute()).
		                        AddLambda(
			                        [this](const FOnAttributeChangeData& Data)
			                        {
				                        OnEnergyChangedSignature.Broadcast(Data.NewValue);
			                        }
		                        );

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(CombatAttributeSet->GetMaxEnergyAttribute()).
		                        AddLambda(
			                        [this](const FOnAttributeChangeData& Data)
			                        {
				                        OnMaxEnergyChangedSignature.Broadcast(Data.NewValue);
			                        }
		                        );
		OnHealthChangedSignature.Broadcast(CombatAttributeSet->GetHealth());
		OnMaxHealthChangedSignature.Broadcast(CombatAttributeSet->GetMaxHealth());
		OnEnergyChangedSignature.Broadcast(CombatAttributeSet->GetEnergy());
		OnMaxEnergyChangedSignature.Broadcast(CombatAttributeSet->GetMaxEnergy());
	}
}

FCollisionQueryParams ACombatCharacter::GetIgnoreCharacterParams() const
{
	FCollisionQueryParams Params;

	TArray<AActor*> CharacterChildActors;
	GetAllChildActors(CharacterChildActors);
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActors(CharacterChildActors);

	return Params;
}


void ACombatCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
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
		if (TimeSinceLastMovementReplication > 0.15f)
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

void ACombatCharacter::HitReactTagChanged(const FGameplayTag CallbackTag, int32 TagCount)
{
	bHitReacting = TagCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? StaggerSpeed : DefaultMaxSpeed;
}

void ACombatCharacter::DeathTagChanged(const FGameplayTag CallbackTag, int32 TagCount)
{
	bDead = TagCount > 0;
	SetLifeSpan(10);
}

void ACombatCharacter::Jump()
{
	Super::Jump();
	GetCharacterMovement()->bNotifyApex = true;
}


void ACombatCharacter::HighLightActor()
{
	RemoteStatsBar->SetVisibility(true);
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(1);
}

void ACombatCharacter::UnHighLightActor()
{
	RemoteStatsBar->SetVisibility(false);
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
	const ACombatPlayerController* PlayerController = Cast<ACombatPlayerController>(GetController());
	if (!PlayerController)
	{
		return FVector();
	}
	return PlayerController->HitTarget;
}

void ACombatCharacter::Server_SetDashing_Implementation(bool bDash)
{
	bIsDash = bDash;
}

void ACombatCharacter::Server_SetBoosting_Implementation(bool bBoost)
{
	bIsBoost = bBoost;
}

void ACombatCharacter::Server_SetJetPacking_Implementation(bool bJet)
{
	bIsJet = bJet;
}


void ACombatCharacter::AimOffset(float DeltaTime)
{
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
