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
	bReplicates = true;


	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	//Create the Weapon Mesh in constructor and validate the pointer in the header
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Mesh"));

	WeaponMesh->SetupAttachment(Root);
	WeaponMesh->SetCollisionResponseToAllChannels(ECR_Block);
	WeaponMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void ACombatRangedWeapon::BeginPlay()
{
	Super::BeginPlay();
}

FVector ACombatRangedWeapon::GetCombatSocketLocation_Implementation()
{
	return WeaponMesh->GetSocketLocation(WeaponTipSocket);
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

	DOREPLIFETIME_CONDITION(ACombatRangedWeapon, WeaponPropertiesStructure, COND_None);
}

void ACombatRangedWeapon::Fire()
{
	if (WeaponPropertiesStructure.WeaponFireMontage == nullptr)return;
	{
		WeaponMesh->PlayAnimation(WeaponPropertiesStructure.WeaponFireMontage, false);
	}
}
