// Shooter_Game, All rights reserved.

#include "Pickups/ShooterPickupBaseActor.h"
#include "Components/SphereComponent.h"

constexpr static float RotationTimerRate = 0.01f;

DEFINE_LOG_CATEGORY_STATIC(PickupLog, All, All);

AShooterPickupBaseActor::AShooterPickupBaseActor()
{
    PrimaryActorTick.bCanEverTick = true;

    SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
    SphereComponent->InitSphereRadius(50);
    SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    SetRootComponent(SphereComponent);
}

bool AShooterPickupBaseActor::CanBeTaken()
{
    return !GetWorldTimerManager().IsTimerActive(RespawnTimerHandle);
}

void AShooterPickupBaseActor::BeginPlay()
{
    Super::BeginPlay();

    check(SphereComponent);

    GetWorldTimerManager().SetTimer(RotationTimerHandle, this, &AShooterPickupBaseActor::RotatePickup, RotationTimerRate, true);
}

void AShooterPickupBaseActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    TryToGivePickupToOverlapedPawns();
}

void AShooterPickupBaseActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);

    const auto PlayerPawn = Cast<APawn>(OtherActor);
    if (!PlayerPawn)
        return;

    if (GivePickupTo(PlayerPawn))
    {
        if (SingleUsePickup)
        {
            GetWorldTimerManager().ClearTimer(RotationTimerHandle);
            Destroy();
        }
        else
        {
            PickupWasTaken();
        }
    }
    else
    {
        OverlapedPawns.Add(PlayerPawn);
    }
}

void AShooterPickupBaseActor::NotifyActorEndOverlap(AActor* OtherActor)
{
    Super::NotifyActorEndOverlap(OtherActor);

    const auto PlayerPawn = Cast<APawn>(OtherActor);
    if (!PlayerPawn)
        return;

    OverlapedPawns.Remove(PlayerPawn);
}

bool AShooterPickupBaseActor::GivePickupTo(APawn* PlayerPawn)
{
    return false;
}

void AShooterPickupBaseActor::TryToGivePickupToOverlapedPawns()
{
    for (const auto OverlapedPawn : OverlapedPawns)
    {
        if (GivePickupTo(OverlapedPawn))
        {
            PickupWasTaken();
            return;
        }
    }
}

void AShooterPickupBaseActor::PickupWasTaken()
{
    SphereComponent->SetVisibility(false, true);
    SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &AShooterPickupBaseActor::Respawn, RespawnTime);
}

void AShooterPickupBaseActor::Respawn()
{
    SphereComponent->SetVisibility(true, true);
    SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AShooterPickupBaseActor::RotatePickup()
{
    AddActorWorldRotation(FRotator(0.0f, RotationYaw, 0.0f));
}
