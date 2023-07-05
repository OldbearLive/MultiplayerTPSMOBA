// Copyright SuspiciousDinosaur @ Youtube


#include "MinionCharacter.h"

#include "CombatGASCompanion/AbilitySystem/CombatAbilitySystemComponent.h"
#include "CombatGASCompanion/AbilitySystem/CombatAttributeSet.h"

AMinionCharacter::AMinionCharacter()
{
	AbilitySystemComponent = CreateDefaultSubobject<UCombatAbilitySystemComponent>("AbilitySystemComponent");

	AbilitySystemComponent->SetIsReplicated(true);

	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);


	AttributeSet = CreateDefaultSubobject<UCombatAttributeSet>("AttributeSet");
}

void AMinionCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	AbilitySystemComponent->InitAbilityActorInfo(this,this);
}

void AMinionCharacter::HighLightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(1);
	
}

void AMinionCharacter::UnHighLightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
}
