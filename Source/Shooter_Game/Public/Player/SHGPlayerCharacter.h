// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Player/SHGBaseCharacter.h"
#include "SHGSettingsCoreTypes.h"
#include "SHGPlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USphereComponent;

UCLASS()
class SHOOTER_GAME_API ASHGPlayerCharacter : public ASHGBaseCharacter
{
    GENERATED_BODY()

public:
    ASHGPlayerCharacter(const FObjectInitializer& ObjectInitializer);

    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    virtual void TurnOff() override;
    virtual void Reset() override;

protected:
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
    USpringArmComponent* SpringArmComponent;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
    UCameraComponent* CameraComponent;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
    USphereComponent* CameraCollisionComponent;

    virtual void OnDeath(AController* KillerController, AController* VictimController) override;

private:
    FSensitivitySettings SensitivitySettings;
    bool bIsMovingForward = false;

    void MoveForward(float Amount);
    void MoveRight(float Amount);
    void LookUp(float Amount);
    void LookRight(float Amount);

    inline virtual bool IsMovingForward() const override;
    void CheckCameraCollisionOverlap();

    void OnSensitivityChanged(const FSensitivitySettings& NewSensitivitySettings);
    UFUNCTION()
    void OnCameraComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                       bool bFromSweep, const FHitResult& SweepResult);
    UFUNCTION()
    void OnCameraComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
