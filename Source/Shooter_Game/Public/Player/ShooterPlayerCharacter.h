// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Player/ShooterBaseCharacter.h"
#include "ShooterPlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USphereComponent;

UCLASS()
class SHOOTER_GAME_API AShooterPlayerCharacter : public AShooterBaseCharacter
{
    GENERATED_BODY()

public:
    AShooterPlayerCharacter(const FObjectInitializer& ObjectInitializer);

    virtual void BeginPlay() override;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    virtual void TurnOff() override;
    virtual void Reset() override;

protected:
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    USpringArmComponent* SpringArmComponent;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    UCameraComponent* CameraComponent;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    USphereComponent* CameraCollisionComponent;

    virtual void OnDeath() override;

private:
    void MoveForward(float Amount);
    void MoveRight(float Amount);

    void StartSprint();
    void StopSprint();

    UFUNCTION()
    void OnCameraComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent,    //
                                       AActor* OtherActor,                          //
                                       UPrimitiveComponent* OtherComp,              //
                                       int32 OtherBodyIndex,                        //
                                       bool bFromSweep,                             //
                                       const FHitResult& SweepResult);

    UFUNCTION()
    void OnCameraComponentEndOverlap(UPrimitiveComponent* OverlappedComponent,    //
                                     AActor* OtherActor,                          //
                                     UPrimitiveComponent* OtherComp,              //
                                     int32 OtherBodyIndex);

    void CheckCameraCollisionOverlap();
};
