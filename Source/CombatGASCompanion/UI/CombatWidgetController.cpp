// Copyright SuspiciousDinosaur @ Youtube


#include "CombatWidgetController.h"

void UCombatWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WidgetControllerParams)
{
	PlayerController= WidgetControllerParams.PlayerController;
	PlayerState= WidgetControllerParams.PlayerState;
	AbilitySystemComponent= WidgetControllerParams.AbilitySystemComponent;
	AttributeSet= WidgetControllerParams.AttributeSet;
}

void UCombatWidgetController::BroadcastInitialValues()
{
}

void UCombatWidgetController::BindCallbacksToDependencies()
{
}
