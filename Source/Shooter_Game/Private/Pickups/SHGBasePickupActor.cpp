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
    GetWorldTimerManager().SetTimer(RotationTimerHandle, this, &ThisClass::OnRotatePickup, RotationTimerRate, true);
}

void ASHGBasePickupActor::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    TryToGivePickupToOverlapActors();
}

void ASHGBasePickupActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);

    OverlapActors.Add(OtherActor);
}

void ASHGBasePickupActor::NotifyActorEndOverlap(AActor* OtherActor)
{
    Super::NotifyActorEndOverlap(OtherActor);

    OverlapActors.Remove(OtherActor);
}

bool ASHGBasePickupActor::TryToGivePickupTo(AActor* Actor)
{
    return false;
}

void ASHGBasePickupActor::TryToGivePickupToOverlapActors()
{
    for (const auto OverlapActor : OverlapActors)
    {
        if (!IsValid(OverlapActor))
        {
            continue;
        }

        if (TryToGivePickupTo(OverlapActor))
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
