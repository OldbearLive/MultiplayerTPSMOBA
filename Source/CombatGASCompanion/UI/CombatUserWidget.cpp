// Copyright SuspiciousDinosaur @ Youtube


#include "CombatUserWidget.h"

void UCombatUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
