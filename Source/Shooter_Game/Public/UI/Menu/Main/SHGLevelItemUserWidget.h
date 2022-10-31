// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SHGUICoreTypes.h"
#include "SHGLevelItemUserWidget.generated.h"

class UImage;
class UTextBlock;
class UButton;

UCLASS()
class SHOOTER_GAME_API USHGLevelItemUserWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    FOnSelectedLevelSignature OnSelectedLevel;

    const FLevelData& GetLevelData() const { return LevelData; }
    void SetLevelData(const FLevelData& Data);
    void SetSelected(bool IsSelected);

protected:
    UPROPERTY(Meta = (BindWidget))
    UImage* FrameImage;

    UPROPERTY(Meta = (BindWidget))
    UImage* LevelImage;

    UPROPERTY(Meta = (BindWidget))
    UTextBlock* LevelNameTextBlock;

    UPROPERTY(Meta = (BindWidget))
    UButton* LevelSelectButton;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FLinearColor HoveredTint{0.05f, 1.0f, 0.2f, 1.0f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FLinearColor UnhoveredTint{1.0f, 1.0f, 1.0f, 0.7f};

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
