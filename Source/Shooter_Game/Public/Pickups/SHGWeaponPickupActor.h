// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Pickups/SHGBasePickupActor.h"
#include "SHGWeaponPickupActor.generated.h"

class ASHGBaseWeaponActor;

UCLASS(Abstract)
class SHOOTER_GAME_API ASHGWeaponPickupActor : public ASHGBasePickupActor
{
    GENERATED_BODY()

public:
    TSubclassOf<ASHGBaseWeaponActor> GetWeaponClass() const { return WeaponClass; }

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
    TSubclassOf<ASHGBaseWeaponActor> WeaponClass;

    virtual void BeginPlay() override;

private:
    virtual bool TryToGivePickupTo(APawn* Pawn);
};
