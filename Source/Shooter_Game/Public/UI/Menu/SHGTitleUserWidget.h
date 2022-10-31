// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SHGTitleUserWidget.generated.h"

class UTextBlock;

UCLASS()
class SHOOTER_GAME_API USHGTitleUserWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    void SetText(const FText& NewText);
    void SetColor(const FLinearColor& NewColor);

protected:
    UPROPERTY(Meta = (BindWidget))
    UTextBlock* TextBlock;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    FText Text;

    virtual void NativePreConstruct() override;
};
