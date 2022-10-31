// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/SHGAnimatedUserWidget.h"
#include "SHGUICoreTypes.h"
#include "SHGHUDUserWidget.generated.h"

UCLASS()
class SHOOTER_GAME_API USHGHUDUserWidget : public USHGAnimatedUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(Meta = (BindWidgetAnim), Transient)
    UWidgetAnimation* OutOfStaminaAnimation;

    UPROPERTY(Meta = (BindWidgetAnim), Transient)
    UWidgetAnimation* GetDamageAnimation;

    virtual void NativeOnInitialized() override;

private:
    UFUNCTION()
    void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);
    void OnNewPawn(APawn* NewPawn);
    void OnOutOfStamina();
    void OnPressedEsc();
    void OnShowPopUpHint(const FHintData& HintData);

    void ShowAnimation(UWidgetAnimation* Animation);

    virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;
};
