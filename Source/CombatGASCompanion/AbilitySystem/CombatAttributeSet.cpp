// Copyright SuspiciousDinosaur @ Youtube


#include "CombatAttributeSet.h"

#include "Net/UnrealNetwork.h"

UCombatAttributeSet::UCombatAttributeSet()
{
	InitHealth(100.f);
	InitMaxHealth(100.f);
	InitEnergy(100.f);
	InitMaxEnergy(100.f);
}

void UCombatAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UCombatAttributeSet,Health,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCombatAttributeSet,MaxHealth,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCombatAttributeSet,Energy,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCombatAttributeSet,MaxEnergy,COND_None,REPNOTIFY_Always);
}

void UCombatAttributeSet::OnRep_Health(const FGameplayAttributeData OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCombatAttributeSet,Health,OldHealth);
}
void UCombatAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCombatAttributeSet,MaxHealth,OldMaxHealth);
}

void UCombatAttributeSet::OnRep_Energy(const FGameplayAttributeData OldEnergy) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCombatAttributeSet,Energy,OldEnergy);
}

void UCombatAttributeSet::OnRep_MaxEnergy(const FGameplayAttributeData OldMaxEnergy) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCombatAttributeSet,MaxEnergy,OldMaxEnergy);
}
