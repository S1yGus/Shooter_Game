// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShooterMoon.generated.h"

class UDirectionalLightComponent;
class UBillboardComponent;

UCLASS()
class SHOOTER_GAME_API AShooterMoon : public AActor
{
    GENERATED_BODY()

public:
    AShooterMoon();

    UStaticMeshComponent* GetStaticMesh() const { return StaticMeshComponent; }
    UDirectionalLightComponent* GetDirectionalLight() const { return DirectionalLightComponent; }

protected:
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    UBillboardComponent* BillboardComponent;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    UStaticMeshComponent* StaticMeshComponent;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    UDirectionalLightComponent* DirectionalLightComponent;
};
