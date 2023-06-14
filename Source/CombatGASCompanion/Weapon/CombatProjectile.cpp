// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatProjectile.h"

#include "NiagaraFunctionLibrary.h"
#include "../../../../../UE_5.1/Engine/Plugins/FX/Niagara/Source/Niagara/Classes/NiagaraSystem.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ACombatProjectile::ACombatProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	CollisionBox=CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	SetRootComponent(CollisionBox);
	CollisionBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility,ECollisionResponse::ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic,ECollisionResponse::ECR_Block);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile MovementComponent"));

	ProjectileMovementComponent->bRotationFollowsVelocity= true;
	
}

// Called when the game starts or when spawned
void ACombatProjectile::BeginPlay()
{
	Super::BeginPlay();

	if(TracerSystem)
	{
		UNiagaraFunctionLibrary::SpawnSystemAttached(TracerSystem,CollisionBox,FName(),GetActorLocation(),GetActorRotation(),EAttachLocation::KeepWorldPosition,false);
	}
}

// Called every frame
void ACombatProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

