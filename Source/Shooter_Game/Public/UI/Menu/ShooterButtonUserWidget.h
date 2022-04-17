// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShooterCoreTypes.h"
#include "ShooterButtonUserWidget.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class SHOOTER_GAME_API UShooterButtonUserWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    FText ButtonText;

    UPROPERTY(BlueprintAssignable)
    FOnClickedButtonSignature OnClickedButton;

protected:
    UPROPERTY(Meta = (BindWidget))
    UButton* Button = nullptr;

    UPROPERTY(Meta = (BindWidget))
    UTextBlock* ButtonTextBlock = nullptr;

    virtual void NativeOnInitialized() override;
    virtual void NativePreConstruct() override;

private:
    UFUNCTION()
    void OnClicked();
};
