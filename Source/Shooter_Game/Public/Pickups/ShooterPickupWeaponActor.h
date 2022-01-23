// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Pickups/ShooterPickupBaseActor.h"
#include "ShooterCoreTypes.h"
#include "ShooterPickupWeaponActor.generated.h"

class AShooterBaseWeaponActor;

UCLASS()
class SHOOTER_GAME_API AShooterPickupWeaponActor : public AShooterPickupBaseActor
{
    GENERATED_BODY()

public:
    TSubclassOf<AShooterBaseWeaponActor> GetWeaponClass() const { return WeaponData.WeaponClass; }

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
    FWeaponData WeaponData;

private:
    virtual bool GivePickupTo(APawn* PlayerPawn);
};
