// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatRangedWeapon.h"

#include "CombatGASCompanion/Character/CombatCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ACombatRangedWeapon::ACombatRangedWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	SetReplicates(true);
	bReplicates = true;


	//Create the Weapon Mesh in constructor and validate the pointer in the header
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Mesh"));
	SetRootComponent(WeaponMesh);

	WeaponMesh->SetCollisionResponseToAllChannels(ECR_Block);
	WeaponMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	
}

// Called when the game starts or when spawned
void ACombatRangedWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACombatRangedWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//REPLICATION REGISTRATION
void ACombatRangedWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);


}

void ACombatRangedWeapon::Fire(const FVector& HitTarget)
{
	if (FireAnimation)
	{
		WeaponMesh->PlayAnimation(FireAnimation, false);
	}
}





