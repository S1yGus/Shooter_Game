// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShooterRaundInfoUserWidget.generated.h"

class UTextBlock;

UCLASS()
class SHOOTER_GAME_API UShooterRaundInfoUserWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    FText GetCurrentRaundTime() const;

protected:
    UPROPERTY(Meta = (BindWidget))
    UTextBlock* RaundInfoTextBlock = nullptr;

    virtual void NativeOnInitialized() override;

private:
    void OnNewPawn(APawn* NewPawn);

    void UpdateRaundInfo(int32 CurrentRaund, int32 RaundsNum);
};
