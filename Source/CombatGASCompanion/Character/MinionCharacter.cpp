// Copyright SuspiciousDinosaur @ Youtube


#include "MinionCharacter.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "CombatGASCompanion/CombatGameplayTagsSingleton.h"
#include "CombatGASCompanion/CombatGASCompanion.h"
#include "CombatGASCompanion/AbilitySystem/CombatAbilitySystemComponent.h"
#include "CombatGASCompanion/AbilitySystem/CombatAttributeSet.h"
#include "CombatGASCompanion/AbilitySystem/CombatBlueprintFunctionLibrary.h"
#include "CombatGASCompanion/AI/CombatAIController.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


AMinionCharacter::AMinionCharacter()
{
	AbilitySystemComponent = CreateDefaultSubobject<UCombatAbilitySystemComponent>("AbilitySystemComponent");

	AbilitySystemComponent->SetIsReplicated(true);

	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UCombatAttributeSet>("AttributeSet");

	RemoteStatsBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("Multiplayer State Widget"));
	RemoteStatsBar->SetupAttachment(GetRootComponent());
	RemoteStatsBar->SetVisibility(false);

	GetMesh()->SetCollisionObjectType(ECC_SkeletalMesh);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetCharacterMovement()->MaxWalkSpeed = DefaultMaxSpeed;
}

void AMinionCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!HasAuthority()) return;
	CombatAIController = Cast<ACombatAIController>(NewController);

	CombatAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	CombatAIController->RunBehaviorTree(BehaviorTree);
	CombatAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), false);

	CombatAIController->GetBlackboardComponent()->SetValueAsBool(FName("RangedAttacker"),
	                                                             CharacterClass != ECharacterClass::Biped);
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
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? StaggerSpeed : DefaultMaxSpeed;
	CombatAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), bHitReacting);
}

void AMinionCharacter::DeathTagChanged(const FGameplayTag CallbackTag, int32 TagCount)
{
	bDead = TagCount > 0;
	SetLifeSpan(10);
}
