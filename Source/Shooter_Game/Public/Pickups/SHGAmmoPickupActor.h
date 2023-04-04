// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Pickups/SHGBasePickupActor.h"
#include "SHGAmmoPickupActor.generated.h"

class ASHGBaseWeaponActor;

UCLASS()
class SHOOTER_GAME_API ASHGAmmoPickupActor : public ASHGBasePickupActor
{
    GENERATED_BODY()

public:
    TSubclassOf<ASHGBaseWeaponActor> GetWeaponClass() const { return AmmoWeaponClass; }

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup");
    TSubclassOf<ASHGBaseWeaponActor> AmmoWeaponClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup", Meta = (ClampMin = "1"));
    int32 ClipsAmount = 1;

private:
    virtual bool TryToGivePickupTo(APawn* Pawn) override;
};
