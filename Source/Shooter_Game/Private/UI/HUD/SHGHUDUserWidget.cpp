// Shooter_Game, All rights reserved.

#include "UI/HUD/SHGHUDUserWidget.h"
#include "Components/ShooterHealthComponent.h"
#include "Components/ShooterStaminaComponent.h"
#include "Player/ShooterPlayerController.h"
#include "SHGGameModeArena.h"

void USHGHUDUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(GetDamageAnimation);
    check(OutOfStaminaAnimation);

    if (const auto PC = GetOwningPlayer<AShooterPlayerController>())
    {
        PC->GetOnNewPawnNotifier().AddUObject(this, &ThisClass::OnNewPawn);
        PC->OnPressedEsc.AddUObject(this, &ThisClass::OnPressedEsc);
    }

    if (const auto GameMode = GetWorld()->GetAuthGameMode<ASHGGameMode>())
    {
        GameMode->OnShowPopUpHint.AddUObject(this, &ThisClass::OnShowPopUpHint);
    }
}

void USHGHUDUserWidget::OnTakeAnyDamage(AActor* DamagedActor,             //
                                        float Damage,                     //
                                        const UDamageType* DamageType,    //
                                        AController* InstigatedBy,        //
                                        AActor* DamageCauser)             //
{
    const auto HealthComponent = DamagedActor->FindComponentByClass<UShooterHealthComponent>();
    if (!HealthComponent || HealthComponent->IsDead())
    {
        if (IsAnimationPlaying(GetDamageAnimation))
        {
            StopAnimation(GetDamageAnimation);
        }
        return;
    }

    ShowAnimation(GetDamageAnimation);
}

void USHGHUDUserWidget::OnNewPawn(APawn* NewPawn)
{
    if (!NewPawn)
        return;

    NewPawn->OnTakeAnyDamage.AddDynamic(this, &ThisClass::OnTakeAnyDamage);

    if (const auto StaminaComponent = NewPawn->FindComponentByClass<UShooterStaminaComponent>())
    {
        StaminaComponent->OnOutOfStamina.AddUObject(this, &ThisClass::OnOutOfStamina);
    }
}

void USHGHUDUserWidget::OnOutOfStamina()
{
    ShowAnimation(OutOfStaminaAnimation);
}

void USHGHUDUserWidget::OnPressedEsc()
{
    const auto GameMode = GetWorld()->GetAuthGameMode<ASHGGameModeArena>();
    if (!IsVisible() || IsAnimationPlaying(StartupAnimation) || !GameMode)
        return;

    GameMode->SetPause(GetOwningPlayer());
    ChangeGameState(EGameState::Pause);
}

void USHGHUDUserWidget::OnShowPopUpHint(const FHintData& HintData)
{
    const auto GameMode = GetWorld()->GetAuthGameMode<ASHGGameModeArena>();
    if (!IsVisible() || !GameMode)
        return;

    GameMode->SetPause(GetOwningPlayer());
    ChangeGameState(EGameState::PopUpHint);
}

void USHGHUDUserWidget::ShowAnimation(UWidgetAnimation* Animation)
{
    if (IsAnimationPlaying(Animation))
        return;

    PlayAnimation(Animation);
}

void USHGHUDUserWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
    Super::OnAnimationFinished_Implementation(Animation);

    if (Animation != FadeoutAnimation)
        return;

    if (const auto GameMode = GetWorld()->GetAuthGameMode<ASHGGameModeBase>())
    {
        GameMode->SetGameState(GameStateToSet);
    }
}
