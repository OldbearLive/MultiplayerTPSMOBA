// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CombatGameModeBase.generated.h"


class UCombatCharacterClassInfo;
/**
 * 
 */
UCLASS()
class COMBATGASCOMPANION_API ACombatGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category= "Character ClassDefaults")
	TObjectPtr<UCombatCharacterClassInfo> CombatCharacterClassInfo;
};
