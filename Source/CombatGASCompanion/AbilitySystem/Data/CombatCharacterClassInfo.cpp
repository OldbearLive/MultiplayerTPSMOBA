// Copyright SuspiciousDinosaur @ Youtube

 
#include "CombatCharacterClassInfo.h"

FCharacterClassInfo UCombatCharacterClassInfo::GetClassDefaultsInfo(ECharacterClass CharacterClass)
{
	return CharacterClassInformation.FindChecked(CharacterClass);
}
