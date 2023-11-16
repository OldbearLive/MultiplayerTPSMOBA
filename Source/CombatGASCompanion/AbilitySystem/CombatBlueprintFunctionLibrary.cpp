// Copyright SuspiciousDinosaur @ Youtube


#include "CombatBlueprintFunctionLibrary.h"

#include "CombatAbilityTypes.h"
#include "CombatGASCompanion/CombatGameModeBase.h"
#include "CombatGASCompanion/HUD/CombatHUD.h"
#include "CombatGASCompanion/Interfaces/CombatInterface.h"
#include "CombatGASCompanion/PlayerController/CombatPlayerState.h"
#include "Kismet/GameplayStatics.h"

UOverlayWidgetController* UCombatBlueprintFunctionLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	if (PlayerController)
	{
		if (ACombatHUD* CombatHUD = Cast<ACombatHUD>(PlayerController->GetHUD()))
		{
			ACombatPlayerState* PlayerState = PlayerController->GetPlayerState<ACombatPlayerState>();
			UAbilitySystemComponent* AbilitySystemComponent = PlayerState->GetAbilitySystemComponent();
			UAttributeSet* AttributeSet = PlayerState->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PlayerController, PlayerState, AbilitySystemComponent,
			                                                     AttributeSet);

			return CombatHUD->GetOverlayWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}

UAttributeWidgetController* UCombatBlueprintFunctionLibrary::GetAttributeWidgetController(
	const UObject* WorldContextObject)
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	if (PlayerController)
	{
		if (ACombatHUD* CombatHUD = Cast<ACombatHUD>(PlayerController->GetHUD()))
		{
			ACombatPlayerState* PlayerState = PlayerController->GetPlayerState<ACombatPlayerState>();
			UAbilitySystemComponent* AbilitySystemComponent = PlayerState->GetAbilitySystemComponent();
			UAttributeSet* AttributeSet = PlayerState->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PlayerController, PlayerState, AbilitySystemComponent,
			                                                     AttributeSet);

			return CombatHUD->GetAttributeWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}

void UCombatBlueprintFunctionLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject,
                                                                  ECharacterClass CharacterClass, float Level,
                                                                  UAbilitySystemComponent* ASC)
{
	ACombatGameModeBase* CombatGameModeBase = Cast<ACombatGameModeBase>(
		UGameplayStatics::GetGameMode(WorldContextObject));

	if (CombatGameModeBase == nullptr)return;

	AActor* AvatarActor = ASC->GetAvatarActor();
	UCombatCharacterClassInfo* ClassInfo = CombatGameModeBase->CharacterClassInfo;

	FCharacterClassInfo ClassDefaultInfo = ClassInfo->GetClassDefaultsInfo(CharacterClass);

	FGameplayEffectContextHandle PrimaryEffectContexthandle = ASC->MakeEffectContext();
	PrimaryEffectContexthandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle PrimaryAttributesSpecHandle = ASC->MakeOutgoingSpec(
		ClassDefaultInfo.PrimaryAttributes, Level, PrimaryEffectContexthandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle SecondaryEffectContexthandle = ASC->MakeEffectContext();
	SecondaryEffectContexthandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttributeSpecHandle = ASC->MakeOutgoingSpec(
		ClassInfo->SecondaryAttributes, Level, SecondaryEffectContexthandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributeSpecHandle.Data.Get());

	FGameplayEffectContextHandle VitalEffectContexthandle = ASC->MakeEffectContext();
	VitalEffectContexthandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle VitalAttributeSpecHandle = ASC->MakeOutgoingSpec(
		ClassInfo->VitalAttributes, Level, VitalEffectContexthandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributeSpecHandle.Data.Get());
}


void UCombatBlueprintFunctionLibrary::GiveStartupAbilities(const UObject* WorldContextObject,
                                                           UAbilitySystemComponent* AbilitySystemComponent,
                                                           ECharacterClass CharacterClass)
{
	UCombatCharacterClassInfo* ClassInfo = GetCharacterClassInfo(WorldContextObject);

	if (ClassInfo == nullptr)return;
	for (TSubclassOf<UGameplayAbility> AbilityClass : ClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		AbilitySystemComponent->GiveAbility(AbilitySpec);
	}
	const FCharacterClassInfo ClassDefaultInfo = ClassInfo->GetClassDefaultsInfo(CharacterClass);

	for (auto AbilityClass : ClassDefaultInfo.ClassDefaultAbilities)
	{
		ICombatInterface* CombatInterface = Cast<ICombatInterface>(AbilitySystemComponent->GetAvatarActor());
		if (CombatInterface)
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, CombatInterface->GetPlayerLevel());
			AbilitySystemComponent->GiveAbility(AbilitySpec);
		}
	}
}

