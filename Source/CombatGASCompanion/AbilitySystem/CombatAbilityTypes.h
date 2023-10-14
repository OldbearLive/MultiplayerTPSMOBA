// Copyright SuspiciousDinosaur @ Youtube
#pragma once

#include "GameplayEffectTypes.h"
#include "CombatAbilityTypes.generated.h"


USTRUCT(BlueprintType)
struct FCombatGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	bool IsOverloadHit() const {return bIsOverloadHit; }
	bool IsShieldHit() const {return bIsShieldHit; }

	void SetIsOverloadHit(bool InOverloadHit) { bIsOverloadHit = InOverloadHit; }
	void SetIsShieldHit(bool InShieldHit) { bIsShieldHit = InShieldHit; }
	
	/** Returns the actual struct used for serialization, subclasses must override this! */
	virtual UScriptStruct* GetScriptStruct() const
	{
		return FGameplayEffectContext::StaticStruct();
	}

	/** Creates a copy of this context, used to duplicate for later modifications */
	virtual FGameplayEffectContext* Duplicate() const
	{
		FGameplayEffectContext* NewContext = new FGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}

	/** Custom serialization, subclasses must override this */
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);

protected:
	UPROPERTY()
	bool bIsShieldHit = false;

	UPROPERTY()
	bool bIsOverloadHit = false;
};

template<>
struct TStructOpsTypeTraits<FCombatGameplayEffectContext>: TStructOpsTypeTraitsBase2<FCombatGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
	
};
