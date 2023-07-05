// Copyright SuspiciousDinosaur @ Youtube


#include "CombatPlayerState.h"

#include "CombatGASCompanion/AbilitySystem/CombatAbilitySystemComponent.h"
#include "CombatGASCompanion/AbilitySystem/CombatAttributeSet.h"

ACombatPlayerState::ACombatPlayerState()
{
	NetUpdateFrequency = 100.f;

	AbilitySystemComponent = CreateDefaultSubobject<UCombatAbilitySystemComponent>("AbilitySystemComponent");

	AbilitySystemComponent->SetIsReplicated(true);

	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);



	AttributeSet = CreateDefaultSubobject<UCombatAttributeSet>("AttributeSet");
}

UAbilitySystemComponent* ACombatPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
