// Copyright SuspiciousDinosaur @ Youtube


#include "CombatAssetManager.h"

#include "CombatGameplayTagsSingleton.h"

UCombatAssetManager& UCombatAssetManager::Get()
{
	check(GEngine);

	UCombatAssetManager* CombatAssetManager = Cast<UCombatAssetManager>(GEngine->AssetManager);
	return *CombatAssetManager;
}

void UCombatAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FCombatGameplayTags::InitializeNativeGameplayTags();
}
