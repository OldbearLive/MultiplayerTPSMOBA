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


	//Create the Weapon Mesh in constructor and validate the pointer in the header
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT ("Weapon Mesh"));
	SetRootComponent(WeaponMesh);

	WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,ECollisionResponse::ECR_Ignore);

	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Area Sphere"));
	AreaSphere->SetupAttachment(RootComponent);
	AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	PickupWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget"));
	PickupWidget->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACombatRangedWeapon::BeginPlay()
{
	Super::BeginPlay();

	if(HasAuthority())
	{
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		AreaSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,ECollisionResponse::ECR_Overlap);
		AreaSphere->OnComponentBeginOverlap.AddDynamic(this,&ACombatRangedWeapon::OnSphereOverlap);
		AreaSphere->OnComponentEndOverlap.AddDynamic(this,&ACombatRangedWeapon::OnSphereEndOverlap);
	}
	
	if(PickupWidget)
	{
		PickupWidget->SetVisibility(false);
	}
}



void ACombatRangedWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	

	ACombatCharacter*CombatCharacter = Cast<ACombatCharacter>(OtherActor);
	if(CombatCharacter && PickupWidget)
	{
		CombatCharacter->SetOverlappingWeapon(this);
	}
}

void ACombatRangedWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACombatCharacter*CombatCharacter = Cast<ACombatCharacter>(OtherActor);
	if(CombatCharacter)
	{
		CombatCharacter->SetOverlappingWeapon(nullptr);
	}
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

	DOREPLIFETIME(ACombatRangedWeapon,WeaponStates);
}


void ACombatRangedWeapon::ShowPickupWidget(bool bShowWidget)
{
	if(PickupWidget)
	{
		PickupWidget->SetVisibility(bShowWidget);
	}
}

void ACombatRangedWeapon::OnRep_WeaponStates()
{

	switch (WeaponStates)
	{
	case  ERangedWeaponStates::ERWS_Equipped:
		ShowPickupWidget(false);
//TestingForAnimLayers
		
		break;
	}
}


void ACombatRangedWeapon::SetWeaponState(ERangedWeaponStates State)
{
	WeaponStates=State;
	switch (WeaponStates)
	{
	case  ERangedWeaponStates::ERWS_Equipped:
		ShowPickupWidget(false);
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	}
}



