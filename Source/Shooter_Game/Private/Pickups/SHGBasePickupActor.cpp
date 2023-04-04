// Shooter_Game, All rights reserved.

#include "Pickups/SHGBasePickupActor.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

constexpr static float RotationTimerRate = 0.015f;

DEFINE_LOG_CATEGORY_STATIC(PickupLog, All, All);

ASHGBasePickupActor::ASHGBasePickupActor()
{
    PrimaryActorTick.bCanEverTick = true;

    SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
    SphereComponent->InitSphereRadius(50);
    SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    SetRootComponent(SphereComponent);
}

bool ASHGBasePickupActor::CanBeTaken() const
{
    return !GetWorldTimerManager().IsTimerActive(RespawnTimerHandle);
}

void ASHGBasePickupActor::BeginPlay()
{
    Super::BeginPlay();

    check(SphereComponent);

    FTimerHandle RotationTimerHandle;
    GetWorldTimerManager().SetTimer(RotationTimerHandle, this, &ThisClass::OnRotatePickup, RotationTimerRate, true);    // Start of rotation.
}

void ASHGBasePickupActor::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    TryToGivePickupToOverlapPawns();
}

void ASHGBasePickupActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);

    if (const auto Pawn = Cast<APawn>(OtherActor))
    {
        if (TryToGivePickupTo(Pawn))
        {
            if (bRespawnable)
            {
                PickupWasTaken();
            }
            else
            {
                Destroy();
            }
        }
        else
        {
            OverlapPawns.Add(Pawn);
        }
    }
}

void ASHGBasePickupActor::NotifyActorEndOverlap(AActor* OtherActor)
{
    Super::NotifyActorEndOverlap(OtherActor);

    if (const auto Pawn = Cast<APawn>(OtherActor))
    {
        OverlapPawns.Remove(Pawn);
    }
}

bool ASHGBasePickupActor::TryToGivePickupTo(APawn* Pawn)
{
    return false;
}

void ASHGBasePickupActor::TryToGivePickupToOverlapPawns()
{
    for (const auto OverlapPawn : OverlapPawns)
    {
        if (IsValid(OverlapPawn) && TryToGivePickupTo(OverlapPawn))
        {
            if (bRespawnable)
            {
                PickupWasTaken();
            }
            else
            {
                Destroy();
            }

            break;
        }
    }
}

void ASHGBasePickupActor::PickupWasTaken()
{
    SphereComponent->SetVisibility(false, true);
    SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    UGameplayStatics::PlaySoundAtLocation(this, TakenPickupSound, GetActorLocation());

    GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &ThisClass::OnRespawn, RespawnTime);
}

void ASHGBasePickupActor::OnRespawn()
{
    SphereComponent->SetVisibility(true, true);
    SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ASHGBasePickupActor::OnRotatePickup()
{
    AddActorWorldRotation(FRotator{0.0f, RotationYaw, 0.0f});
}
