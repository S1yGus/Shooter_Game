// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShooterCoreTypes.h"
#include "ShooterGameUserWidget.generated.h"

class UProgressBar;
class UWidgetAnimation;

UCLASS()
class SHOOTER_GAME_API UShooterGameUserWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    float GetHelthPercent() const;

    UFUNCTION(BlueprintCallable)
    float GetStaminaPercent() const;

    UFUNCTION(BlueprintCallable)
    bool GetCurrentWeaponUIData(FWeaponUIData& Data) const;

    UFUNCTION(BlueprintCallable)
    bool GetCurrentWeaponAmmoData(FAmmoData& Data) const;

    UFUNCTION(BlueprintCallable)
    bool GetCurrentRaundInfo(float& Raund, float& RaundsAmount, float& RaundTime) const;

    UFUNCTION(BlueprintCallable)
    bool GetPlayerStateInfo(float& Kills, float& Deaths) const;

protected:
    UPROPERTY(Meta = (BindWidgetAnim), Transient)
    UWidgetAnimation* GetDamageAnimation;

    UPROPERTY(Meta = (BindWidgetAnim), Transient)
    UWidgetAnimation* OutOfStaminaAnimation;

    UPROPERTY(Meta = (BindWidgetAnim), Transient)
    UWidgetAnimation* NotEnoughStaminaAnimation;

    UPROPERTY(Meta = (BindWidget))
    UProgressBar* HealthProgressBar;

    UPROPERTY(Meta = (BindWidget))
    UProgressBar* StaminaProgressBar;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI|Health")
    float HealthColorThreshold = 0.4f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI|Health")
    FLinearColor NormalHealthColor = FLinearColor::White;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI|Health")
    FLinearColor LowHealthColor = FLinearColor::Red;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI|Stamina")
    float StaminaColorThreshold = 0.4f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI|Stamina")
    FLinearColor NormalStaminaColor = FLinearColor::White;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI|Stamina")
    FLinearColor LowStaminaColor = FLinearColor::Red;

    virtual void NativeOnInitialized() override;

private:
    UFUNCTION()
    void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

    UFUNCTION()
    void OnHealthChanged(float Health);

    UFUNCTION()
    void OnStaminaChanged(float Stamina);

    void OnOutOfStamina();
    void OnNotEnoughStamina();

    void OnNewPawn(APawn* NewPawn);
    void ShowAnimation(UWidgetAnimation* Animation);
    void UpdateHealthProgressBar();
    void UpdateStaminaProgressBar();
};
