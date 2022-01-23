// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ShooterWeaponComponent.h"
#include "ShooterAIWeaponComponent.generated.h"

UCLASS()
class SHOOTER_GAME_API UShooterAIWeaponComponent : public UShooterWeaponComponent
{
    GENERATED_BODY()

public:
    virtual void StartFire() override;
    virtual void NextWeapon() override;
    bool CheckWeapon(EWeaponType WeaponType) const;
    bool CheckWeaponAmmo(EWeaponType WeaponType) const;
    bool CheckCurrentWeapon(EWeaponType WeaponType) const;

    float GetCurrentMinAttackDistance() const;
    float GetCurrentMaxAttackDistance() const;
};
