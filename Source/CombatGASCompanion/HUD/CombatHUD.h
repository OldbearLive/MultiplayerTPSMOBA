// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatGASCompanion/UI/CombatWidgetController.h"
#include "GameFramework/HUD.h"
#include "CombatHUD.generated.h"


class UOverlayWidgetController;
class UCombatUserWidget;

struct FWidgetControllerParams;


USTRUCT(BlueprintType)
struct FHUDPackage
{
	GENERATED_BODY()

public:
	class UTexture2D* CrosshairsCenter;
	UTexture2D* CrosshairsTop;
	UTexture2D* CrosshairsBottom;
	UTexture2D* CrosshairsLeft;
	UTexture2D* CrosshairsRight;
	float CrosshairSpreadX;
	float CrosshairSpreadY;
	FLinearColor CrosshairColor;
};

/**
 * 
 */
UCLASS()
class COMBATGASCOMPANION_API ACombatHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;

protected:


	/*
	 *
	 *Overlay WidgetDrawing Functionality
	 * 
	 */
public:

	UPROPERTY()
	TObjectPtr<UCombatUserWidget> OverlayWidget;

	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);

	void InitOverlay(APlayerController*PC,APlayerState*PS,UAbilitySystemComponent*ASC,UAttributeSet*AS);

private:

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCombatUserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController>OverlayWidgetControllerClass;
	
	/*
	 *
	 *Crosshair Drawing Functionality
	 * 
	 */

public:
	void DrawCrosshair(UTexture2D* Texture, FVector2d ViewportCenter, FVector2d Spread, FLinearColor CrosshairColor);

	FORCEINLINE void SetHUDPackage(const FHUDPackage& Package) { HUDPackage = Package; }

private:
	FHUDPackage HUDPackage;

	UPROPERTY(EditAnywhere)
	float CrosshairSpreadMaxX = 16.f;
	UPROPERTY(EditAnywhere)
	float CrosshairSpreadMaxY = 16.f;
};
