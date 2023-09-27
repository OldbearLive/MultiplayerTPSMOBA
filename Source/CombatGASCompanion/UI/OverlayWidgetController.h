// Copyright SuspiciousDinosaur @ Youtube

#pragma once

#include "CoreMinimal.h"
#include "CombatWidgetController.h"
#include "OverlayWidgetController.generated.h"

class UCombatUserWidget;

USTRUCT(BlueprintType)
struct FUIWidgetRow: public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FGameplayTag MessageTag = FGameplayTag();

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FText Message = FText();
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<UCombatUserWidget> MessageWidget;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	UTexture2D*Image = nullptr;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FLinearColor OutlineColor = FLinearColor::White;
};


//DELEGATES
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWudgetRowSignature,FUIWidgetRow,NewMaxEnergy);



/**
 * 
 */
UCLASS(BlueprintType,Blueprintable)
class COMBATGASCOMPANION_API UOverlayWidgetController : public UCombatWidgetController
{
	GENERATED_BODY()
public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable,Category = "GAS|UI|Attributes")
	FOnAttributeChangedSignature OnHealthChangedSignature;

	UPROPERTY(BlueprintAssignable,Category = "GAS|UI|Attributes")
	FOnAttributeChangedSignature OnMaxHealthChangedSignature;
	
	UPROPERTY(BlueprintAssignable,Category = "GAS|UI|Attributes")
	FOnAttributeChangedSignature OnEnergyChangedSignature;
	
	UPROPERTY(BlueprintAssignable,Category = "GAS|UI|Attributes")
	FOnAttributeChangedSignature OnMaxEnergyChangedSignature;

	UPROPERTY(BlueprintAssignable,Category = "GAS|UI|Messages")
	FMessageWudgetRowSignature MessageWudgetRowDelegate;

protected:

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Widget Data")
	TObjectPtr<UDataTable> MessageWidgetDataTable;
	
	
	template<typename T>
	T* GetDataTableByTag(UDataTable* DataTable, const FGameplayTag& Tag);
	
};

// TEMPLATE FUNCTION TO DEFINE MESSAGE DATA WITH TAG TO DISPLAY IT ON PICKUPS
template <typename T>
T* UOverlayWidgetController::GetDataTableByTag(UDataTable* DataTable, const FGameplayTag& Tag)
{
	T* Row = DataTable->FindRow<T>(Tag.GetTagName(), TEXT(""));
	if(Row)
	{
		return Row;
	}
	return nullptr;
}

