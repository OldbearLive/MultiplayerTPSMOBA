// Copyright SuspiciousDinosaur @ Youtube


#include "LevelUpInfo.h"

int32 ULevelUpInfo::FindLevelforExp(int32 inExp)
{
	int32 Level = 1;
	bool bSearching = true;
	while (bSearching)
	{
		if (LevelUpInfos.Num() - 1 <= Level)
		{
			//LevelupInfo[0] Placeholder Level info
			return Level;
		}
		if (inExp >= LevelUpInfos[Level].RequiredEXP)
		{
			++Level;
		}
		else
		{
			bSearching = false;
		}
	}
	return Level;
}
