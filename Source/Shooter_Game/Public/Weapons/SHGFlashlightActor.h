// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SHGFlashlightActor.generated.h"

class UStaticMeshComponent;
class USpotLightComponent;
class UMaterialInstanceDynamic;

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

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Material")
    FName EmissivePowerParameterName = "EmissivePower";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Material")
    float EmissivePower = 150.0f;

    virtual void BeginPlay() override;

private:
    UPROPERTY()
    UMaterialInstanceDynamic* DynamicMaterial;
};
