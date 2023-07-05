// Copyright SuspiciousDinosaur @ Youtube


#include "CombatEffectActor.h"

// Sets default values
ACombatEffectActor::ACombatEffectActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACombatEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACombatEffectActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

