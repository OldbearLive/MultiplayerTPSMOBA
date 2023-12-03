// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameplayTagContainer.h"
#include "CombatGASCompanion/AbilitySystem/CombatAbilitySystemComponent.h"
#include "CombatGASCompanion/Character/CombatCharacter.h"
#include "CombatGASCompanion/Input/CombatEnhancedInputComponent.h"
#include "CombatGASCompanion/Interfaces/CombatInterface.h"
#include "CombatGASCompanion/Interfaces/InteractWithCrosshairsInterface.h"
#include "CombatGASCompanion/UI/DamageTextComponent.h"
#include "Kismet/GameplayStatics.h"


ACombatPlayerController::ACombatPlayerController()
{
	bReplicates = true;
}

void ACombatPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if (IsLocalPlayerController())
	{
		TraceUnderCrosshairs(HitResult);

		if (HitResult.bBlockingHit)
		{
			HitTarget = HitResult.ImpactPoint;
		}
		HitTarget = HitResult.TraceEnd;
	}
}

void ACombatPlayerController::CreateDamageWidget(float DamageAmount, bool bIsShieldHit,
                                                 bool IsOverloadHit, UDamageTextComponent* DamageText,
                                                 const FVector inWorldPos)
{
	DamageText->RegisterComponent();
	DamageText->SetWorldLocation(inWorldPos);
	DamageText->SetDamageText(DamageAmount, bIsShieldHit, IsOverloadHit);
}

void ACombatPlayerController::ShowDamageNumber_Implementation(float DamageAmount, AActor* TargetActor,
                                                              bool bIsShieldHit,
                                                              bool IsOverloadHit)
{
	if (IsValid(TargetActor))
	{
		if (DamageTextComponentClass)
		{
			UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(TargetActor, DamageTextComponentClass);
			CreateDamageWidget(DamageAmount, bIsShieldHit, IsOverloadHit, DamageText, HitTarget);
		}
		if (StatusTextComponentClass)
		{
			UDamageTextComponent* StatusText = NewObject<UDamageTextComponent>(TargetActor, StatusTextComponentClass);
			ICombatInterface* CombatInterface = Cast<ICombatInterface>(TargetActor);
			if (CombatInterface)
			{
				FVector StatusTextLocation = CombatInterface->Execute_GetStatusWidgetLocation(TargetActor);
				CreateDamageWidget(DamageAmount, bIsShieldHit, IsOverloadHit, StatusText, StatusTextLocation);
			}
		}
	}
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

	UCombatEnhancedInputComponent* EnhancedInputComponent = CastChecked<UCombatEnhancedInputComponent>(InputComponent);


	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACombatPlayerController::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACombatPlayerController::Look);

	EnhancedInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagsPressed,
	                                           &ThisClass::AbilityInputTagsReleased,
	                                           &ThisClass::AbilityInputTagsHeld);
	/*if (CombatCharacter)
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, CombatCharacter,
		                                   &ACombatCharacter::Jump);
	}*/
	/*EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Triggered, this, &ACombatPlayerController::Equip);
	EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &ACombatPlayerController::FAimPressed);
	EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &ACombatPlayerController::FAimReleased);
	EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &ACombatPlayerController::FFirePressed);
	EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &ACombatPlayerController::FFireReleased);*/
}

void ACombatPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (CombatCharacter) CombatCharacter = nullptr;
	CombatCharacter = Cast<ACombatCharacter>(InPawn);
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
	FVector2d LookAxisVector;
	if (CombatCharacter)
	{
		LookAxisVector=InputActionValue.Get<FVector2d>() * CombatCharacter->CameraSensitivity;
		if (APawn* ControlledPawn = GetPawn<APawn>())
		{
			ControlledPawn->AddControllerPitchInput(LookAxisVector.Y);
			ControlledPawn->AddControllerYawInput(LookAxisVector.X);
		}
	}
	LookAxisVector = InputActionValue.Get<FVector2d>();
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddControllerPitchInput(LookAxisVector.Y);
		ControlledPawn->AddControllerYawInput(LookAxisVector.X);
	}
}

UCombatAbilitySystemComponent* ACombatPlayerController::GetCombatASC()
{
	if (CombatAbilitySystemComponent == nullptr)
	{
		CombatAbilitySystemComponent = Cast<UCombatAbilitySystemComponent>(
			UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return CombatAbilitySystemComponent;
}

void ACombatPlayerController::AbilityInputTagsPressed(FGameplayTag InputTag)
{
	if (GetCombatASC() == nullptr)return;
	GetCombatASC()->AbilityInputTagPressed(InputTag);
	GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Red, *InputTag.ToString());
}

void ACombatPlayerController::AbilityInputTagsReleased(FGameplayTag InputTag)
{
	if (GetCombatASC() == nullptr)return;
	GetCombatASC()->AbilityInputTagReleased(InputTag);
	GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Blue, *InputTag.ToString());
}

void ACombatPlayerController::AbilityInputTagsHeld(FGameplayTag InputTag)
{
	if (GetCombatASC() == nullptr)return;
	GetCombatASC()->AbilityInputTagHeld(InputTag);
	GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Yellow, *InputTag.ToString());
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
				Start += CrosshairWorldDirection * (DistanceToCharacter);
				//ShowDebugSphereForCrosshairTraceStart

				DrawDebugSphere(GetWorld(), Start, 2, 12, FColor::Green);
			}

			FVector End = Start + (CrosshairWorldDirection * TraceLength);

			FCollisionQueryParams Params;
			Params.AddIgnoredActor(ControlledPawn);
			GetWorld()->LineTraceSingleByChannel(TraceHitResult, Start, End, ECC_Visibility, Params);
			DrawDebugLine(GetWorld(), Start, End, FColor::Yellow);

			DrawDebugSphere(GetWorld(), End, 5, 12, FColor::Red);
			if (TraceHitResult.bBlockingHit)
			{
				DrawDebugSphere(GetWorld(), TraceHitResult.ImpactPoint, 5, 12, FColor::Red);
			}
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
