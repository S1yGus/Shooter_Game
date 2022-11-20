// Shooter_Game, All rights reserved.

#include "Components/SHGPlayerFXComponent.h"

USHGPlayerFXComponent::USHGPlayerFXComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USHGPlayerFXComponent::BeginPlay()
{
    Super::BeginPlay();

    check(GetDamageCameraShakeClass);
    check(LocomotionCameraShakeClass);

    if (const auto Owner = GetOwner())
    {
        Owner->OnTakeAnyDamage.AddDynamic(this, &ThisClass::OnTakeAnyDamage);
    }

    MakeCameraShake(LocomotionCameraShakeClass);
}

void USHGPlayerFXComponent::MakeCameraShake(TSubclassOf<UCameraShakeBase> CameraShakeClass, float Scale)
{
    const auto OwnerPawn = GetOwner<APawn>();
    if (!OwnerPawn)
        return;

    const auto PC = OwnerPawn->GetController<APlayerController>();
    if (!PC || !PC->PlayerCameraManager)
        return;

    PC->PlayerCameraManager->StartCameraShake(CameraShakeClass, Scale);
}

void USHGPlayerFXComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
    const auto Scale = FMath::GetMappedRangeValueClamped(CameraShakeDamageRange, FVector2D{0.0f, 1.0f}, Damage);
    MakeCameraShake(GetDamageCameraShakeClass, Scale);
}
