// Shooter_Game, All rights reserved.

#include "UI/HUD/ShooterGameUserWidget.h"
#include "Components/ShooterHealthComponent.h"
#include "Components/ShooterStaminaComponent.h"

void UShooterGameUserWidget::NativeOnInitialized()
{
    const auto Controller = GetOwningPlayer();
    if (!Controller)
        return;

    Controller->GetOnNewPawnNotifier().AddUObject(this, &UShooterGameUserWidget::OnNewPawn);
}

void UShooterGameUserWidget::OnTakeAnyDamage(AActor* DamagedActor,             //
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

void UShooterGameUserWidget::OnNewPawn(APawn* NewPawn)
{
    if (!NewPawn)
        return;

    NewPawn->OnTakeAnyDamage.AddDynamic(this, &UShooterGameUserWidget::OnTakeAnyDamage);

    if (const auto StaminaComponent = NewPawn->FindComponentByClass<UShooterStaminaComponent>())
    {
        StaminaComponent->OnOutOfStamina.AddUObject(this, &UShooterGameUserWidget::OnOutOfStamina);
    }
}

void UShooterGameUserWidget::OnOutOfStamina()
{
    ShowAnimation(OutOfStaminaAnimation);
}

void UShooterGameUserWidget::ShowAnimation(UWidgetAnimation* Animation)
{
    if (IsAnimationPlaying(Animation))
        return;

    PlayAnimation(Animation);
}
