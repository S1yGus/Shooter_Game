// Shooter_Game, All rights reserved.

#include "UI/HUD/ShooterDayNightUserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Environment/ShooterDayNightSystem.h"
#include "ShooterUtils.h"

FText UShooterDayNightUserWidget::GetCurrentTime() const
{
    const auto DayNightSystem = Cast<AShooterDayNightSystem>(UGameplayStatics::GetActorOfClass(GetWorld(), AShooterDayNightSystem::StaticClass()));
    if (!DayNightSystem)
        return FText();

    const auto Time = DayNightSystem->GetCurrentTimespan();
    const auto TimeStr = FString::Printf(TEXT("Day: %s Time: %s:%s:%s"), *ShooterUtils::FormatStringFromInt(Time.GetDays(), 2, '0'),    //
                                         *ShooterUtils::FormatStringFromInt(Time.GetHours(), 2, '0'),                                   //
                                         *ShooterUtils::FormatStringFromInt(Time.GetMinutes(), 2, '0'),                                 //
                                         *ShooterUtils::FormatStringFromInt(Time.GetSeconds(), 2, '0'));                                //
    return FText::FromString(TimeStr);
}

void UShooterDayNightUserWidget::NativeOnInitialized()
{
    const auto DayNightSystem = Cast<AShooterDayNightSystem>(UGameplayStatics::GetActorOfClass(GetWorld(), AShooterDayNightSystem::StaticClass()));
    this->SetVisibility(DayNightSystem ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}
