// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShooterCoreTypes.h"
#include "ShooterLevelSelectorUserWidget.generated.h"

class UHorizontalBox;
class UShooterLevelItemUserWidget;

UCLASS()
class SHOOTER_GAME_API UShooterLevelSelectorUserWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UShooterLevelItemUserWidget> LevelItemWidgetClass;

    UPROPERTY(Meta = (BindWidget))
    UHorizontalBox* LevelItemsBox = nullptr;

    virtual void NativeOnInitialized() override;

private:
    UPROPERTY()
    TArray<UShooterLevelItemUserWidget*> LevelItemWidgets;

    void OnSelectedLevel(const FLevelData& Data);

    void FillLevelItemsBox();
};
