// Shooter_Game, All rights reserved.

#include "UI/Menu/SHGPopUpHintUserWidget.h"
#include "UI/Menu/SHGButtonUserWidget.h"
#include "UI/Menu/SHGTitleUserWidget.h"
#include "Components/TextBlock.h"
#include "SHGGameMode.h"
#include "Player/SHGPlayerController.h"

void USHGPopUpHintUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(ResumeButton);
    check(TitleText);
    check(HintTextBlock);

    ResumeButton->OnClickedButton.AddUObject(this, &ThisClass::OnClickedResumeButton);

    if (const auto GameMode = GetWorld()->GetAuthGameMode<ASHGGameMode>())
    {
        GameMode->OnShowPopUpHint.AddUObject(this, &ThisClass::OnShowPopUpHint);
    }

    if (const auto PC = GetOwningPlayer<ASHGPlayerController>())
    {
        PC->OnPressedEnt.AddUObject(this, &ThisClass::OnPressedEnter);
        PC->OnPressedEsc.AddUObject(this, &ThisClass::OnPressedEnter);
    }
}

void USHGPopUpHintUserWidget::OnShowPopUpHint(const FHintData& HintData)
{
    TitleText->SetText(HintData.Title);
    TitleText->SetColorAndOpacity(HintData.TitleColor);
    HintTextBlock->SetText(HintData.HintText);
}

void USHGPopUpHintUserWidget::OnPressedEnter()
{
    if (!IsVisible() || IsAnyAnimationPlaying())
        return;

    OnClickedResumeButton();
}

void USHGPopUpHintUserWidget::OnClickedResumeButton()
{
    ShowFadeoutAnimation();
}

void USHGPopUpHintUserWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
    Super::OnAnimationFinished_Implementation(Animation);

    if (Animation != FadeoutAnimation)
        return;

    if (const auto GameMode = GetWorld()->GetAuthGameMode<ASHGGameModeBase>())
    {
        GameMode->ClearPause();
    }
}
