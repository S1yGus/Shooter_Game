// Shooter_Game, All rights reserved.

#include "UI/ShooterGameUserWidget.h"
#include "Components/ShooterHealthComponent.h"
#include "Components/ShooterWeaponComponent.h"
#include "ShooterGameModeBase.h"
#include "Player/ShooterPlayerState.h"
#include "Components/ProgressBar.h"

void UShooterGameUserWidget::NativeOnInitialized()
{
    const auto Controller = GetOwningPlayer();
    if (!Controller)
        return;

    Controller->GetOnNewPawnNotifier().AddUObject(this, &UShooterGameUserWidget::OnNewPawn);
}

float UShooterGameUserWidget::GetHelthPercent() const
{
    const auto HealthComponent = GetOwningPlayerPawn()->FindComponentByClass<UShooterHealthComponent>();
    if (!HealthComponent)
        return 0.0f;

    return HealthComponent->GetHealthPercent();
}

bool UShooterGameUserWidget::GetCurrentWeaponUIData(FWeaponUIData& Data) const
{
    const auto WeaponComponent = GetOwningPlayerPawn()->FindComponentByClass<UShooterWeaponComponent>();
    if (!WeaponComponent)
        return false;

    return WeaponComponent->GetCurrentWeaponUIData(Data);
}

bool UShooterGameUserWidget::GetCurrentWeaponAmmoData(FAmmoData& Data) const
{
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
        return;

    if (GetDamageAnimation)
    {
        ShowGetDamageAnimation();
    }

    OnTakeDamage();
    UpdateHealthProgressBar();
}

void UShooterGameUserWidget::OnHealthChanged(float Health)
{
    UpdateHealthProgressBar();
}

void UShooterGameUserWidget::OnNewPawn(APawn* NewPawn)
{
    if (!NewPawn)
        return;

    NewPawn->OnTakeAnyDamage.AddDynamic(this, &UShooterGameUserWidget::OnTakeAnyDamage);

    const auto HealthComponent = NewPawn->FindComponentByClass<UShooterHealthComponent>();
    HealthComponent->OnHealthChanged.AddDynamic(this, &UShooterGameUserWidget::OnHealthChanged);

    UpdateHealthProgressBar();
}

void UShooterGameUserWidget::ShowGetDamageAnimation()
{
    if (!IsAnimationPlaying(GetDamageAnimation))
    {
        PlayAnimation(GetDamageAnimation);
    }
}

void UShooterGameUserWidget::UpdateHealthProgressBar()
{
    if (!HealthProgressBar)
        return;

    HealthProgressBar->SetFillColorAndOpacity(GetHelthPercent() > HealthColorThreshold ? NormalHealthColor : LowHealthColor);
}
