// Copyright SuspiciousDinosaur @ Youtube


#include "CombatEffectActor.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "CombatGASCompanion/AbilitySystem/CombatAttributeSet.h"


// Sets default values
ACombatEffectActor::ACombatEffectActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");

	SetRootComponent(MeshComponent);
	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");

	Sphere->SetupAttachment(GetRootComponent());
}

void ACombatEffectActor::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                      const FHitResult& HitResult)
{
	IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(OtherActor);
	if (ASCInterface)
	{
		const UCombatAttributeSet* CombatAttributeSet = Cast<UCombatAttributeSet>(
			ASCInterface->GetAbilitySystemComponent()->GetAttributeSet(UCombatAttributeSet::StaticClass()));
		if(CombatAttributeSet)
		{
			//TODO: Change to GAmeplayEffect at somepoint.
			UCombatAttributeSet*MutableCombatAttributeset = const_cast<UCombatAttributeSet*>(CombatAttributeSet);
			MutableCombatAttributeset->SetHealth(CombatAttributeSet->GetHealth()+ 25.0f);
			Destroy();
		}
	}
}

void ACombatEffectActor::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

// Called when the game starts or when spawned
void ACombatEffectActor::BeginPlay()
{
	Super::BeginPlay();
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ACombatEffectActor::BeginOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &ACombatEffectActor::EndOverlap);
}

// Called every frame
