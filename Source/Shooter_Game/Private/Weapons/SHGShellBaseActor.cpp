// Shooter_Game, All rights reserved.

#include "Weapons/SHGShellBaseActor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"

ASHGShellBaseActor::ASHGShellBaseActor()
{
    PrimaryActorTick.bCanEverTick = false;

    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
    StaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    StaticMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    StaticMesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECollisionResponse::ECR_Block);
    SetRootComponent(StaticMesh);

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovment");
    ProjectileMovement->InitialSpeed = 300.0f;
    ProjectileMovement->ProjectileGravityScale = 1.0f;
    ProjectileMovement->bShouldBounce = true;
    ProjectileMovement->Bounciness = 0.3f;
}

void ASHGShellBaseActor::BeginPlay()
{
    Super::BeginPlay();

    check(ProjectileMovement);
    check(StaticMesh);

    ProjectileMovement->Velocity = DropDirection * ProjectileMovement->InitialSpeed;

    SetLifeSpan(LifeSpan);
}
