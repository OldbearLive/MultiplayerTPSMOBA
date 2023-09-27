// Copyright SuspiciousDinosaur @ Youtube


#include "MinionCharacter.h"

#include "CombatGASCompanion/AbilitySystem/CombatAbilitySystemComponent.h"
#include "CombatGASCompanion/AbilitySystem/CombatAttributeSet.h"
#include "CombatGASCompanion/AbilitySystem/CombatBlueprintFunctionLibrary.h"
#include "Components/WidgetComponent.h"

AMinionCharacter::AMinionCharacter()
{
	AbilitySystemComponent = CreateDefaultSubobject<UCombatAbilitySystemComponent>("AbilitySystemComponent");

	AbilitySystemComponent->SetIsReplicated(true);

	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);


	AttributeSet = CreateDefaultSubobject<UCombatAttributeSet>("AttributeSet");

	RemoteStatsBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("Multiplayer State Widget"));
	RemoteStatsBar->SetupAttachment(GetRootComponent());
	RemoteStatsBar->SetVisibility(false);

}

void AMinionCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	InitAbilityActorInfo();
}

void AMinionCharacter::InitializeDefaultAttributes() const
{
	UCombatBlueprintFunctionLibrary::InitializeDefaultAttributes(this,CharacterClass,Level,AbilitySystemComponent);
}

void AMinionCharacter::HighLightActor()
{
	RemoteStatsBar->SetVisibility(true);
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(1);
	
}

void AMinionCharacter::UnHighLightActor()
{
	RemoteStatsBar->SetVisibility(false);
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

	InitializeDefaultAttributes();

	if (UCombatAttributeSet* CombatAttributeSet = Cast<UCombatAttributeSet>(AttributeSet))
	{
		//Lambdas to bind to Delegates
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(CombatAttributeSet->GetHealthAttribute()).
								AddLambda(
									[this](const FOnAttributeChangeData& Data)
									{
										OnHealthChangedSignature.Broadcast(Data.NewValue);
									}
								);

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(CombatAttributeSet->GetMaxHealthAttribute()).
								AddLambda(
									[this](const FOnAttributeChangeData& Data)
									{
										OnMaxHealthChangedSignature.Broadcast(Data.NewValue);
									}
								);

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(CombatAttributeSet->GetEnergyAttribute()).
								AddLambda(
									[this](const FOnAttributeChangeData& Data)
									{
										OnEnergyChangedSignature.Broadcast(Data.NewValue);
									}
								);

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(CombatAttributeSet->GetMaxEnergyAttribute()).
								AddLambda(
									[this](const FOnAttributeChangeData& Data)
									{
										OnMaxEnergyChangedSignature.Broadcast(Data.NewValue);
									}
								);
		OnHealthChangedSignature.Broadcast(CombatAttributeSet->GetHealth());
		OnMaxHealthChangedSignature.Broadcast(CombatAttributeSet->GetMaxHealth());
		OnEnergyChangedSignature.Broadcast(CombatAttributeSet->GetEnergy());
		OnMaxEnergyChangedSignature.Broadcast(CombatAttributeSet->GetMaxEnergy());
	}
}
