// Shooter_Game, All rights reserved.

#include "Weapons/SHGProjectileBaseActor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Sound/SoundCue.h"
#include "Components/DecalComponent.h"

#define ECC_Projectile ECollisionChannel::ECC_GameTraceChannel3

ASHGProjectileBaseActor::ASHGProjectileBaseActor()
{
    PrimaryActorTick.bCanEverTick = false;

    SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
    SphereComponent->SetSphereRadius(5.0f);
    SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    SphereComponent->SetCollisionObjectType(ECC_Projectile);
    SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
    SphereComponent->SetCollisionResponseToChannel(ECC_Projectile, ECollisionResponse::ECR_Ignore);
    SphereComponent->bReturnMaterialOnMove = true;
    SphereComponent->SetWalkableSlopeOverride(FWalkableSlopeOverride{WalkableSlope_Unwalkable, 0.0f});
    SphereComponent->CanCharacterStepUpOn = ECB_No;
    SetRootComponent(SphereComponent);

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovment");
    ProjectileMovement->InitialSpeed = 20000.0f;
    ProjectileMovement->ProjectileGravityScale = 0.3f;
}

void ASHGProjectileBaseActor::BeginPlay()
{
    Super::BeginPlay();

    check(SphereComponent);
    check(ProjectileMovement);

    // SphereComponent->IgnoreActorWhenMoving(GetOwner(), true);
    SphereComponent->OnComponentHit.AddDynamic(this, &ThisClass::OnProjectileHit);
    ProjectileMovement->Velocity = ShotDirection * ProjectileMovement->InitialSpeed;

    SetLifeSpan(LifeSpan);
}

void ASHGProjectileBaseActor::OnProjectileHit(UPrimitiveComponent* HitComponent,    //
                                              AActor* OtherActor,                   //
                                              UPrimitiveComponent* OtherComp,       //
                                              FVector NormalImpulse,                //
                                              const FHitResult& Hit)
{
    if (!OtherActor || OtherActor == this || !OtherComp)
        return;

    FPointDamageEvent DamageEvent;
    DamageEvent.HitInfo = Hit;
    OtherActor->TakeDamage(FMath::RandRange(Damage.X, Damage.Y), DamageEvent, GetController(), this);

    if (OtherComp->IsSimulatingPhysics())
    {
        OtherComp->AddImpulseAtLocation(GetVelocity() * ImpulseMultiplier, GetActorLocation());
    }

    ProjectileMovement->StopMovementImmediately();

    MakeImpactFX(Hit);

    Destroy();
}

AController* ASHGProjectileBaseActor::GetController() const
{
    if (Owner)
    {
        const auto OwnerPawn = Owner->GetOwner<APawn>();
        return OwnerPawn ? OwnerPawn->GetController() : nullptr;
    }

    return nullptr;
}

void ASHGProjectileBaseActor::MakeImpactFX(const FHitResult& HitResult)
{
    FImpactFXData* ImpactFXData = &DefaultImpactFXData;
    if (HitResult.PhysMaterial.IsValid())
    {
        if (const auto PhysMaterial = HitResult.PhysMaterial.Get(); ImpactFXDataMap.Contains(PhysMaterial))
        {
            ImpactFXData = &ImpactFXDataMap[PhysMaterial];
        }
    }

    // VFX
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(this,                                 //
                                                   ImpactFXData->ImpactNiagaraSystem,    //
                                                   HitResult.ImpactPoint,                //
                                                   HitResult.ImpactNormal.Rotation() + FRotator(-90.0f, 0.0f, 0.0f));

    // Sound
    UGameplayStatics::PlaySoundAtLocation(this, ImpactFXData->ImpactSound, HitResult.ImpactPoint);

    // Decal
    if (ImpactFXData->ImpactDecalData.Material.Num() == 0)
        return;

    const auto RandomDecalArrayIndex = FMath::RandHelper(ImpactFXData->ImpactDecalData.Material.Num());
    auto DecalComponent = UGameplayStatics::SpawnDecalAtLocation(this,                                                             //
                                                                 ImpactFXData->ImpactDecalData.Material[RandomDecalArrayIndex],    //
                                                                 ImpactFXData->ImpactDecalData.Size,                               //
                                                                 HitResult.ImpactPoint,                                            //
                                                                 (HitResult.ImpactNormal * -1.0f).Rotation());

    if (DecalComponent)
    {
        DecalComponent->SetFadeOut(ImpactFXData->ImpactDecalData.LifeTime, ImpactFXData->ImpactDecalData.FadeOutTime);
    }
}
