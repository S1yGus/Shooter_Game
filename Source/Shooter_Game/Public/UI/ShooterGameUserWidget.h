// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShooterCoreTypes.h"
#include "ShooterGameUserWidget.generated.h"

class UShooterWeaponComponent;
class UShooterHealthComponent;
class UProgressBar;

UCLASS()
class SHOOTER_GAME_API UShooterGameUserWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    float GetHelthPercent() const;

    UFUNCTION(BlueprintCallable)
    bool GetCurrentWeaponUIData(FWeaponUIData& Data) const;

    UFUNCTION(BlueprintCallable)
    bool GetCurrentWeaponAmmoData(FAmmoData& Data) const;

    UFUNCTION(BlueprintCallable)
    bool GetCurrentRaundInfo(float& Raund, float& RaundsAmount, float& RaundTime) const;

    UFUNCTION(BlueprintCallable)
    bool GetPlayerStateInfo(float& Kills, float& Deaths) const;

    UFUNCTION(BlueprintImplementableEvent)
    void OnTakeDamage();

protected:
    UPROPERTY(Meta = (BindWidgetAnim), Transient)
    UWidgetAnimation* GetDamageAnimation;

    UPROPERTY(Meta = (BindWidget))
    UProgressBar* HealthProgressBar;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    float HealthColorThreshold = 0.4f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FLinearColor NormalHealthColor = FLinearColor::White;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FLinearColor LowHealthColor = FLinearColor::Red;

    virtual void NativeOnInitialized() override;

private:
    UFUNCTION()
    void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

    UFUNCTION()
    void OnHealthChanged(float Health);

    void OnNewPawn(APawn* NewPawn);
    void ShowGetDamageAnimation();
    void UpdateHealthProgressBar();
};
