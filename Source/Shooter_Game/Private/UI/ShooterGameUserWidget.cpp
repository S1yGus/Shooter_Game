// Shooter_Game, All rights reserved.

#include "UI/ShooterGameUserWidget.h"
#include "Components/ShooterHealthComponent.h"
#include "Components/ShooterWeaponComponent.h"
#include "ShooterGameModeBase.h"
#include "Player/ShooterPlayerState.h"

bool UShooterGameUserWidget::Initialize()
{
    const auto Controller = GetOwningPlayer();
    if (!Controller)
        return Super::Initialize();

    Controller->GetOnNewPawnNotifier().AddUObject(this, &UShooterGameUserWidget::OnNewPawn);
    return Super::Initialize();
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

    OnTakeDamage();
}

void UShooterGameUserWidget::OnNewPawn(APawn* NewPawn)
{
    if (!NewPawn)
        return;

    NewPawn->OnTakeAnyDamage.AddDynamic(this, &UShooterGameUserWidget::OnTakeAnyDamage);
}
