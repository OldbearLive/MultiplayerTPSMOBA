// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatHUD.h"

#include "Blueprint/UserWidget.h"
#include "CombatGASCompanion/UI/AttributeWidgetController.h"
#include "CombatGASCompanion/UI/CombatUserWidget.h"
#include "CombatGASCompanion/UI/OverlayWidgetController.h"

void ACombatHUD::DrawHUD()
{
	Super::DrawHUD();

	FVector2d ViewPortSize;
	if (GEngine)
	{
		GEngine->GameViewport->GetViewportSize(ViewPortSize);
		const FVector2d ViewportCenter(ViewPortSize.X / 2.f, ViewPortSize.Y / 2.f);

		float SpreadScaledX = CrosshairSpreadMaxX * HUDPackage.CrosshairSpreadX;
		float SpreadScaledY = CrosshairSpreadMaxY * HUDPackage.CrosshairSpreadY;

		if (HUDPackage.CrosshairsCenter)
		{
			FVector2d Spread(0.f, 0.f);
			DrawCrosshair(HUDPackage.CrosshairsCenter, ViewportCenter, Spread, HUDPackage.CrosshairColor);
		}
		if (HUDPackage.CrosshairsTop)
		{
			FVector2d Spread(0.f, -SpreadScaledY);
			DrawCrosshair(HUDPackage.CrosshairsTop, ViewportCenter, Spread, HUDPackage.CrosshairColor);
		}
		if (HUDPackage.CrosshairsBottom)
		{
			FVector2d Spread(0.f, SpreadScaledY);
			DrawCrosshair(HUDPackage.CrosshairsBottom, ViewportCenter, Spread, HUDPackage.CrosshairColor);
		}
		if (HUDPackage.CrosshairsLeft)
		{
			FVector2d Spread(-SpreadScaledX, 0.f);
			DrawCrosshair(HUDPackage.CrosshairsLeft, ViewportCenter, Spread, HUDPackage.CrosshairColor);
		}
		if (HUDPackage.CrosshairsRight)
		{
			FVector2d Spread(SpreadScaledX, 0.f);
			DrawCrosshair(HUDPackage.CrosshairsRight, ViewportCenter, Spread, HUDPackage.CrosshairColor);
		}
	}
}


UOverlayWidgetController* ACombatHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if(OverlayWidgetController== nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this,OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->BindCallbacksToDependencies();
		return  OverlayWidgetController;
	}
	return OverlayWidgetController;
}

void ACombatHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass,TEXT("OverlayWidgetClass Uninitialized, Please Fillout HUD Class"))
	//checkf(OverlayWidgetController,TEXT("OverlayWidgetController Uninitialized, Please Fillout HUD Class"))
	
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(),OverlayWidgetClass);

	OverlayWidget= Cast<UCombatUserWidget>(Widget);

	const FWidgetControllerParams WidgetControllerParams(PC,PS,ASC,AS);
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);

	OverlayWidget->SetWidgetController(WidgetController);

	WidgetController->BroadcastInitialValues();
	
	Widget->AddToViewport();
}

UAttributeWidgetController* ACombatHUD::GetAttributeWidgetController(const FWidgetControllerParams& WCParams)
{
	if(AttributeWidgetController== nullptr)
	{
		AttributeWidgetController= NewObject<UAttributeWidgetController>(this,AttributeWidgetControllerClass);
		AttributeWidgetController->SetWidgetControllerParams(WCParams);
		AttributeWidgetController->BindCallbacksToDependencies();
	}
	return AttributeWidgetController;
}

void ACombatHUD::DrawCrosshair(UTexture2D* Texture, FVector2d ViewportCenter, FVector2d Spread,
                               FLinearColor CrosshairColor)
{
	const float TextureWidth = Texture->GetSizeX();
	const float TextureHeight = Texture->GetSizeY();

	const FVector2d TextureDrawPoint(ViewportCenter.X - (TextureWidth / 2.f) + Spread.X,
	                                 ViewportCenter.Y - (TextureHeight / 2.f) + Spread.Y);

	DrawTexture(Texture, TextureDrawPoint.X, TextureDrawPoint.Y, TextureWidth, TextureHeight, 0.f, 0.f, 1.f, 1.f,
	            CrosshairColor);
}
