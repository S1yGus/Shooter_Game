// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Pickups/SHGBasePickupActor.h"
#include "SHGHealthPickupActor.generated.h"

UCLASS()
class SHOOTER_GAME_API ASHGHealthPickupActor : public ASHGBasePickupActor
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
    float HealAmount = 50.0f;

private:
    virtual bool TryToGivePickupTo(AActor* Actor) override;
};
