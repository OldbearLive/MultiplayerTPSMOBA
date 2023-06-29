// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "CombatMainMenuGameMode.generated.h"

/**
 * 
 */
UCLASS()
class COMBATGASCOMPANION_API ACombatMainMenuGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	virtual void PostLogin(APlayerController* NewPlayer) override;
};
