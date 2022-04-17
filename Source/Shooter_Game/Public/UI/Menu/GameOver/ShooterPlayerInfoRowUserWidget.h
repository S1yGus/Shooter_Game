// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShooterPlayerInfoRowUserWidget.generated.h"

class UTextBlock;
class UImage;

UCLASS()
class SHOOTER_GAME_API UShooterPlayerInfoRowUserWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    void SetNameTextBlock(const FText& NewText);
    void SetKillsTextBlock(const FText& NewText);
    void SetDeathsTextBlock(const FText& NewText);
    void SetTeamTextBlock(const FText& NewText);
    void SetTeamColorImage(const FLinearColor& NewColor);
    void SetPlayerHighlighting(bool IsHighlighting);

protected:
    UPROPERTY(Meta = (BindWidget))
    UTextBlock* NameTextBlock = nullptr;

    UPROPERTY(Meta = (BindWidget))
    UTextBlock* KillsTextBlock = nullptr;

    UPROPERTY(Meta = (BindWidget))
    UTextBlock* DeathsTextBlock = nullptr;

    UPROPERTY(Meta = (BindWidget))
    UTextBlock* TeamTextBlock = nullptr;

    UPROPERTY(Meta = (BindWidget))
    UImage* PlayerHighlighting = nullptr;

    UPROPERTY(Meta = (BindWidget))
    UImage* TeamColorImage = nullptr;
};
