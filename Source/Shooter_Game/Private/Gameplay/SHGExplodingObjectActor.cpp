// Shooter_Game, All rights reserved.

#include "Gameplay/SHGExplodingObjectActor.h"
#include "Components/SHGHealthComponent.h"
#include "Components/DecalComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "SHGUtils.h"

constexpr static float MinimumExplosionDamage = 0.0f, InnerExplosionRadius = 0.0f;

ASHGExplodingObjectActor::ASHGExplodingObjectActor()
{
    PrimaryActorTick.bCanEverTick = false;

    HealthComponent = CreateDefaultSubobject<USHGHealthComponent>("HealthComponent");
}

void ASHGExplodingObjectActor::BeginPlay()
{
    Super::BeginPlay();

    check(HealthComponent);

    HealthComponent->OnDeath.AddUObject(this, &ThisClass::OnDeath);
}

void ASHGExplodingObjectActor::MakeImpactFX()
{
    const auto ActorLocation = GetActorLocation();
    FHitResult HitResult;
    GetWorld()->LineTraceSingleByChannel(HitResult,                                                 //
                                         ActorLocation,                                             //
                                         ActorLocation + FVector{0.0, 0.0, -1.0} * DamageRadius,    // On Z axis.
                                         ECollisionChannel::ECC_Visibility);

    FImpactFXData* ImpactFXData = &DefaultFXData;
    if (HitResult.PhysMaterial.IsValid())
    {
        const auto PhysMaterial = HitResult.PhysMaterial.Get();
        if (FXDataMap.Contains(PhysMaterial))
        {
            ImpactFXData = &FXDataMap[PhysMaterial];
        }
    }

    // VFX
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(this,                                 //
                                                   ImpactFXData->ImpactNiagaraSystem,    //
                                                   ActorLocation);

    // Sound
    UGameplayStatics::PlaySoundAtLocation(this, ImpactFXData->ImpactSound, ActorLocation);

    // CameraShake
    UGameplayStatics::PlayWorldCameraShake(this,                                                  //
                                           ExplosionCameraShakeData.ExplosionCameraShakeClass,    //
                                           GetActorLocation(),                                    //
                                           InnerExplosionRadius,                                  //
                                           ExplosionCameraShakeData.ExplosionCameraShakeRadius);

    // Decal
    if (HitResult.bBlockingHit && ImpactFXData->ImpactDecalData.Material.Num() != 0)
    {
        const auto RandomDecalArrayIndex = FMath::RandHelper(ImpactFXData->ImpactDecalData.Material.Num());
        auto DecalComponent = UGameplayStatics::SpawnDecalAtLocation(this,                                                             //
                                                                     ImpactFXData->ImpactDecalData.Material[RandomDecalArrayIndex],    //
                                                                     ImpactFXData->ImpactDecalData.Size,                               //
                                                                     HitResult.ImpactPoint,                                            //
                                                                     (HitResult.ImpactNormal * -1.0f).Rotation());    // -1.0f to spawn decal the right side.

        if (DecalComponent)
        {
            DecalComponent->SetFadeScreenSize(ImpactFXData->ImpactDecalData.FadeScreenSize);
            DecalComponent->SetFadeOut(ImpactFXData->ImpactDecalData.LifeTime, ImpactFXData->ImpactDecalData.FadeOutTime);
        }
    }
}

void ASHGExplodingObjectActor::OnDeath(AController* KillerController, AController* VictimController)
{
    SHGUtils::ApplyRadialDamageWithImpulse(this,                                    //
                                           FMath::RandRange(Damage.X, Damage.Y),    //
                                           MinimumExplosionDamage,                  //
                                           ImpulsePower,                            //
                                           GetActorLocation(),                      //
                                           InnerExplosionRadius,                    //
                                           DamageRadius,                            //
                                           bDoFullDamage,                           //
                                           UDamageType::StaticClass(),              //
                                           {},                                      //
                                           this,                                    //
                                           KillerController,                        //
                                           DamageTraceChannel);

    MakeImpactFX();

    OnExplosion();
}
