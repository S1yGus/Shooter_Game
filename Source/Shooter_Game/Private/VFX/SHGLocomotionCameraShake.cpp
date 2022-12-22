// Shooter_Game, All rights reserved.

#include "VFX/SHGLocomotionCameraShake.h"
#include "WaveOscillatorCameraShakePattern.h"

USHGLocomotionCameraShake::USHGLocomotionCameraShake(const FObjectInitializer& ObjInit)
    : Super(ObjInit.SetDefaultSubobjectClass<UWaveOscillatorCameraShakePattern>("RootShakePattern"))
{
}

void USHGLocomotionCameraShake::UpdateLocomotionCameraShake(float InRotationAmplitudeMultiplier, float InRotationFrequencyMultiplier)
{
    if (auto OscillatorShakePattern = Cast<UWaveOscillatorCameraShakePattern>(GetRootShakePattern()))
    {
        OscillatorShakePattern->RotationAmplitudeMultiplier = InRotationAmplitudeMultiplier;
        OscillatorShakePattern->RotationFrequencyMultiplier = InRotationFrequencyMultiplier;
    }
}
