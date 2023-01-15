// Shooter_Game, All rights reserved.

#include "Weapons/SHGFlashlightActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SpotLightComponent.h"

ASHGFlashlightActor::ASHGFlashlightActor()
{
    PrimaryActorTick.bCanEverTick = false;

    FlashlightMesh = CreateDefaultSubobject<UStaticMeshComponent>("FlashlightMesh");
    SetRootComponent(FlashlightMesh);

    SpotLight = CreateDefaultSubobject<USpotLightComponent>("SpotLight");
    SpotLight->SetHiddenInGame(true);
    SpotLight->AttenuationRadius = 11000.0f;
    SpotLight->OuterConeAngle = 26.0f;
    SpotLight->SourceRadius = 1.0f;
    SpotLight->bUseTemperature = true;
    SpotLight->Temperature = 7500.0f;
    SpotLight->VolumetricScatteringIntensity = 2.5f;
    SpotLight->SetupAttachment(FlashlightMesh);
}

void ASHGFlashlightActor::SetFlashlight(bool bCondition)
{
    SpotLight->SetHiddenInGame(!bCondition);
}

void ASHGFlashlightActor::BeginPlay()
{
    Super::BeginPlay();

    check(FlashlightMesh);
    check(SpotLight);
}
