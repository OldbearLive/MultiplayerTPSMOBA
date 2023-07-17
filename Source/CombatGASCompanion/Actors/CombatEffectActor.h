// Copyright SuspiciousDinosaur @ Youtube

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "Components/SphereComponent.h"
#include "CombatEffectActor.generated.h"


UENUM(BlueprintType)
enum class EEffectApplicationPolicy:uint8
{
	ApplyOnOverlap,
	ApplyOnEndOverlap,
	DoNotApply
};

UENUM(BlueprintType)
enum class EEffectRemovalPolicy:uint8
{
	RemoveOnEndOverlap,
	DoNotRemove
};


UCLASS()
class COMBATGASCOMPANION_API ACombatEffectActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACombatEffectActor();

	
	

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor*TargetActor);

	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor*TargetActor);

	//END FUNCTION SECTION

	//START VARIABLE SECTION
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category= "Applied Effects")
	bool bDestroyOnEffectRemoval = false;
	
	//END VARIABLE SECTION

	//EFFECTAPPLICATION SECTION

	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect>GameplayEffectClass);

	//CLASSTYPES AND POLICIES
	
	//CLASS
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category= "Applied Effects")
	TSubclassOf<UGameplayEffect>InstantGameplayEffectClass;
	
	//APPLICATIONPOLICY
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category= "Applied Effects")
	EEffectApplicationPolicy InstantEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	
	
	//CLASS
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Applied Effects")
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;

	//APPLICATIONPOLICY
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category= "Applied Effects")
	EEffectApplicationPolicy DurationEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	

	//CLASS
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Applied Effects")
	TSubclassOf<UGameplayEffect> InfiniteGameplayEffectClass;

	//APPLICATIONPOLICY
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category= "Applied Effects")
	EEffectApplicationPolicy InfiniteEffectApplicationPolicy= EEffectApplicationPolicy::DoNotApply;

	//REMOVALPOLICY
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category= "Applied Effects")
	EEffectRemovalPolicy InfiniteEffectRemovalPolicy= EEffectRemovalPolicy::RemoveOnEndOverlap;


	TMap<FActiveGameplayEffectHandle,UAbilitySystemComponent*> ActiveEffectHandles;
	
private:
	
};
