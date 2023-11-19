// Copyright SuspiciousDinosaur @ Youtube


#include "CombatProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "CombatGASCompanion/AbilitySystem/CombatBlueprintFunctionLibrary.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GeometryCollection/GeometryCollectionSimulationTypes.h"
#include "Kismet/GameplayStatics.h"
#include "Tasks/Task.h"
#include "Tasks/Task.h"

// Sets default values
ACombatProjectile::ACombatProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("Projectile Movement");
	BoxComponent = CreateDefaultSubobject<UBoxComponent>("Box Collision");
	SetRootComponent(BoxComponent);


	ProjectileMovementComponent->InitialSpeed = 550.f;
	ProjectileMovementComponent->MaxSpeed = 550.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;
}

void ACombatProjectile::OnOverlap(AActor* TargetActor)
{
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), GetActorRotation());
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation(),
	                                               GetActorRotation());

	

	if (!DamageEffectSpecHandle.Data.IsValid() || DamageEffectSpecHandle.Data.Get()->GetContext().GetEffectCauser() ==TargetActor)
	{
		return;
	}
	
	if (!UCombatBlueprintFunctionLibrary::IsNotFriend(DamageEffectSpecHandle.Data.Get()->GetContext().GetEffectCauser(),TargetActor))
	{
		return;
	}
	if (HasAuthority())
	{
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (TargetASC)
		{
			TargetASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());
		}
		//BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//Destroy();
	}
	else
	{
		bHit = true;
	}
}

void ACombatProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void ACombatProjectile::Destroyed()
{
	if (!bHit && !HasAuthority())
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), GetActorRotation());
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation(),
		                                               GetActorRotation());
	}

	Super::Destroyed();
}
