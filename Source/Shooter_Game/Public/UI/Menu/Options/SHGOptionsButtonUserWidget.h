// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SHGUICoreTypes.h"
#include "SHGOptionsButtonUserWidget.generated.h"

class UButton;
class UTextBlock;
class UImage;

UCLASS()
class SHOOTER_GAME_API USHGOptionsButtonUserWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    FOnClickedOptionsButtonSignature OnClickedOptionsButton;

    void Init(const FText& OptionDisplayName, int32 InWidgetID);
    void SetSelected(bool bIsSelected);

    int32 GetWidgetID() const { return WidgetID; }

protected:
    UPROPERTY(Meta = (BindWidget))
    UButton* OptionsButton;

    UPROPERTY(Meta = (BindWidget))
    UImage* FrameImage;

    UPROPERTY(Meta = (BindWidget))
    UTextBlock* ButtonTextBlock;

    virtual void NativeOnInitialized() override;

private:
    int32 WidgetID = 0;

    UFUNCTION()
    void OnClicked();
};
