// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SHGFlashlightActor.generated.h"

class UStaticMeshComponent;
class USpotLightComponent;

UCLASS()
class SHOOTER_GAME_API ASHGFlashlightActor : public AActor
{
    GENERATED_BODY()

public:
    ASHGFlashlightActor();

    void SetFlashlight(bool bCondition);

protected:
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    UStaticMeshComponent* FlashlightMesh;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    USpotLightComponent* SpotLight;

    virtual void BeginPlay() override;
};
