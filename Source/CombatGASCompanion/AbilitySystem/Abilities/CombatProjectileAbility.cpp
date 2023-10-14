// Copyright SuspiciousDinosaur @ Youtube


#include "CombatProjectileAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "CombatGASCompanion/CombatGameplayTagsSingleton.h"
#include "CombatGASCompanion/Actors/CombatProjectile.h"

void UCombatProjectileAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                               const FGameplayAbilityActorInfo* ActorInfo,
                                               const FGameplayAbilityActivationInfo ActivationInfo,
                                               const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UCombatProjectileAbility::SpawnProjectile(const FVector& ProjectileTargetLocation)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer)return;

	FRotator Rotation = (ProjectileTargetLocation - SpawnTransform.GetLocation()).Rotation();

	SpawnTransform.SetRotation(Rotation.Quaternion());
	ACombatProjectile* Projectile = GetWorld()->SpawnActorDeferred<ACombatProjectile>(
		ProjectileClass, SpawnTransform, GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	Projectile->SetOwner(GetAvatarActorFromActorInfo());

	const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(
		GetAvatarActorFromActorInfo());

	FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();
	EffectContextHandle.SetAbility(this);
	EffectContextHandle.AddSourceObject(Projectile);

	const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(
		DamageEffectClass, GetAbilityLevel(), EffectContextHandle);


	FCombatGameplayTags GameplayTags = FCombatGameplayTags::Get();


	for (auto& Pair : DamageTypes)
	{
		const float ScaledDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Pair.Key, ScaledDamage);
	}


	Projectile->DamageEffectSpecHandle = SpecHandle;


	Projectile->FinishSpawning(SpawnTransform);
}
