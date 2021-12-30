// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Pickups/ShooterPickupBaseActor.h"
#include "ShooterPickupHealthActor.generated.h"

UCLASS()
class SHOOTER_GAME_API AShooterPickupHealthActor : public AShooterPickupBaseActor
{
	GENERATED_BODY()

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
    float HealAmount = 50.0f;
	
private:
	virtual bool GivePickupTo(APawn* PlayerPawn) override;
};
