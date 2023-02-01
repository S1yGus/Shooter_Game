// Shooter_Game, All rights reserved.

#include "Weapons/SHGBaseProjectileActor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Sound/SoundCue.h"
#include "Components/DecalComponent.h"
#include "Components/SHGHealthComponent.h"

#define ECC_Projectile ECollisionChannel::ECC_GameTraceChannel3

ASHGBaseProjectileActor::ASHGBaseProjectileActor()
{
    PrimaryActorTick.bCanEverTick = false;

    HitSphereComponent = CreateDefaultSubobject<USphereComponent>("HitSphereComponent");
    HitSphereComponent->SetSphereRadius(2.0f);
    HitSphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    HitSphereComponent->SetCollisionObjectType(ECC_Projectile);
    HitSphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
    HitSphereComponent->SetCollisionResponseToChannel(ECC_Projectile, ECollisionResponse::ECR_Ignore);
    HitSphereComponent->bReturnMaterialOnMove = true;
    HitSphereComponent->SetWalkableSlopeOverride(FWalkableSlopeOverride{WalkableSlope_Unwalkable, 0.0f});
    HitSphereComponent->CanCharacterStepUpOn = ECB_No;
    SetRootComponent(HitSphereComponent);

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovment");
    ProjectileMovement->InitialSpeed = 20000.0f;
    ProjectileMovement->ProjectileGravityScale = 0.3f;
}

void ASHGBaseProjectileActor::BeginPlay()
{
    Super::BeginPlay();

    check(HitSphereComponent);
    check(ProjectileMovement);

    // SphereComponent->IgnoreActorWhenMoving(GetOwner(), true);
    HitSphereComponent->OnComponentHit.AddDynamic(this, &ThisClass::OnProjectileHit);

    ProjectileMovement->OnProjectileStop.AddDynamic(this, &ThisClass::OnProjectileStop);
    ProjectileMovement->OnProjectileBounce.AddDynamic(this, &ThisClass::OnProjectileBounce);
    ProjectileMovement->Velocity = ShotDirection * ProjectileMovement->InitialSpeed;

    SetLifeSpan(LifeSpanTime);
}

float ASHGBaseProjectileActor::GetDamage() const
{
    return FMath::GetMappedRangeValueClamped(FVector2D{ProjectileMovement->BounceVelocityStopSimulatingThreshold, ProjectileMovement->InitialSpeed},    //
                                             FVector2D{MinRicochetDamageMultiplier, 1.0f},                                                              //
                                             GetVelocity().Length()) * FMath::RandRange(Damage.X, Damage.Y);
}

void ASHGBaseProjectileActor::OnProjectileHit(UPrimitiveComponent* HitComponent,    //
                                              AActor* OtherActor,                   //
                                              UPrimitiveComponent* OtherComp,       //
                                              FVector NormalImpulse,                //
                                              const FHitResult& Hit)
{
    if (!OtherActor || OtherActor == this || !OtherComp)
        return;

    bool bAppliedDamage = false;
    if (OtherActor->FindComponentByClass<USHGHealthComponent>())
    {
        FPointDamageEvent DamageEvent;
        DamageEvent.HitInfo = Hit;
        OtherActor->TakeDamage(GetDamage(), DamageEvent, GetOwnerPawnController(), this);

        bAppliedDamage = true;
    }

    if (OtherComp->IsSimulatingPhysics())
    {
        OtherComp->AddImpulseAtLocation(GetVelocity() * ImpulseMultiplier, Hit.ImpactPoint);
    }

    if (bAppliedDamage)
    {
        ProjectileMovement->StopSimulating(Hit);
    }
}

void ASHGBaseProjectileActor::OnProjectileStop(const FHitResult& ImpactResult)
{
    if (!ProjectileMovement->bShouldBounce)
    {
        MakeImpactFX(ImpactResult);
    }

    Destroy();
}

void ASHGBaseProjectileActor::OnProjectileBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity)
{
    MakeImpactFX(ImpactResult, !ProjectileMovement->IsVelocityUnderSimulationThreshold());
}

AController* ASHGBaseProjectileActor::GetOwnerPawnController() const
{
    if (Owner)
    {
        const auto OwnerPawn = Owner->GetOwner<APawn>();
        return OwnerPawn ? OwnerPawn->GetController() : nullptr;
    }

    return nullptr;
}

void ASHGBaseProjectileActor::MakeImpactFX(const FHitResult& HitResult, bool bRicochet)
{
    FImpactFXData* ImpactFXData = bRicochet ? &DefaultRicochetFXData : &DefaultImpactFXData;
    if (HitResult.PhysMaterial.IsValid())
    {
        const auto& FXDataMap = bRicochet ? RicochetFXDataMap : ImpactFXDataMap;
        const auto PhysMaterial = HitResult.PhysMaterial.Get();
        if (FXDataMap.Contains(PhysMaterial))
        {
            ImpactFXData = &ImpactFXDataMap[PhysMaterial];
        }
    }

    // VFX
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(this,                                 //
                                                   ImpactFXData->ImpactNiagaraSystem,    //
                                                   HitResult.ImpactPoint,                //
                                                   HitResult.ImpactNormal.Rotation());

    // Sound
    UGameplayStatics::PlaySoundAtLocation(this, ImpactFXData->ImpactSound, HitResult.ImpactPoint);

    // Decal
    if (ImpactFXData->ImpactDecalData.Material.Num() != 0)
    {
        const auto RandomDecalArrayIndex = FMath::RandHelper(ImpactFXData->ImpactDecalData.Material.Num());
        auto DecalComponent = UGameplayStatics::SpawnDecalAtLocation(this,                                                             //
                                                                     ImpactFXData->ImpactDecalData.Material[RandomDecalArrayIndex],    //
                                                                     ImpactFXData->ImpactDecalData.Size,                               //
                                                                     HitResult.ImpactPoint,                                            //
                                                                     (HitResult.ImpactNormal * -1.0f).Rotation());                     // -1.0f to spawn decal the right side.

        if (DecalComponent)
        {
            DecalComponent->SetFadeScreenSize(ImpactFXData->ImpactDecalData.FadeScreenSize);
            DecalComponent->SetFadeOut(ImpactFXData->ImpactDecalData.LifeTime, ImpactFXData->ImpactDecalData.FadeOutTime);
        }
    }
}
