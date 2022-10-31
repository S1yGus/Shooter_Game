// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SHGUICoreTypes.h"
#include "SHGButtonUserWidget.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class SHOOTER_GAME_API USHGButtonUserWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    FOnClickedButtonSignature OnClickedButton;

    void SetText(const FText& NewText);

protected:
    UPROPERTY(Meta = (BindWidget))
    UButton* Button;

    UPROPERTY(Meta = (BindWidget))
    UTextBlock* ButtonTextBlock;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    FText ButtonText;

    virtual void NativeOnInitialized() override;
    virtual void NativePreConstruct() override;

private:
    UFUNCTION()
    void OnClicked();
};
