// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/ShooterBaseAnimatedUserWidget.h"
#include "ShooterGameUserWidget.generated.h"

class UTextBlock;
class UWidgetAnimation;

UCLASS()
class SHOOTER_GAME_API UShooterGameUserWidget : public UShooterBaseAnimatedUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(Meta = (BindWidgetAnim), Transient)
    UWidgetAnimation* OutOfStaminaAnimation = nullptr;

    UPROPERTY(Meta = (BindWidgetAnim), Transient)
    UWidgetAnimation* GetDamageAnimation = nullptr;

    virtual void NativeOnInitialized() override;

private:
    UFUNCTION()
    void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

    void OnNewPawn(APawn* NewPawn);
    void OnOutOfStamina();

    void ShowAnimation(UWidgetAnimation* Animation);
};
