// Copyright SuspiciousDinosaur @ Youtube


#include "CombatAssetManager.h"

#include "AbilitySystemGlobals.h"
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

	//REQUIRED TO USE TARGET DATA IN PROJECT
	UAbilitySystemGlobals::Get().InitGlobalData();
}
