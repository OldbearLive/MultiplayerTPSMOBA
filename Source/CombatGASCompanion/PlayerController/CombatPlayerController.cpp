// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "CombatGASCompanion/Interfaces/InteractWithCrosshairsInterface.h"
#include "Kismet/GameplayStatics.h"

ACombatPlayerController::ACombatPlayerController()
{
	bReplicates = true;
}

void ACombatPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	FHitResult HitResult;
	TraceUnderCrosshairs(HitResult);
}

void ACombatPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (CombatContext)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
			GetLocalPlayer());

		if (Subsystem)
		{
			Subsystem->AddMappingContext(CombatContext, 0);
		}
	}
}


void ACombatPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);


	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACombatPlayerController::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACombatPlayerController::Look);
	/*EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, );
	EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Triggered, this, &ACombatPlayerController::Equip);
	EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &ACombatPlayerController::FAimPressed);
	EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &ACombatPlayerController::FAimReleased);
	EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &ACombatPlayerController::FFirePressed);
	EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &ACombatPlayerController::FFireReleased);*/
}

void ACombatPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2d InputAxisVector = InputActionValue.Get<FVector2d>();

	const FRotator Rotation = GetControlRotation();

	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}


void ACombatPlayerController::Look(const FInputActionValue& InputActionValue)
{
	const FVector2d LookAxisVector = InputActionValue.Get<FVector2d>();

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddControllerPitchInput(LookAxisVector.Y);
		ControlledPawn->AddControllerYawInput(LookAxisVector.X);
	}
}

void ACombatPlayerController::TraceUnderCrosshairs(FHitResult& TraceHitResult)
{
	{
		const APawn* ControlledPawn = GetPawn<APawn>();
		if (ControlledPawn == nullptr)return;

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
			if (ControlledPawn)
			{
				float DistanceToCharacter = (ControlledPawn->GetActorLocation() - Start).Size();
				Start += CrosshairWorldDirection * (DistanceToCharacter + 75.f);
				//ShowDebugSphereForCrosshairTraceStart

				DrawDebugSphere(GetWorld(), Start, 12.f, 12, FColor::Red);
			}

			FVector End = Start + (CrosshairWorldDirection * 50000);

			GetWorld()->LineTraceSingleByChannel(TraceHitResult, Start, End, ECC_Visibility);

			if (!TraceHitResult.bBlockingHit)return;

			LastActor = ThisActor;

			ThisActor = Cast<IInteractWithCrosshairsInterface>(TraceHitResult.GetActor());

			//TARGETING
			/*LineTraceFrom Crosshair
			 *
			 * A- Last Actor is null and ThisActor is null
			 *		- Do Nothing
			 * B- Last Actor is null and ThisActor is Valid
			 *		- Call Interface function Highlight on This Actor
			 *	C -Last Actor is Valid and ThisActor is NUll
			 *		- Call Interface function UnHighlight on Last Actor
			 *	D - Last Actor and This actor are Different
			 *		- Call Interface Unhightlight on Last actor and Highlight on thisactor
			 *	E - Last Actor and This actor are Same
			 *		- Do Nothing
			 */

			if (LastActor == nullptr)
			{
				if (ThisActor != nullptr)
				{
					//Case B
					ThisActor->HighLightActor();
				}
				else
				{
					//CaseA
					//both are null and do nothing
				}
			}
			else //LastActor is Valid
			{
				if (ThisActor == nullptr)
				{
					// CaseC
					LastActor->UnHighLightActor();
				}
				else //Both are Valid
				{
					if (LastActor != ThisActor)
					{
						/*Case D*/
						LastActor->UnHighLightActor();
						ThisActor->HighLightActor();
					}
					else
					{
						//Case E
					}
				}
			}
		}
	}
}
