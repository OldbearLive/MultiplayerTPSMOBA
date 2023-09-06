// Copyright SuspiciousDinosaur @ Youtube


#include "CombatProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ACombatProjectile::ACombatProjectile()
{
 	
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("Projectile Movement");

	ProjectileMovementComponent->InitialSpeed = 550.f;
	ProjectileMovementComponent->MaxSpeed = 550.f;
	ProjectileMovementComponent->ProjectileGravityScale=0.f;
}

void ACombatProjectile::OnOverlap(AActor* TargetActor)
{
	
}


void ACombatProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}


