// Shooter_Game, All rights reserved.

#include "Environment/ShooterMoon.h"
#include "Components/BillboardComponent.h"
#include "Components/DirectionalLightComponent.h"

AShooterMoon::AShooterMoon()
{
    PrimaryActorTick.bCanEverTick = false;

    BillboardComponent = CreateDefaultSubobject<UBillboardComponent>("BillboardComponent");
    SetRootComponent(BillboardComponent);

    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
    StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    StaticMeshComponent->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
    StaticMeshComponent->CastShadow = 0;
    StaticMeshComponent->SetRelativeRotation(FRotator(180.0f, 0.0f, 0.0f));
    StaticMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 10000000.0f));
    StaticMeshComponent->SetRelativeScale3D(FVector(10000.0f, 10000.0f, 10000.0f));
    StaticMeshComponent->SetupAttachment(GetRootComponent());

    DirectionalLightComponent = CreateDefaultSubobject<UDirectionalLightComponent>("DirectionalLight");
    DirectionalLightComponent->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
    DirectionalLightComponent->SetIntensity(0.8f);
    DirectionalLightComponent->bUseTemperature = true;
    DirectionalLightComponent->SetTemperature(7500.0f);
    DirectionalLightComponent->SetVolumetricScatteringIntensity(0.5f);
    DirectionalLightComponent->bEnableLightShaftOcclusion = true;
    DirectionalLightComponent->bEnableLightShaftBloom = true;
    DirectionalLightComponent->SetDynamicShadowCascades(4);
    DirectionalLightComponent->bUsedAsAtmosphereSunLight = true;
    DirectionalLightComponent->AtmosphereSunLightIndex = 1;
    DirectionalLightComponent->bCastCloudShadows = true;
    DirectionalLightComponent->SetupAttachment(GetRootComponent());
}
