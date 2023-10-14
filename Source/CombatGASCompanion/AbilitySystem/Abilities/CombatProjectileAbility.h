// Copyright SuspiciousDinosaur @ Youtube

#pragma once

#include "CoreMinimal.h"
#include "CombatDamageGameplayAbility.h"
#include "CombatProjectileAbility.generated.h"

class ACombatProjectile;
/**
 * 
 */
UCLASS()
class COMBATGASCOMPANION_API UCombatProjectileAbility : public UCombatDamageGameplayAbility
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;


	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void SpawnProjectile(const FVector& ProjectileTargetLocation);
	

	UPROPERTY(BlueprintReadWrite)
	FTransform SpawnTransform;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ACombatProjectile> ProjectileClass;

	
};


