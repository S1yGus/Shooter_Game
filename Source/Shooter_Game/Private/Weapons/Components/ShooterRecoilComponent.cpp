// Shooter_Game, All rights reserved.

#include "Weapons/Components/ShooterRecoilComponent.h"
#include "Kismet/KismetMathLibrary.h"

constexpr static float RecoilTimerRate = 0.007f;

UShooterRecoilComponent::UShooterRecoilComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UShooterRecoilComponent::MakeRecoil()
{
    if (!CalculateRecoil())
        return;

    GetWorld()->GetTimerManager().SetTimer(RecoilTimerHandle, this, &UShooterRecoilComponent::RecoilTimerTick, RecoilTimerRate, true);
}

void UShooterRecoilComponent::StopRecoilRecoverTimer()
{
    GetWorld()->GetTimerManager().ClearTimer(RecoilRecoveryTimerHandle);
}

APawn* UShooterRecoilComponent::GetOwnerPawn()
{
    return GetOwner() ? GetOwner()->GetOwner<APawn>() : nullptr;
}

bool UShooterRecoilComponent::CalculateRecoil()
{
    const auto OwnerPawn = GetOwnerPawn();
    if (!OwnerPawn)
        return false;

    InitialControllerInputRotation = OwnerPawn->GetViewRotation();

    const auto NumberOfTicks = RecoilTime / RecoilTimerRate;
    CurrentPitchRecoil = FMath::RandRange(MinPitchRecoilMagnitude, MaxPitchRecoilMagnitude) / NumberOfTicks;
    CurrentYawRecoil = FMath::RandRange(MinYawRecoilMagnitude, MaxYawRecoilMagnitude) / NumberOfTicks;

    CurrentPitchRecoil /= OwnerPawn->GetController<APlayerController>()->InputPitchScale_DEPRECATED;
    CurrentYawRecoil /= OwnerPawn->GetController<APlayerController>()->InputYawScale_DEPRECATED;

    CurrentRecoveryPitchRecoil = CurrentPitchRecoil / -RecoilRecoverScale;
    CurrentRecoveryYawRecoil = CurrentYawRecoil / -RecoilRecoverScale;

    return true;
}

void UShooterRecoilComponent::RecoilTimerTick()
{
    const auto OwnerPawn = GetOwnerPawn();
    if (!OwnerPawn)
        return;

    OwnerPawn->AddControllerPitchInput(CurrentPitchRecoil);
    OwnerPawn->AddControllerYawInput(CurrentYawRecoil);

    CurrentRecoilTime += RecoilTimerRate;
    if (CurrentRecoilTime >= RecoilTime)
    {
        GetWorld()->GetTimerManager().ClearTimer(RecoilTimerHandle);
        CurrentRecoilTime = 0.0f;

        GetWorld()->GetTimerManager().SetTimer(RecoilRecoveryTimerHandle, this, &UShooterRecoilComponent::RecoilRecoveryTimerTick, RecoilTimerRate, true);
    }
}

void UShooterRecoilComponent::RecoilRecoveryTimerTick()
{
    const auto OwnerPawn = GetOwnerPawn();
    if (!OwnerPawn)
        return;

    const auto ControllerInputCurrentRotation = OwnerPawn->GetViewRotation();
    const auto Delta = UKismetMathLibrary::NormalizedDeltaRotator(ControllerInputCurrentRotation, InitialControllerInputRotation);
    if (Delta.Pitch >= MaxPitchRecoilMagnitude + RecoilRecoverPitchAdditionalOffset             //
        || FMath::Abs(Delta.Yaw) >= MaxYawRecoilMagnitude + RecoilRecoverYawAdditionalOffset    //
        || Delta.Pitch <= 0)                                                                    //
    {
        GetWorld()->GetTimerManager().ClearTimer(RecoilRecoveryTimerHandle);
        return;
    }

    OwnerPawn->AddControllerPitchInput(CurrentRecoveryPitchRecoil);
    OwnerPawn->AddControllerYawInput(CurrentRecoveryYawRecoil);
}