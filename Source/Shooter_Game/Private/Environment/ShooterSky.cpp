// Shooter_Game, All rights reserved.

#include "Environment/ShooterSky.h"
#include "Components/StaticMeshComponent.h"

AShooterSky::AShooterSky()
{
    PrimaryActorTick.bCanEverTick = false;

    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
    StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    StaticMeshComponent->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
    StaticMeshComponent->CastShadow = 0;
    SetRootComponent(StaticMeshComponent);
}
