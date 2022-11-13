// Shooter_Game, All rights reserved.

#include "Components/ShooterPlayerVFXComponent.h"

USHGPlayerVFXComponent::USHGPlayerVFXComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USHGPlayerVFXComponent::MakeCameraShake(TSubclassOf<UCameraShakeBase> CameraShakeClass, float Scale)
{
    if (!CameraShakeClass)
        return;

    const auto OwnerPawn = GetOwner<APawn>();
    if (!OwnerPawn)
        return;

    const auto PlayerController = OwnerPawn->GetController<APlayerController>();
    if (!PlayerController || !PlayerController->PlayerCameraManager)
        return;

    PlayerController->PlayerCameraManager->StartCameraShake(CameraShakeClass, Scale);
}

void USHGPlayerVFXComponent::BeginPlay()
{
    Super::BeginPlay();

    GetOwner()->OnTakeAnyDamage.AddDynamic(this, &USHGPlayerVFXComponent::OnTakeAnyDamage);

    MakeCameraShake(LocomotionCameraShakeClass);
}

void USHGPlayerVFXComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
    MakeCameraShake(GetDamageCameraShakeClass);
}
