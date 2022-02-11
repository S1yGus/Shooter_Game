// Shooter_Game, All rights reserved.

#include "AI/ShooterAICharacter.h"
#include "AI/ShooterAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/ShooterAIWeaponComponent.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/ShooterHealthBarUserWidget.h"
#include "Components/ShooterHealthComponent.h"

constexpr static float UpdateHealthWidgetVisibilityTimerRate = 0.2f;

AShooterAICharacter::AShooterAICharacter(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer.SetDefaultSubobjectClass<UShooterAIWeaponComponent>("WeaponComponent"))
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
    HealthWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
    HealthWidgetComponent->SetDrawAtDesiredSize(true);
}

void AShooterAICharacter::BeginPlay()
{
    Super::BeginPlay();

    check(HealthWidgetComponent);

    OnHealthChanged(HealthComponent->GetHealth());

    GetWorldTimerManager().SetTimer(UpdateHealthWidgetVisibilityTimerHandle,               //
                                    this,                                                  //
                                    &AShooterAICharacter::UpdateHealthWidgetVisibility,    //
                                    UpdateHealthWidgetVisibilityTimerRate,                 //
                                    true);
}

void AShooterAICharacter::OnHealthChanged(float Health)
{
    Super::OnHealthChanged(Health);

    const auto HealthWidget = Cast<UShooterHealthBarUserWidget>(HealthWidgetComponent->GetUserWidgetObject());
    if (!HealthWidget)
        return;

    HealthWidget->SetHealthPercent(HealthComponent->GetHealthPercent());
}

void AShooterAICharacter::UpdateHealthWidgetVisibility()
{
    if (!GetWorld()->GetFirstPlayerController() || !GetWorld()->GetFirstPlayerController()->GetPawn())
        return;

    const auto DistanceBetween = FVector::Distance(GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation(), GetActorLocation());

    FHitResult HitResult;
    FCollisionQueryParams CollisionObjectQueryParams;
    CollisionObjectQueryParams.AddIgnoredActor(GetWorld()->GetFirstPlayerController()->GetPawn());
    CollisionObjectQueryParams.AddIgnoredActor(this);
    GetWorld()->LineTraceSingleByChannel(HitResult,                                                                //
                                         GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation(),    //
                                         GetActorLocation(),                                                       //
                                         ECollisionChannel::ECC_Visibility,                                        //
                                         CollisionObjectQueryParams);

    HealthWidgetComponent->SetVisibility(DistanceBetween < DistanceBetweenThreshold && !HitResult.bBlockingHit, true);
}

void AShooterAICharacter::OnDeath()
{
    Super::OnDeath();

    const auto AIController = Cast<AAIController>(Controller);
    if (!AIController || !AIController->BrainComponent)
        return;

    AIController->BrainComponent->Cleanup();
}
