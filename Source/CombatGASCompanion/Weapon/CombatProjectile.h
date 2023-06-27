// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CombatProjectile.generated.h"

UCLASS()
class COMBATGASCOMPANION_API ACombatProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACombatProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp,AActor* OtherActor,UPrimitiveComponent* OtherComp,FVector NormalImpulse,const FHitResult& Hit);

	virtual void Destroyed() override;
private:
	UPROPERTY(EditAnywhere)
	class UBoxComponent* CollisionBox;

	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* TracerSystem;

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* HitFX;
	
	UPROPERTY(EditAnywhere)
	class USoundBase* HitSound;

	FRotator SpawnRotator;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
