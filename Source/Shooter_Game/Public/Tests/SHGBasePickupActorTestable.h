// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Pickups/SHGBasePickupActor.h"
#include "SHGBasePickupActorTestable.generated.h"

UCLASS()
class SHOOTER_GAME_API ASHGBasePickupActorTestable : public ASHGBasePickupActor
{
    GENERATED_BODY()

public:
    void SetPickupData(bool bInReturnValue, bool bInRespawnable = false, float InRespawnTime = 0.0f);

private:
    bool bReturnValue = true;

    virtual bool TryToGivePickupTo(APawn* Pawn) override;
};
