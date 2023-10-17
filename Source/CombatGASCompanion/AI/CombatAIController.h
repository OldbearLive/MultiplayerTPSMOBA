// Copyright SuspiciousDinosaur @ Youtube

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CombatAIController.generated.h"

class UBehaviorTreeComponent;
class UBlackboardComponent;
/**
 * 
 */
UCLASS()
class COMBATGASCOMPANION_API ACombatAIController : public AAIController
{
	GENERATED_BODY()

public:

	ACombatAIController();

protected:
	

	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
};
