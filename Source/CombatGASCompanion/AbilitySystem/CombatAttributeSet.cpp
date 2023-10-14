// Copyright SuspiciousDinosaur @ Youtube


#include "CombatAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "CombatBlueprintFunctionLibrary.h"
#include "GameplayEffectExtension.h"
#include "CombatGASCompanion/CombatGameplayTagsSingleton.h"
#include "CombatGASCompanion/PlayerController/CombatPlayerController.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

UCombatAttributeSet::UCombatAttributeSet()
{
	const FCombatGameplayTags& GameplayTags = FCombatGameplayTags::Get();

	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Spirit, GetSpiritAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Technique, GetTechniqueAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Toughness, GetToughnessAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Tertiary_Battery, GetBatteryAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Tertiary_MaxBattery, GetMaxBatteryAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Vital_Energy, GetEnergyAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Vital_Health, GetHealthAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_EnergyEfficiency, GetEnergyEfficiencyAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_EnergyPenetration, GetEnergyPenetrationAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_EnergyRegeneration, GetEnergyRegenerationAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_HealthRegeneration, GetHealthRegenerationAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxEnergy, GetMaxEnergyAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxHealth, GetMaxHealthAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_OverloadChance, GetOverloadChanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_OverloadDamage, GetOverloadDamageAttribute);
	TagsToAttributes.Add(GameplayTags.Attrributes_Resistance_Energy, GetEnergyResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attrributes_Resistance_Physical, GetPhysicalResistanceAttribute);
}

void UCombatAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//VitalAttribute

	DOREPLIFETIME_CONDITION_NOTIFY(UCombatAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCombatAttributeSet, Energy, COND_None, REPNOTIFY_Always);


	//BatteryAttribute

	DOREPLIFETIME_CONDITION_NOTIFY(UCombatAttributeSet, Battery, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCombatAttributeSet, MaxBattery, COND_None, REPNOTIFY_Always);

	//PrimaryAttribute

	DOREPLIFETIME_CONDITION_NOTIFY(UCombatAttributeSet, Spirit, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCombatAttributeSet, Toughness, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCombatAttributeSet, Technique, COND_None, REPNOTIFY_Always);

	//SecondaryAttributes

	DOREPLIFETIME_CONDITION_NOTIFY(UCombatAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCombatAttributeSet, MaxEnergy, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCombatAttributeSet, EnergyEfficiency, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCombatAttributeSet, EnergyPenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCombatAttributeSet, OverloadChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCombatAttributeSet, OverloadDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCombatAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCombatAttributeSet, EnergyRegeneration, COND_None, REPNOTIFY_Always);

	//Resistance Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UCombatAttributeSet, PhysicalResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCombatAttributeSet, EnergyResistance, COND_None, REPNOTIFY_Always);

	
}

void UCombatAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	//Before Attributechange occurs through any means, Clamping and maybe UI?
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
 
	if (Attribute == GetEnergyAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxEnergy());
	}
}

void UCombatAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props)
{
	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if (IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo->
		AvatarActor.
		IsValid())
	{
		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();

		if (Props.SourceController == nullptr && Props.SourceAvatarActor != nullptr)
		{
			if (APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
			{
				Props.SourceController = Pawn->GetController();
			}
		}
		if (Props.SourceController)
		{
			Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
		}
	}

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.
	                                                   IsValid())
	{
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}
}


void UCombatAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	//Executed after Changes happen to the attribute

	FEffectProperties Props;
	SetEffectProperties(Data, Props);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}
	if (Data.EvaluatedData.Attribute == GetEnergyAttribute())
	{
		SetEnergy(FMath::Clamp(GetEnergy(), 0.f, GetMaxEnergy()));
		ShowFloatingText(Props, Data.EvaluatedData.Magnitude,
		                 UCombatBlueprintFunctionLibrary::IsShieldHit(Props.EffectContextHandle),
		                 UCombatBlueprintFunctionLibrary::IsOverloadHit(Props.EffectContextHandle));
	}

	//SET INCOMING DAMAGE ATTRIBUTE ON SERVER AND USE IT TO PERFORM DAMAGE CALCULATION
	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		const float LocalIncomingDamage = GetIncomingDamage();
		SetIncomingDamage(0.f);
		if (LocalIncomingDamage > 0.f)
		{
			const float NewHealth = GetHealth() - LocalIncomingDamage;
			SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

			const bool bFatal = NewHealth <= 0.f;
			if (bFatal)
			{
				if (!Props.TargetASC->HasMatchingGameplayTag(FCombatGameplayTags::Get().Death))
				{
					FGameplayTagContainer TagContainer;
					TagContainer.AddTag(FCombatGameplayTags::Get().Death);
					Props.TargetASC->TryActivateAbilitiesByTag(TagContainer);
				}
			}
			else
			{
				FGameplayTagContainer TagContainer;
				TagContainer.AddTag(FCombatGameplayTags::Get().HitReact);
				Props.TargetASC->TryActivateAbilitiesByTag(TagContainer);
			}
			if (Props.SourceAvatarActor != Props.TargetAvatarActor)
			{
				const bool bShield = UCombatBlueprintFunctionLibrary::IsShieldHit(Props.EffectContextHandle);
				const bool bOverload = UCombatBlueprintFunctionLibrary::IsOverloadHit(Props.EffectContextHandle);
				ShowFloatingText(Props, LocalIncomingDamage, bShield, bOverload);
			}
		}
	}
}

