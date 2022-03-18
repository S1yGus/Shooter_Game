// Shooter_Game, All rights reserved.

#include "UI/ShooterGameUserWidget.h"
#include "Components/ShooterHealthComponent.h"
#include "Components/ShooterWeaponComponent.h"
#include "Components/ShooterStaminaComponent.h"
#include "Components/ProgressBar.h"
#include "ShooterGameModeBase.h"
#include "Player/ShooterPlayerState.h"

void UShooterGameUserWidget::NativeOnInitialized()
{
    const auto Controller = GetOwningPlayer();
    if (!Controller)
        return;

    Controller->GetOnNewPawnNotifier().AddUObject(this, &UShooterGameUserWidget::OnNewPawn);
}

float UShooterGameUserWidget::GetHelthPercent() const
{
    if (!GetOwningPlayerPawn())
        return 0.0f;

    const auto HealthComponent = GetOwningPlayerPawn()->FindComponentByClass<UShooterHealthComponent>();
    if (!HealthComponent)
        return 0.0f;

    return HealthComponent->GetHealthPercent();
}

float UShooterGameUserWidget::GetStaminaPercent() const
{
    if (!GetOwningPlayerPawn())
        return 0.0f;

    const auto StaminaComponent = GetOwningPlayerPawn()->FindComponentByClass<UShooterStaminaComponent>();
    if (!StaminaComponent)
        return 0.0f;

    return StaminaComponent->GetStaminaPercent();
}

bool UShooterGameUserWidget::GetCurrentWeaponUIData(FWeaponUIData& Data) const
{
    if (!GetOwningPlayerPawn())
        return false;

    const auto WeaponComponent = GetOwningPlayerPawn()->FindComponentByClass<UShooterWeaponComponent>();
    if (!WeaponComponent)
        return false;

    return WeaponComponent->GetCurrentWeaponUIData(Data);
}

bool UShooterGameUserWidget::GetCurrentWeaponAmmoData(FAmmoData& Data) const
{
    if (!GetOwningPlayerPawn())
        return false;

    const auto WeaponComponent = GetOwningPlayerPawn()->FindComponentByClass<UShooterWeaponComponent>();
    if (!WeaponComponent)
        return false;

    return WeaponComponent->GetCurrentWeaponAmmoData(Data);
}

bool UShooterGameUserWidget::GetCurrentRaundInfo(float& Raund, float& RaundsAmount, float& RaundTime) const
{
    const auto GameMode = Cast<AShooterGameModeBase>(GetWorld()->GetAuthGameMode());
    if (!GameMode)
        return false;

    Raund = GameMode->GetCurrentRaund();
    RaundsAmount = GameMode->GetRaundNum();
    RaundTime = GameMode->GetCurrentRaundTime();
    return true;
}

bool UShooterGameUserWidget::GetPlayerStateInfo(float& Kills, float& Deaths) const
{
    const auto PlayerState = Cast<AShooterPlayerState>(GetOwningPlayer()->PlayerState);
    if (!PlayerState)
        return false;

    Kills = PlayerState->GetKillsNum();
    Deaths = PlayerState->GetDeathsNum();
    return true;
}

void UShooterGameUserWidget::OnTakeAnyDamage(AActor* DamagedActor,             //
                                             float Damage,                     //
                                             const UDamageType* DamageType,    //
                                             AController* InstigatedBy,        //
                                             AActor* DamageCauser)
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

void UShooterGameUserWidget::OnHealthChanged(float Health)
{
    UpdateHealthProgressBar();
}

void UShooterGameUserWidget::OnStaminaChanged(float Stamina)
{
    UpdateStaminaProgressBar();
}

void UShooterGameUserWidget::OnOutOfStamina()
{
    ShowAnimation(OutOfStaminaAnimation);
    ShowAnimation(NotEnoughStaminaAnimation);
}

void UShooterGameUserWidget::OnNotEnoughStamina()
{
    ShowAnimation(NotEnoughStaminaAnimation);
}

void UShooterGameUserWidget::OnNewPawn(APawn* NewPawn)
{
    if (!NewPawn)
        return;

    NewPawn->OnTakeAnyDamage.AddDynamic(this, &UShooterGameUserWidget::OnTakeAnyDamage);

    const auto HealthComponent = NewPawn->FindComponentByClass<UShooterHealthComponent>();
    if (HealthComponent)
    {
        HealthComponent->OnHealthChanged.AddDynamic(this, &UShooterGameUserWidget::OnHealthChanged);
    }

    const auto StaminaComponent = NewPawn->FindComponentByClass<UShooterStaminaComponent>();
    if (StaminaComponent)
    {
        StaminaComponent->OnStaminaChanged.AddDynamic(this, &UShooterGameUserWidget::OnStaminaChanged);
        StaminaComponent->OnOutOfStamina.AddUObject(this, &UShooterGameUserWidget::OnOutOfStamina);
        StaminaComponent->OnNotEnoughStamina.AddUObject(this, &UShooterGameUserWidget::OnNotEnoughStamina);
    }

    UpdateHealthProgressBar();
    UpdateStaminaProgressBar();
}

void UShooterGameUserWidget::ShowAnimation(UWidgetAnimation* Animation)
{
    if (IsAnimationPlaying(Animation))
        return;

    PlayAnimation(Animation);
}

void UShooterGameUserWidget::UpdateHealthProgressBar()
{
    if (!HealthProgressBar)
        return;

    HealthProgressBar->SetFillColorAndOpacity(GetHelthPercent() > HealthColorThreshold ? NormalHealthColor : LowHealthColor);
}

void UShooterGameUserWidget::UpdateStaminaProgressBar()
{
    if (!StaminaProgressBar)
        return;

    StaminaProgressBar->SetFillColorAndOpacity(GetStaminaPercent() > StaminaColorThreshold ? NormalStaminaColor : LowStaminaColor);
}
