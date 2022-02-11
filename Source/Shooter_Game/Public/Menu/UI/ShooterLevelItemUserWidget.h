// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShooterCoreTypes.h"
#include "ShooterLevelItemUserWidget.generated.h"

class UImage;
class UTextBlock;
class UButton;

UCLASS()
class SHOOTER_GAME_API UShooterLevelItemUserWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    FOnSelectedLevelSignature OnSelectedLevel;

    FLevelData GetLevelData() const { return LevelData; }
    void SetLevelData(const FLevelData& Data);

    void SetSelected(bool IsSelected);

protected:
    UPROPERTY(Meta = (BindWidget))
    UImage* FrameImage = nullptr;

    UPROPERTY(Meta = (BindWidget))
    UImage* LevelImage = nullptr;

    UPROPERTY(Meta = (BindWidget))
    UTextBlock* LevelNameTextBlock = nullptr;

    UPROPERTY(Meta = (BindWidget))
    UButton* LevelSelectButton = nullptr;

    virtual void NativeOnInitialized() override;

private:
    FLevelData LevelData;

    UFUNCTION()
    void OnLevelItemClicked();

    UFUNCTION()
    void OnLevelItemHovered();

    UFUNCTION()
    void OnLevelItemUnhovered();
};