void UCombatAttributeSet::ShowFloatingText(const FEffectProperties& Properties, float Damage, bool bIsShieldHit,
                                           bool IsOverloadHit) const
{
	ACombatPlayerController* LocalPlayerController = Cast<ACombatPlayerController>(Properties.SourceController);
	if (LocalPlayerController)
	{
		LocalPlayerController->ShowDamageNumber(Damage, Properties.TargetAvatarActor,bIsShieldHit,IsOverloadHit);
	}
}


// Vital Attributes Section Start
void UCombatAttributeSet::OnRep_Health(const FGameplayAttributeData OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCombatAttributeSet, Health, OldHealth);
}

void UCombatAttributeSet::OnRep_Energy(const FGameplayAttributeData OldEnergy) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCombatAttributeSet, Energy, OldEnergy);
}

// Vital Attributes Section End


// Battery Attributes Section Start
void UCombatAttributeSet::OnRep_Battery(const FGameplayAttributeData OldBattery) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCombatAttributeSet, Battery, OldBattery);
}

void UCombatAttributeSet::OnRep_MaxBattery(const FGameplayAttributeData OldMaxBattery) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCombatAttributeSet, MaxBattery, OldMaxBattery);
}

// Battery Attributes Section End


// Primary Attributes Section Start
void UCombatAttributeSet::OnRep_Toughness(const FGameplayAttributeData OldToughness) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCombatAttributeSet, Toughness, OldToughness);
}

void UCombatAttributeSet::OnRep_Technique(const FGameplayAttributeData OldTechnique) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCombatAttributeSet, Technique, OldTechnique);
}

void UCombatAttributeSet::OnRep_Spirit(const FGameplayAttributeData OldSpirit) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCombatAttributeSet, Spirit, OldSpirit);
}

// Primary Attributes Section End


// Secondary Attributes Section Start
void UCombatAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCombatAttributeSet, MaxHealth, OldMaxHealth);
}

void UCombatAttributeSet::OnRep_MaxEnergy(const FGameplayAttributeData OldMaxEnergy) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCombatAttributeSet, MaxEnergy, OldMaxEnergy);
}

void UCombatAttributeSet::OnRep_EnergyEfficiency(const FGameplayAttributeData OldEnergyEfficiency) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCombatAttributeSet, EnergyEfficiency, OldEnergyEfficiency);
}

void UCombatAttributeSet::OnRep_EnergyPenetration(const FGameplayAttributeData OldEnergyPenetration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCombatAttributeSet, EnergyPenetration, OldEnergyPenetration);
}

void UCombatAttributeSet::OnRep_OverloadChance(const FGameplayAttributeData OldOverloadChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCombatAttributeSet, OverloadChance, OldOverloadChance);
}

void UCombatAttributeSet::OnRep_OverloadDamage(const FGameplayAttributeData OldOverloadDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCombatAttributeSet, OverloadDamage, OldOverloadDamage);
}

void UCombatAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData OldHealthRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCombatAttributeSet, HealthRegeneration, OldHealthRegeneration);
}

void UCombatAttributeSet::OnRep_EnergyRegeneration(const FGameplayAttributeData OldEnergyRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCombatAttributeSet, EnergyPenetration, OldEnergyRegeneration);
}

void UCombatAttributeSet::OnRep_EnergyResistance(const FGameplayAttributeData OldEnergyResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCombatAttributeSet, EnergyResistance, OldEnergyResistance);
}

void UCombatAttributeSet::OnRep_PhysicalResistance(const FGameplayAttributeData OldPhysicalResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCombatAttributeSet, PhysicalResistance, OldPhysicalResistance);
}

// Secondary Attributes Section End
