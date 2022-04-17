// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShooterCoreTypes.h"
#include "ShooterOptionsWarningUserWidget.generated.h"

class UShooterButtonUserWidget;

UCLASS()
class SHOOTER_GAME_API UShooterOptionsWarningUserWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    FOnCancelResolutionSettingsSignature OnCancelResolutionSettings;

    UFUNCTION(BlueprintCallable)
    int32 GetTimeRemains() const { return CancelSettingsTime; }

protected:
    UPROPERTY(Meta = (BindWidget))
    UShooterButtonUserWidget* SaveButton = nullptr;

    UPROPERTY(Meta = (BindWidget))
    UShooterButtonUserWidget* CancelButton = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    float CancelSettingsTime = 10.0f;

    virtual void NativeOnInitialized() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
    FTimerHandle WarningTimerHandle;

    UFUNCTION()
    void OnSaveSettings();

    UFUNCTION()
    void OnCancelSettings();
};