bool UCombatBlueprintFunctionLibrary::IsShieldHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	const FCombatGameplayEffectContext* LocalCombatEffectContext = static_cast<const FCombatGameplayEffectContext*>(
		EffectContextHandle.Get());
	if (LocalCombatEffectContext)
	{
		return LocalCombatEffectContext->IsShieldHit();
	}
	return false;
}

bool UCombatBlueprintFunctionLibrary::IsOverloadHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	const FCombatGameplayEffectContext* LocalCombatEffectContext = static_cast<const FCombatGameplayEffectContext*>(
		EffectContextHandle.Get());
	if (LocalCombatEffectContext)
	{
		return LocalCombatEffectContext->IsOverloadHit();
	}
	return false;
}

void UCombatBlueprintFunctionLibrary::SetIsShieldHit(FGameplayEffectContextHandle& EffectContextHandle,
                                                     bool bInShieldHit)
{
	FCombatGameplayEffectContext* LocalCombatEffectContext = static_cast<FCombatGameplayEffectContext*>(
		EffectContextHandle.Get());
	if (LocalCombatEffectContext)
	{
		return LocalCombatEffectContext->SetIsShieldHit(bInShieldHit);
	}
}

void UCombatBlueprintFunctionLibrary::SetIsOverloadHit(FGameplayEffectContextHandle& EffectContextHandle,
                                                       bool bInOverloadHit)
{
	FCombatGameplayEffectContext* LocalCombatEffectContext = static_cast<FCombatGameplayEffectContext*>(
		EffectContextHandle.Get());
	if (LocalCombatEffectContext)
	{
		return LocalCombatEffectContext->SetIsOverloadHit(bInOverloadHit);
	}
}

void UCombatBlueprintFunctionLibrary::GetLivePlayersWithinRadius(const UObject* WorldContext,
                                                                 TArray<AActor*>& OverlappingActors,
                                                                 const TArray<AActor*>& ActorsToIgnore, float Radius,
                                                                 const FVector& SphereOrigin)
{
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActors(ActorsToIgnore);

	TArray<FOverlapResult> Overlaps;
	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::LogAndReturnNull))
	{
		World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity,
		                                FCollisionObjectQueryParams(
			                                FCollisionObjectQueryParams::InitType::AllDynamicObjects),
		                                FCollisionShape::MakeSphere(Radius), CollisionParams);
		for (auto OverlapResults : Overlaps)
		{
			const bool ImplementsInterface = OverlapResults.GetActor()->Implements<UCombatInterface>();
			if (ImplementsInterface)
			{
				const bool IsAlive = !ICombatInterface::Execute_IsDead(OverlapResults.GetActor());
				if (IsAlive)
				{
					OverlappingActors.AddUnique(ICombatInterface::Execute_GetAvatar(OverlapResults.GetActor()));
				}
			}
		}
	}
}

bool UCombatBlueprintFunctionLibrary::IsNotFriend(AActor* FirstActor, AActor* SecondActor)
{
	const bool FirstIsPlayer = FirstActor->ActorHasTag(FName("Player"));
	const bool SecondIsPlayer = SecondActor->ActorHasTag(FName("Player"));
	const bool FirstIsEnemy = FirstActor->ActorHasTag(FName("Enemy"));
	const bool SecondIsEnemy = SecondActor->ActorHasTag(FName("Enemy"));

	const bool BothArePlayers = FirstIsPlayer && SecondIsPlayer;
	const bool BothAreEnemies = FirstIsEnemy && SecondIsEnemy;

	const bool bFriends = BothArePlayers || BothAreEnemies;
	return !bFriends;
}

UCombatCharacterClassInfo* UCombatBlueprintFunctionLibrary::GetCharacterClassInfo(const UObject* WorldContext)
{
	const ACombatGameModeBase* CombatGameModeBase = Cast<ACombatGameModeBase>(
		UGameplayStatics::GetGameMode(WorldContext));

	if (CombatGameModeBase == nullptr)return nullptr;

	return CombatGameModeBase->CharacterClassInfo;
}
