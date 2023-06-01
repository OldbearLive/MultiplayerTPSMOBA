// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatMainMenuGameMode.h"

#include "GameFramework/GameStateBase.h"

void ACombatMainMenuGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();
	if (NumberOfPlayers == 2)
	{
		if (UWorld* World = GetWorld())
		{
			bUseSeamlessTravel = true;
			World->ServerTravel(FString("/Game/Maps/CombatMap?listen"));
		}
	}
}
