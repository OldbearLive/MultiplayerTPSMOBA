// Copyright SuspiciousDinosaur @ Youtube

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CombatProjectile.generated.h"

class UProjectileMovementComponent;

UCLASS()
class COMBATGASCOMPANION_API ACombatProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ACombatProjectile();


	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor*TargetActor);
	
		UPROPERTY(VisibleAnywhere)
    	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

protected:
	
	virtual void BeginPlay() override;

private:




};
