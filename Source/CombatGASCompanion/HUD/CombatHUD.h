// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatGASCompanion/UI/CombatWidgetController.h"
#include "GameFramework/HUD.h"
#include "CombatHUD.generated.h"


class UOverlayWidgetController;
class UCombatUserWidget;
class UAttributeWidgetController;

struct FWidgetControllerParams;


USTRUCT(BlueprintType)
struct FHUDPackage
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	class UTexture2D* CrosshairsCenter = nullptr;
	UPROPERTY(EditDefaultsOnly)
	UTexture2D* CrosshairsTop = nullptr;
	UPROPERTY(EditDefaultsOnly)
	UTexture2D* CrosshairsBottom = nullptr;
	UPROPERTY(EditDefaultsOnly)
	UTexture2D* CrosshairsLeft = nullptr;
	UPROPERTY(EditDefaultsOnly)
	UTexture2D* CrosshairsRight = nullptr;
	UPROPERTY(EditDefaultsOnly)
	float CrosshairSpreadX = 0.f;
	UPROPERTY(EditDefaultsOnly)
	float CrosshairSpreadY = 0.f;
	UPROPERTY(EditDefaultsOnly)
	FLinearColor CrosshairColor = FLinearColor::Red;
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
	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);

	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

	UAttributeWidgetController* GetAttributeWidgetController(const FWidgetControllerParams& WCParams);

private:
	UPROPERTY()
	TObjectPtr<UCombatUserWidget> OverlayWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCombatUserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UAttributeWidgetController> AttributeWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAttributeWidgetController> AttributeWidgetControllerClass;
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
