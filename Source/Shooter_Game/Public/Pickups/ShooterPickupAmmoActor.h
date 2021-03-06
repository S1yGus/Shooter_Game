// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Pickups/ShooterPickupBaseActor.h"
#include "ShooterPickupAmmoActor.generated.h"

class AShooterBaseWeaponActor;

UCLASS()
class SHOOTER_GAME_API AShooterPickupAmmoActor : public AShooterPickupBaseActor
{
    GENERATED_BODY()

public:
    TSubclassOf<AShooterBaseWeaponActor> GetWeaponClass() const { return AmmoWeaponClass; }

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings");
    TSubclassOf<AShooterBaseWeaponActor> AmmoWeaponClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (ClampMin = "1"));
    int32 ClipsAmount = 1;

private:
    virtual bool GivePickupTo(APawn* PlayerPawn) override;
};
