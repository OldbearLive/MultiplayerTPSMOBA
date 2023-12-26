// Copyright SuspiciousDinosaur @ Youtube


#include "AbilityInfo.h"

FCombatAbilityInfo UAbilityInfo::FindAbilityInfoForTag(const FGameplayTag& AbilityTag, bool bLogNotFound)
{
	for (const FCombatAbilityInfo& Info : CombatAbilityInfos)
	{
		if (Info.AbilityTag == AbilityTag)
		{
			return Info;
		}
	}
	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't Find Ability with %s"), *AbilityTag.ToString());
	}
	return FCombatAbilityInfo();
}
