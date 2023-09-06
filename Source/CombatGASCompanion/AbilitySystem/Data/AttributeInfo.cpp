// Copyright SuspiciousDinosaur @ Youtube


#include "AttributeInfo.h"

FCombatAttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFount) const
{
	for(const FCombatAttributeInfo& Info: AttributeInformation)
	{
		if(Info.AttributeTag.MatchesTagExact(AttributeTag))
		{
			return Info;
		}
	}
	if(bLogNotFount)
	{
		UE_LOG(LogTemp,Error,TEXT("Cant Find info for attribute tag [%s] on AttributeInfo [%s]"),*AttributeTag.ToString(),*GetNameSafe(this));
	}
	return FCombatAttributeInfo();
}
