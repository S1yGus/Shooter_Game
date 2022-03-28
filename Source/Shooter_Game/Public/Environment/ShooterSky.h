// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShooterSky.generated.h"

class UStaticMeshComponent;

UCLASS()
class SHOOTER_GAME_API AShooterSky : public AActor
{
    GENERATED_BODY()

public:
    AShooterSky();

protected:
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    UStaticMeshComponent* StaticMeshComponent;
};
