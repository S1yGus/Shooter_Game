// Shooter_Game, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterBaseCharacter.generated.h"

DEFINE_LOG_CATEGORY_STATIC(ShooterBaseCharacterLog, All, All)

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class SHOOTER_GAME_API AShooterBaseCharacter : public ACharacter
{
    GENERATED_BODY()

  public:
    // Sets default values for this character's properties
    AShooterBaseCharacter();

  protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement", Meta = (ClampMin = "500", ClampMax = "800"))
    float NormalWalkSpeed = 600.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement", Meta = (ClampMin = "800", ClampMax = "1200"))
    float MaxWalkSpeed = 1200.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USpringArmComponent* SpringArmComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UCameraComponent* CameraComponent;

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

  public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UFUNCTION(BlueprintCallable, Category = "Movement")
    bool IsSprinting() const;

    UFUNCTION(BlueprintCallable, Category = "Movement")
    float GetMovementDirection() const;

  private:
    bool WantsToSprint = false;
    bool MovingForward = false;

    void MoveForward(float Amount);
    void MoveRight(float Amount);

    void StartSprint();
    void StopSprint();
};
