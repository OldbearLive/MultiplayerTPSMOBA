// Copyright SuspiciousDinosaur @ Youtube


#include "CombatBlueprintFunctionLibrary.h"

#include "CombatGASCompanion/HUD/CombatHUD.h"
#include "CombatGASCompanion/PlayerController/CombatPlayerState.h"
#include "Kismet/GameplayStatics.h"

UOverlayWidgetController* UCombatBlueprintFunctionLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	APlayerController*PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject,0);
	if(PlayerController)
	{
		if(ACombatHUD* CombatHUD = Cast<ACombatHUD>(PlayerController->GetHUD()))
		{
			ACombatPlayerState* PlayerState = PlayerController->GetPlayerState<ACombatPlayerState>();
			UAbilitySystemComponent* AbilitySystemComponent = PlayerState->GetAbilitySystemComponent();
			UAttributeSet* AttributeSet = PlayerState->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PlayerController,PlayerState,AbilitySystemComponent,AttributeSet);

			return CombatHUD->GetOverlayWidgetController(WidgetControllerParams);
			
		}
	}
		return nullptr;
}

UAttributeWidgetController* UCombatBlueprintFunctionLibrary::GetAttributeWidgetController(
	const UObject* WorldContextObject)
{
	APlayerController*PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject,0);
	if(PlayerController)
	{
		if(ACombatHUD* CombatHUD = Cast<ACombatHUD>(PlayerController->GetHUD()))
		{
			ACombatPlayerState* PlayerState = PlayerController->GetPlayerState<ACombatPlayerState>();
			UAbilitySystemComponent* AbilitySystemComponent = PlayerState->GetAbilitySystemComponent();
			UAttributeSet* AttributeSet = PlayerState->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PlayerController,PlayerState,AbilitySystemComponent,AttributeSet);

			return CombatHUD->GetAttributeWidgetController(WidgetControllerParams);
			
		}
	}
	return nullptr;
}
