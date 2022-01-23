// Shooter_Game, All rights reserved.

#include "Components/ShooterVFXComponent.h"

UShooterVFXComponent::UShooterVFXComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UShooterVFXComponent::MakeCameraShake(TSubclassOf<UMatineeCameraShake> CameraShakeClass, float Scale)
{
    const auto OwnerPawn = GetOwner<APawn>();
    if (!OwnerPawn)
        return;

    const auto PlayerController = OwnerPawn->GetController<APlayerController>();
    if (!PlayerController || !PlayerController->PlayerCameraManager)
        return;

    if (!CameraShakeClass)
        return;

    PlayerController->PlayerCameraManager->StartMatineeCameraShake(CameraShakeClass, Scale);
}

void UShooterVFXComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
    MakeCameraShake(GetDamageCameraShakeClass);
}

void UShooterVFXComponent::BeginPlay()
{
    Super::BeginPlay();

    GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UShooterVFXComponent::OnTakeAnyDamage);

    MakeCameraShake(LocomotionCameraShakeClass);
}
