// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatHUD.h"

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
			FVector2d Spread(0.f,0.f);
			DrawCrosshair(HUDPackage.CrosshairsCenter, ViewportCenter,Spread);
		}
		if (HUDPackage.CrosshairsTop)
		{
			FVector2d Spread(0.f,-SpreadScaledY);
			DrawCrosshair(HUDPackage.CrosshairsTop, ViewportCenter,Spread);
		}
		if (HUDPackage.CrosshairsBottom)
		{
			FVector2d Spread(0.f,SpreadScaledY);
			DrawCrosshair(HUDPackage.CrosshairsBottom, ViewportCenter,Spread);
		}
		if (HUDPackage.CrosshairsLeft)
		{
			FVector2d Spread(-SpreadScaledX,0.f);
			DrawCrosshair(HUDPackage.CrosshairsLeft, ViewportCenter,Spread);
		}
		if (HUDPackage.CrosshairsRight)
		{
			FVector2d Spread(SpreadScaledX,0.f);
			DrawCrosshair(HUDPackage.CrosshairsRight, ViewportCenter,Spread);
		}
	}
}

void ACombatHUD::DrawCrosshair(UTexture2D* Texture, FVector2d ViewportCenter,FVector2d Spread)
{
	const float TextureWidth = Texture->GetSizeX();
	const float TextureHeight = Texture->GetSizeY();

	const FVector2d TextureDrawPoint(ViewportCenter.X - (TextureWidth / 2.f) + Spread.X, ViewportCenter.Y - (TextureHeight / 2.f)+Spread.Y);

	DrawTexture(Texture, TextureDrawPoint.X, TextureDrawPoint.Y, TextureWidth, TextureHeight, 0.f, 0.f, 1.f, 1.f,
	            FLinearColor::White);

	
}
