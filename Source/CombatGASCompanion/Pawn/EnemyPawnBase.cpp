// Copyright SuspiciousDinosaur @ Youtube


#include "EnemyPawnBase.h"

#include "AbilitySystemComponent.h"

#include "CombatGASCompanion/AbilitySystem/CombatAbilitySystemComponent.h"



// Sets default values
AEnemyPawnBase::AEnemyPawnBase()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

UAbilitySystemComponent* AEnemyPawnBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AEnemyPawnBase::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyPawnBase::InitAbilityActorInfo()
{
}

void AEnemyPawnBase::InitializeDefaultAttributes() const
{
}

void AEnemyPawnBase::ApplyEffectSpecToSelf(TSubclassOf<UGameplayEffect> GameplayEffect, float Level) const
{
	if (IsValid(GetAbilitySystemComponent()))
	{
		if (IsValid(GameplayEffect))
		{
			auto EffectContext = GetAbilitySystemComponent()->MakeEffectContext();
			EffectContext.AddSourceObject(this);
			const auto EffectSpec = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffect, Level, EffectContext);
			GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(
				*EffectSpec.Data.Get(), GetAbilitySystemComponent());
		}
	}
}

void AEnemyPawnBase::AddCharacterAbilities()
{
	UCombatAbilitySystemComponent* CombatASC = CastChecked<UCombatAbilitySystemComponent>(AbilitySystemComponent);
	if (!HasAuthority())return;

	CombatASC->AddCharacterAbilities(StartupAbiity);
}
