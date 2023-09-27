// Copyright SuspiciousDinosaur @ Youtube


#include "BaseCharacter.h"

#include "AbilitySystemComponent.h"
#include "CombatGASCompanion/AbilitySystem/CombatAbilitySystemComponent.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}



void ABaseCharacter::InitAbilityActorInfo()
{
}

void ABaseCharacter::InitializeDefaultAttributes() const
{
	ApplyEffectSpecToSelf(DefaultPrimaryAttributes,1.f);
	ApplyEffectSpecToSelf(DefaultSecondaryAttributes,1.f);
	ApplyEffectSpecToSelf(DefaultVitalAttributes,1.f);
	
}

void ABaseCharacter::ApplyEffectSpecToSelf(TSubclassOf<UGameplayEffect> GameplayEffect, float Level) const
{
	if(IsValid(GetAbilitySystemComponent()))
	{
		if(IsValid(GameplayEffect))
		{
			auto EffectContext = GetAbilitySystemComponent()->MakeEffectContext();
			EffectContext.AddSourceObject(this);
			const auto EffectSpec = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffect,Level,EffectContext);
			GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*EffectSpec.Data.Get(),GetAbilitySystemComponent());
			
		}
	}
}

void ABaseCharacter::AddCharacterAbilities()
{
	UCombatAbilitySystemComponent* CombatASC = CastChecked<UCombatAbilitySystemComponent>(AbilitySystemComponent);
	if(!HasAuthority())return;

	CombatASC->AddCharacterAbilities(StartupAbiity);
}


