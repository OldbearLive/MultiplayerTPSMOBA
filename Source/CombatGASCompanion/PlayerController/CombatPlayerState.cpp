// Copyright SuspiciousDinosaur @ Youtube


#include "CombatPlayerState.h"

#include "CombatGASCompanion/AbilitySystem/CombatAbilitySystemComponent.h"
#include "CombatGASCompanion/AbilitySystem/CombatAttributeSet.h"
#include "Net/UnrealNetwork.h"

ACombatPlayerState::ACombatPlayerState()
{
	NetUpdateFrequency = 100.f;

	AbilitySystemComponent = CreateDefaultSubobject<UCombatAbilitySystemComponent>("AbilitySystemComponent");

	AbilitySystemComponent->SetIsReplicated(true);

	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);



	AttributeSet = CreateDefaultSubobject<UCombatAttributeSet>("AttributeSet");
}

void ACombatPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACombatPlayerState,Level);
}

UAbilitySystemComponent* ACombatPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ACombatPlayerState::OnRep_Level(int32 OldLevel)
{
	
}
