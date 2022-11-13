// Shooter_Game, All rights reserved.

#include "AI/ShooterAICharacter.h"
#include "AI/ShooterAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/ShooterAIWeaponComponent.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/Gameplay/SHGAIHealthBarUserWidget.h"
#include "Components/SHGHealthComponent.h"

constexpr static float UpdateHealthWidgetVisibilityTimerRate = 0.02f;

AShooterAICharacter::AShooterAICharacter(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer.SetDefaultSubobjectClass<USHGAIWeaponComponent>("WeaponComponent"))
{
    AutoPossessAI = EAutoPossessAI::Disabled;
    AIControllerClass = AShooterAIController::StaticClass();

    bUseControllerRotationYaw = false;
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->bUseControllerDesiredRotation = true;
        GetCharacterMovement()->RotationRate = FRotator(0.0f, 0.0f, 300.0f);
    }

    HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("HealthWidgetComponent");
    HealthWidgetComponent->SetupAttachment(GetRootComponent());
    HealthWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
    HealthWidgetComponent->SetDrawSize(FVector2D(100.0, 6.0));
    HealthWidgetComponent->SetDrawAtDesiredSize(false);
}

void AShooterAICharacter::BeginPlay()
{
    Super::BeginPlay();

    check(HealthWidgetComponent);

    OnHealthChanged(HealthComponent->GetHealth(), HealthComponent->GetHealthPercent());

    GetWorldTimerManager().SetTimer(UpdateHealthWidgetVisibilityTimerHandle,     //
                                    this,                                        //
                                    &AShooterAICharacter::UpdateHealthWidget,    //
                                    UpdateHealthWidgetVisibilityTimerRate,       //
                                    true);
}

void AShooterAICharacter::OnHealthChanged(float Health, float HealthPercent)
{
    Super::OnHealthChanged(Health, HealthPercent);

    const auto HealthWidget = Cast<USHGAIHealthBarUserWidget>(HealthWidgetComponent->GetUserWidgetObject());
    if (!HealthWidget)
        return;

    HealthWidget->SetHealthPercent(HealthPercent);
}

void AShooterAICharacter::UpdateHealthWidget()
{
    if (!GetWorld()->GetFirstPlayerController()                             //
        || !GetWorld()->GetFirstPlayerController()->GetPawn()               //
        || !GetWorld()->GetFirstPlayerController()->PlayerCameraManager)    //
        return;

    const auto PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
    const auto DistanceBetween = FVector::Distance(PlayerLocation, GetActorLocation());
    HealthWidgetComponent->SetVisibility(DistanceBetween < DistanceBetweenThreshold);

    auto NewRotation = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraRotation();
    NewRotation.Yaw += 180.0f;
    NewRotation.Pitch *= -1.0f;
    HealthWidgetComponent->SetWorldRotation(NewRotation);
}

void AShooterAICharacter::OnDeath()
{
    Super::OnDeath();

    const auto AIController = Cast<AAIController>(Controller);
    if (!AIController || !AIController->BrainComponent)
        return;

    AIController->BrainComponent->Cleanup();
}
