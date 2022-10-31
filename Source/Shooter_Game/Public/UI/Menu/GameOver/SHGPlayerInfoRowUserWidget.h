// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SHGPlayerInfoRowUserWidget.generated.h"

class UTextBlock;
class UImage;

UCLASS()
class SHOOTER_GAME_API USHGPlayerInfoRowUserWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    void SetName(const FText& NewText);
    void SetKills(const FText& NewText);
    void SetDeaths(const FText& NewText);
    void SetTeam(const FText& NewText);
    void SetTeamColor(const FLinearColor& NewColor);
    void SetPlayerHighlighting(bool IsHighlighting);

protected:
    UPROPERTY(Meta = (BindWidget))
    UTextBlock* NameTextBlock;

    UPROPERTY(Meta = (BindWidget))
    UTextBlock* KillsTextBlock;

    UPROPERTY(Meta = (BindWidget))
    UTextBlock* DeathsTextBlock;

    UPROPERTY(Meta = (BindWidget))
    UTextBlock* TeamTextBlock;

    UPROPERTY(Meta = (BindWidget))
    UImage* PlayerHighlighting;

    UPROPERTY(Meta = (BindWidget))
    UImage* TeamColorImage;

    virtual void NativeOnInitialized() override;
};
