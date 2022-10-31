// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShooterDayNightSystem.generated.h"

class UMaterialParameterCollection;
class UBillboardComponent;
class ADirectionalLight;
class UCurveFloat;
class AShooterMoon;

UCLASS()
class SHOOTER_GAME_API AShooterDayNightSystem : public AActor
{
    GENERATED_BODY()

public:
    AShooterDayNightSystem();

    virtual void OnConstruction(const FTransform& Transform) override;

    UFUNCTION(BlueprintCallable)
    FTimespan GetCurrentTimespan() const { return FTimespan::FromSeconds(Time); }
    float GetCurrentDay() const;
    float GetCurrentHour() const;

protected:
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    UBillboardComponent* BillboardComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (ClampMin = "0.0"))
    float Speed = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (ClampMin = "0"))
    int32 Day = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (ClampMin = "0", ClampMax = "23"))
    int32 Hour = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (ClampMin = "0", ClampMax = "59"))
    int32 Minute = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", Meta = (ClampMin = "0", ClampMax = "59"))
    int32 Second = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Sun")
    ADirectionalLight* Sun;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Moon")
    AShooterMoon* Moon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Sun")
    UCurveFloat* SunRotationCurve;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Moon")
    UCurveFloat* MoonRotationCurve;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Moon")
    UCurveFloat* MoonPhaseCurve;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Moon")
    FVector2D MonLightIntensityRange = FVector2D(0.2f, 0.8f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Moon")
    FName PhaseParameterName = "Phase";

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Sun")
    float SunPositionYawOffset = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Moon")
    float MoonPositionYawOffset = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
    UMaterialParameterCollection* WeatherParameterCollection;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
    FName WindParameterCollectionName = "Wind";

    virtual void BeginPlay() override;

private:
    FTimerHandle DayNightSystemTimerHandle;

    UPROPERTY();
    float Time = 0.0f;

    float GetTimerRate() const;
    void DayNightSystemTimerTick();
    void SetWindValue();
    void UpdateSunRotation();
    void UpdateMoon();
    void UpdateMoonRotation();
    void UpdateMoonPhase();
    void UpdateMoonLightIntensity(float Phase);
};
