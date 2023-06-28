// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CombatHUD.generated.h"


USTRUCT(BlueprintType)
struct FHUDPackage
{
	GENERATED_BODY()
public:
	class UTexture2D * CrosshairsCenter;
	UTexture2D * CrosshairsTop;
	UTexture2D * CrosshairsBottom;
	UTexture2D * CrosshairsLeft;
	UTexture2D * CrosshairsRight;
};
/**
 * 
 */
UCLASS()
class COMBATGASCOMPANION_API ACombatHUD : public AHUD
{
	GENERATED_BODY()


public:
	virtual  void DrawHUD() override;

	void DrawCrosshair(UTexture2D* Texture,FVector2d ViewportCenter);

private:
	FHUDPackage HUDPackage;
public:
	FORCEINLINE void SetHUDPackage(const FHUDPackage& Package){ HUDPackage = Package;}
};
