// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SHGUICoreTypes.h"
#include "SHGLevelSelectorUserWidget.generated.h"

class UHorizontalBox;
class USHGLevelItemUserWidget;

UCLASS()
class SHOOTER_GAME_API USHGLevelSelectorUserWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<USHGLevelItemUserWidget> LevelItemWidgetClass;

    UPROPERTY(Meta = (BindWidget))
    UHorizontalBox* LevelItemsBox;

    virtual void NativeOnInitialized() override;

private:
    UPROPERTY()
    TArray<USHGLevelItemUserWidget*> LevelItemWidgets;

    void OnSelectedLevel(const FLevelData& Data);

    void FillLevelItemsBox();
};
