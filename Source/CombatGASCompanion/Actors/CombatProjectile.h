// Copyright SuspiciousDinosaur @ Youtube

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "NiagaraSystem.h"
#include "GameFramework/Actor.h"
#include "CombatProjectile.generated.h"

class UBoxComponent;
class UProjectileMovementComponent;

UCLASS()
class COMBATGASCOMPANION_API ACombatProjectile : public AActor
{
	GENERATED_BODY()

public:
	ACombatProjectile();


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UBoxComponent> BoxComponent;

	UPROPERTY(BlueprintReadWrite, meta =(ExposeOnSpawn = true))
	FGameplayEffectSpecHandle DamageEffectSpecHandle;

protected:
	virtual void BeginPlay() override;

	virtual void Destroyed() override;

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> ImpactEffect;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> ImpactSound;

	bool bHit = false;

	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor* TargetActor);
};
