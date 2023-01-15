// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Pickups/ShooterPickupBaseActor.h"
#include "SHGWeaponCoreTypes.h"
#include "SHGPickupWeaponActor.generated.h"

class ASHGBaseWeaponActor;

UCLASS()
class SHOOTER_GAME_API ASHGPickupWeaponActor : public AShooterPickupBaseActor
{
    GENERATED_BODY()

public:
    TSubclassOf<ASHGBaseWeaponActor> GetWeaponClass() const { return WeaponClass; }

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
    TSubclassOf<ASHGBaseWeaponActor> WeaponClass;

    virtual void BeginPlay() override;

private:
    virtual bool GivePickupTo(APawn* PlayerPawn);
};
