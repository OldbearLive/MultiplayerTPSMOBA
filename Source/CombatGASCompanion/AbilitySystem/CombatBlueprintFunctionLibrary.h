// Copyright SuspiciousDinosaur @ Youtube

#pragma once

#include "CoreMinimal.h"
#include "Data/CombatCharacterClassInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CombatBlueprintFunctionLibrary.generated.h"

class UAbilitySystemComponent;
class UAttributeWidgetController;
class UOverlayWidgetController;
/**
 * 
 */
UCLASS()
class COMBATGASCOMPANION_API UCombatBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "CombatBlueprintFunctionLibrary")
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "CombatBlueprintFunctionLibrary")
	static UAttributeWidgetController* GetAttributeWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "CharacterClassDefaults")
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass,
	                                        float Level, UAbilitySystemComponent* ASC);


	UFUNCTION(BlueprintCallable, Category = "CharacterClassDefaults")
	static UCombatCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContext);


	UFUNCTION(BlueprintCallable, Category = "CharacterClassDefaults")
	static void GiveStartupAbilities(const UObject* WorldContext, UAbilitySystemComponent* AbilitySystemComponent,
	                                 ECharacterClass CharacterClass);

	UFUNCTION(BlueprintPure, Category = "GameplayEffects")
	static bool IsShieldHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "GameplayEffects")
	static bool IsOverloadHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable, Category = "GameplayEffects")
	static void SetIsShieldHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInShieldHit);

	UFUNCTION(BlueprintCallable, Category = "GameplayEffects")
	static void SetIsOverloadHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInOverloadHit);

	UFUNCTION(BlueprintCallable, Category = "Combat|Utilities")
	static void GetLivePlayersWithinRadius(const UObject* WorldContext, TArray<AActor*>& OverlappingActors,
	                                       const TArray<AActor*>& ActorsToIgnore, float Radius,
	                                       const FVector& SphereOrigin);

	UFUNCTION(BlueprintPure, Category = "Combat|Utilities")
	static bool IsNotFriend(AActor* FirstActor,AActor* SecondActor);

	static int32 XPRewardForClassAndLevel(const UObject* WorldContext,ECharacterClass CharacterClass, int32 InLevel);
};
