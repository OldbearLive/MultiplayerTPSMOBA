// Copyright SuspiciousDinosaur @ Youtube

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CombatCharacterClassInfo.generated.h"


class UGameplayEffect;

UENUM(BlueprintType)
enum class ECharacterClass:uint8
{
	BasicMinion
};

USTRUCT(BlueprintType)
struct FCharacterClassInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly,Category="Class Defaults")
	TSubclassOf<UGameplayEffect> PrimaryAttributes;
	
};


/**
 * 
 */
UCLASS()
class COMBATGASCOMPANION_API UCombatCharacterClassInfo : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly,Category=" Character Class Defaults")
	TMap<ECharacterClass,FCharacterClassInfo> CharacterClassInformation;

	UPROPERTY(EditDefaultsOnly,Category="Common Class Defaults")
	TSubclassOf<UGameplayEffect> SecondaryAttributes;

	UPROPERTY(EditDefaultsOnly,Category="Common Class Defaults")
	TSubclassOf<UGameplayEffect> VItalAttributes;


	UFUNCTION()
	FCharacterClassInfo GetClassDefaultsInfo(ECharacterClass CharacterClass);

	
	
};
