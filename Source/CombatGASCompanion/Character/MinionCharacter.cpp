// Copyright SuspiciousDinosaur @ Youtube


#include "MinionCharacter.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "CombatGASCompanion/CombatGameplayTagsSingleton.h"
#include "CombatGASCompanion/AbilitySystem/CombatAbilitySystemComponent.h"
#include "CombatGASCompanion/AbilitySystem/CombatAttributeSet.h"
#include "CombatGASCompanion/AbilitySystem/CombatBlueprintFunctionLibrary.h"
#include "CombatGASCompanion/AI/CombatAIController.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/FloatingPawnMovement.h"

AMinionCharacter::AMinionCharacter()
{
	AbilitySystemComponent = CreateDefaultSubobject<UCombatAbilitySystemComponent>("AbilitySystemComponent");

	AbilitySystemComponent->SetIsReplicated(true);

	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UCombatAttributeSet>("AttributeSet");

	RemoteStatsBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("Multiplayer State Widget"));
	RemoteStatsBar->SetupAttachment(GetRootComponent());
	RemoteStatsBar->SetVisibility(false);

	PawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>("PawnMovementComponent");
	PawnMovement->MaxSpeed = DefaultMaxSpeed;
}

void AMinionCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	CombatAIController = Cast<ACombatAIController>(NewController);
	if (!HasAuthority()) return;

	CombatAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	CombatAIController->RunBehaviorTree(BehaviorTree);
}


void AMinionCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitAbilityActorInfo();

	if (HasAuthority())
	{
		UCombatBlueprintFunctionLibrary::GiveStartupAbilities(this, AbilitySystemComponent);
	}


	AbilitySystemComponent->RegisterGameplayTagEvent(FCombatGameplayTags::Get().HitReact,
	                                                 EGameplayTagEventType::NewOrRemoved)
	                      .AddUObject(
		                      this,
		                      &AMinionCharacter::HitReactTagChanged
	                      );
	AbilitySystemComponent->RegisterGameplayTagEvent(FCombatGameplayTags::Get().Death,
	                                                 EGameplayTagEventType::NewOrRemoved)
	                      .AddUObject(
		                      this,
		                      &AMinionCharacter::DeathTagChanged
	                      );
}

void AMinionCharacter::InitializeDefaultAttributes() const
{
	UCombatBlueprintFunctionLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
}

void AMinionCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (HasAuthority())
	{
		const FVector Gravity = FVector(0.f, 0.f, -0.8f);
		PawnMovement->AddInputVector(Gravity);
	}
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

UPawnMovementComponent* AMinionCharacter::GetMovementComponent() const
{
	return PawnMovement;
}

void AMinionCharacter::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UCombatAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
	if (HasAuthority())
	{
		InitializeDefaultAttributes();
	}

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

void AMinionCharacter::HitReactTagChanged(const FGameplayTag CallbackTag, int32 TagCount)
{
	bHitReacting = TagCount > 0;
	PawnMovement->MaxSpeed = bHitReacting ? StaggerSpeed : DefaultMaxSpeed;
}

void AMinionCharacter::DeathTagChanged(const FGameplayTag CallbackTag, int32 TagCount)
{
	bDead = TagCount > 0;
	SetLifeSpan(10);
}
