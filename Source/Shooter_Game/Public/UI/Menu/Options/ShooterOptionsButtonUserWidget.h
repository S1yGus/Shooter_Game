// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShooterCoreTypes.h"
#include "ShooterOptionsButtonUserWidget.generated.h"

class UButton;
class UTextBlock;
class UImage;

UCLASS()
class SHOOTER_GAME_API UShooterOptionsButtonUserWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    FOnClickedOptionsButtonSignature OnClickedOptionsButton;

    void SetOptionsButtonData(const FOptionsButtonData& NewOptionsButtonData);
    const FOptionsButtonData& GetOptionsButtonData() const { return OptionsButtonData; }

    void SetSelected(bool IsSelected);

protected:
    UPROPERTY(Meta = (BindWidget))
    UButton* OptionsButton = nullptr;

    UPROPERTY(Meta = (BindWidget))
    UImage* FrameImage = nullptr;

    UPROPERTY(Meta = (BindWidget))
    UTextBlock* ButtonTextBlock = nullptr;

    virtual void NativeOnInitialized() override;

private:
    FOptionsButtonData OptionsButtonData;

    UFUNCTION()
    void OnClicked();
};
