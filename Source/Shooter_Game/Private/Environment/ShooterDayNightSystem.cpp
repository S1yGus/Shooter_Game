// Shooter_Game, All rights reserved.

#include "Environment/ShooterDayNightSystem.h"
#include "Components/BillboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "Curves/CurveFloat.h"
#include "Engine/DirectionalLight.h"
#include "Environment/ShooterMoon.h"
#include "Components/DirectionalLightComponent.h"

constexpr static float UnitOfTime = 1.0f;    // in seconds
constexpr static float MinTimerRate = 0.07f;
constexpr static int32 SecondsInDay = 86400;
constexpr static int32 SecondsInHour = 3600;
constexpr static int32 SecondsInMinute = 60;
constexpr static float MoonLightActivityThreshold = -0.02f;

AShooterDayNightSystem::AShooterDayNightSystem()
{
    PrimaryActorTick.bCanEverTick = false;

    BillboardComponent = CreateDefaultSubobject<UBillboardComponent>("BillboardComponent");
    SetRootComponent(BillboardComponent);
}

void AShooterDayNightSystem::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

    Time = Day * SecondsInDay + Hour * SecondsInHour + Minute * SecondsInMinute + Second;
    SunPositionYawOffset = Sun->GetActorRotation().Yaw;
    MoonPositionYawOffset = Moon->GetActorRotation().Yaw;

    UpdateSunRotation();
    UpdateMoon();
}

float AShooterDayNightSystem::GetCurrentDay() const
{
    return Time / SecondsInDay;
}

float AShooterDayNightSystem::GetCurrentHour() const
{
    return FMath::Fmod(Time, SecondsInDay) / SecondsInHour;
}

void AShooterDayNightSystem::BeginPlay()
{
    Super::BeginPlay();

    SetWindValue();

    GetWorldTimerManager().SetTimer(DayNightSystemTimerHandle, this, &AShooterDayNightSystem::DayNightSystemTimerTick, GetTimerRate(), true);
}

float AShooterDayNightSystem::GetTimerRate() const
{
    if (FMath::IsNearlyZero(Speed))
        return 0.0f;

    return FMath::Max(UnitOfTime / Speed, MinTimerRate);
}

void AShooterDayNightSystem::DayNightSystemTimerTick()
{
    Time += Speed * GetTimerRate();

    UpdateSunRotation();
    UpdateMoon();

    // Debug
    /*FString CurrentTime = FString::Printf(TEXT("Dsays: %i Time: %i:%i:%i"), GetCurrentTimespan().GetDays(), GetCurrentTimespan().GetHours(),
                                          GetCurrentTimespan().GetMinutes(), GetCurrentTimespan().GetSeconds());
    GEngine->AddOnScreenDebugMessage(-1, GetTimerRate(), FColor::Red, CurrentTime);*/
}

void AShooterDayNightSystem::SetWindValue()
{
    if (!WeatherParameterCollection)
        return;

    const auto WeatherParameterCollectionInst = GetWorld()->GetParameterCollectionInstance(WeatherParameterCollection);
    const auto CurrentWindValue = WeatherParameterCollection->GetVectorParameterByName(WindParameterCollectionName);
    if (CurrentWindValue)
    {
        WeatherParameterCollectionInst->SetVectorParameterValue(WindParameterCollectionName, CurrentWindValue->DefaultValue * Speed);
    }
}

void AShooterDayNightSystem::UpdateSunRotation()
{
    if (!Sun || !SunRotationCurve)
        return;

    const auto SunPitch = SunRotationCurve->GetFloatValue(GetCurrentHour());
    Sun->SetActorRotation(FRotator(SunPitch, SunPositionYawOffset, 0.0f));
}

void AShooterDayNightSystem::UpdateMoon()
{
    UpdateMoonRotation();
    UpdateMoonPhase();
}

void AShooterDayNightSystem::UpdateMoonRotation()
{
    if (!Moon || !MoonRotationCurve)
        return;

    const auto NewMoonPitch = MoonRotationCurve->GetFloatValue(GetCurrentHour());
    Moon->SetActorRotation(FRotator(NewMoonPitch, MoonPositionYawOffset, 0.0f));
}

void AShooterDayNightSystem::UpdateMoonPhase()
{
    if (!MoonPhaseCurve || !Moon->GetStaticMesh())
        return;

    const auto Phase = MoonPhaseCurve->GetFloatValue(FMath::Fmod(GetCurrentDay(), 28.0f));
    const auto MoonMaterialInst = Moon->GetStaticMesh()->CreateAndSetMaterialInstanceDynamic(0);
    MoonMaterialInst->SetScalarParameterValue(PhaseParameterName, Phase);

    UpdateMoonLightIntensity(Phase);
}

void AShooterDayNightSystem::UpdateMoonLightIntensity(float Phase)
{
    if (!Moon->GetDirectionalLight())
        return;

    const auto MoonDirectionalLight = Moon->GetDirectionalLight();
    MoonDirectionalLight->SetIntensity(FMath::GetMappedRangeValueClamped(FVector2D(0.0f, 1.0f), MonLightIntensityRange, FMath::Abs(Phase)));

    // helps to avoid sun god rays distortion
    FVector::DotProduct(Moon->GetActorUpVector(), this->GetActorUpVector()) < MoonLightActivityThreshold ? Moon->GetDirectionalLight()->SetVisibility(false)
                                                                                                         : Moon->GetDirectionalLight()->SetVisibility(true);
}
