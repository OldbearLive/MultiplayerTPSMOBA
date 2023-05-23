// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputActionValue.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
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
	CameraComponent->SetupAttachment(CameraBoom,USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;

	bUseControllerRotationYaw =false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(RootComponent);
}
//ReplicatedVariables
void ACombatCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ACombatCharacter,OverlappingWeapon,COND_OwnerOnly);
}


// Called every frame
void ACombatCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}
// Called when the game starts or when spawned
void ACombatCharacter::BeginPlay()
{
	Super::BeginPlay();

	if(const APlayerController*PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem*Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PilotInputMappingContext,0);
		}
		
	}
}


void ACombatCharacter::FMove(const FInputActionValue& Value)
{
	const FVector2d MovementValue =Value.Get<FVector2d>();

	const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f,Rotation.Yaw,0.f);
		const FVector ForwardDirection(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X));
		AddMovementInput(ForwardDirection,MovementValue.Y);
		const FVector RightDirection(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y));
		AddMovementInput(RightDirection,MovementValue.X);
	
}

void ACombatCharacter::FLook(const FInputActionValue& Value)
{
	const FVector2d LookAxisVector = Value.Get<FVector2d>();
	AddControllerPitchInput(LookAxisVector.Y);
	AddControllerYawInput(LookAxisVector.X);
}




// Called to bind functionality to input
void ACombatCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if(UEnhancedInputComponent*EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(Move,ETriggerEvent::Triggered,this,&ACombatCharacter::FMove);
		EnhancedInputComponent->BindAction(Look,ETriggerEvent::Triggered,this,&ACombatCharacter::FLook);
		EnhancedInputComponent->BindAction(JumpAction,ETriggerEvent::Triggered,this,&ACharacter::Jump);
	}
}


void ACombatCharacter::SetOverlappingWeapon(ACombatRangedWeapon* Weapon)
{

	if(OverlappingWeapon)
	{
		OverlappingWeapon->ShowPickupWidget(false);
	}
	OverlappingWeapon=Weapon;

	if(IsLocallyControlled())
	{
		if(OverlappingWeapon)
		{
			OverlappingWeapon->ShowPickupWidget(true);
		}
		
	}
}

void ACombatCharacter::OnRep_OverlappingWeapon(ACombatRangedWeapon*LastWeapon)
{
	
	if(OverlappingWeapon)
	{
		OverlappingWeapon->ShowPickupWidget(true);
	}

	if(LastWeapon)
	{
		LastWeapon->ShowPickupWidget(false);
	}
}
