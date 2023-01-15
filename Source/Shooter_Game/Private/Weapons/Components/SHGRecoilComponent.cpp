// Shooter_Game, All rights reserved.

#include "Weapons/Components/SHGRecoilComponent.h"
#include "Kismet/KismetMathLibrary.h"

constexpr static float RecoilTimerRate = 0.007f;

USHGRecoilComponent::USHGRecoilComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USHGRecoilComponent::MakeRecoil()
{
    if (CalculateRecoil())
    {
        GetWorld()->GetTimerManager().SetTimer(RecoilTimerHandle, this, &ThisClass::OnRecoilUpdate, RecoilTimerRate, true);
    }
}

void USHGRecoilComponent::StopRecoilRecoverTimer()
{
    GetWorld()->GetTimerManager().ClearTimer(RecoilRecoveryTimerHandle);
}

APawn* USHGRecoilComponent::GetOwnerPawn()
{
    const auto Owner = GetOwner();
    return Owner ? Owner->GetOwner<APawn>() : nullptr;
}

bool USHGRecoilComponent::CalculateRecoil()
{
    const auto OwnerPawn = GetOwnerPawn();
    if (!OwnerPawn)
        return false;

    InitialControllerInputRotation = OwnerPawn->GetViewRotation();

    const auto NumberOfTicks = RecoilTime / RecoilTimerRate;
    CurrentPitchRecoil = FMath::RandRange(MinPitchRecoilMagnitude, MaxPitchRecoilMagnitude) / NumberOfTicks;
    CurrentYawRecoil = FMath::RandRange(MinYawRecoilMagnitude, MaxYawRecoilMagnitude) / NumberOfTicks;

    CurrentRecoveryPitchRecoil = CurrentPitchRecoil / -RecoilRecoverScale;
    CurrentRecoveryYawRecoil = CurrentYawRecoil / -RecoilRecoverScale;

    return true;
}

void USHGRecoilComponent::OnRecoilUpdate()
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

        GetWorld()->GetTimerManager().SetTimer(RecoilRecoveryTimerHandle, this, &ThisClass::OnRecoilRecoveryUpdate, RecoilTimerRate, true);
    }
}

void USHGRecoilComponent::OnRecoilRecoveryUpdate()
{
    const auto OwnerPawn = GetOwnerPawn();
    if (!OwnerPawn)
        return;

    const auto ControllerInputCurrentRotation = OwnerPawn->GetViewRotation();
    const auto Delta = UKismetMathLibrary::NormalizedDeltaRotator(ControllerInputCurrentRotation, InitialControllerInputRotation);
    if (Delta.Pitch >= MaxPitchRecoilMagnitude + RecoilRecoverPitchAdditionalOffset             //
        || FMath::Abs(Delta.Yaw) >= MaxYawRecoilMagnitude + RecoilRecoverYawAdditionalOffset    //
        || Delta.Pitch <= 0.0)                                                                  //
    {
        GetWorld()->GetTimerManager().ClearTimer(RecoilRecoveryTimerHandle);
        return;
    }

    OwnerPawn->AddControllerPitchInput(CurrentRecoveryPitchRecoil);
    OwnerPawn->AddControllerYawInput(CurrentRecoveryYawRecoil);
}
