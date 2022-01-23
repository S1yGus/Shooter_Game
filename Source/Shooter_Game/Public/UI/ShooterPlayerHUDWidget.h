// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShooterCoreTypes.h"
#include "ShooterPlayerHUDWidget.generated.h"

class UShooterWeaponComponent;
class UShooterHealthComponent;

UCLASS()
class SHOOTER_GAME_API UShooterPlayerHUDWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual bool Initialize();

    UFUNCTION(BlueprintCallable)
    float GetHelthPercent() const;

    UFUNCTION(BlueprintCallable)
    bool GetCurrentWeaponUIData(FWeaponUIData& Data) const;

    UFUNCTION(BlueprintCallable)
    bool GetCurrentWeaponAmmoData(FAmmoData& Data) const;

    UFUNCTION(BlueprintCallable)
    bool IsPlayerAlive() const;

    UFUNCTION(BlueprintCallable)
    bool IsPlayerSpectating() const;

    UFUNCTION(BlueprintImplementableEvent)
    void OnTakeDamage();

protected:
    UFUNCTION()
    void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);
};
