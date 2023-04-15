// Shooter_Game, All rights reserved.

#include "Tests/SHGBasePickupActorTestable.h"

void ASHGBasePickupActorTestable::SetPickupData(bool bInReturnValue, bool bInRespawnable, float InRespawnTime)
{
    bReturnValue = bInReturnValue;
    bRespawnable = bInRespawnable;
    RespawnTime = InRespawnTime;
}

bool ASHGBasePickupActorTestable::TryToGivePickupTo(APawn* Pawn)
{
    return bReturnValue;
}
