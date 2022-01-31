// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShooterCoreTypes.h"
#include "ShooterGameUserWidget.generated.h"

class UShooterWeaponComponent;
class UShooterHealthComponent;

UCLASS()
class SHOOTER_GAME_API UShooterGameUserWidget : public UUserWidget
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
    bool GetCurrentRaundInfo(float& Raund, float& RaundsAmount, float& RaundTime) const;

    UFUNCTION(BlueprintCallable)
    bool GetPlayerStateInfo(float& Kills, float& Deaths) const;

    UFUNCTION(BlueprintImplementableEvent)
    void OnTakeDamage();

private:
    UFUNCTION()
    void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

    void OnNewPawn(APawn* NewPawn);
};
