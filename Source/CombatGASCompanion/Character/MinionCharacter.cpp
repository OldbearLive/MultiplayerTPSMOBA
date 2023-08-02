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
	
	InitAbilityActorInfo();
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

int32 AMinionCharacter::GetPlayerLevel()
{
	return Level;
}

void AMinionCharacter::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this,this);
	Cast<UCombatAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
}
