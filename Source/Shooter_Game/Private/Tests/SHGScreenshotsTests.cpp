// Shooter_Game, All rights reserved.

#if WITH_AUTOMATION_TESTS

#include "Tests/SHGScreenshotsTests.h"
#include "CoreMinimal.h"
#include "Tests/Utils/SHGTestsUtils.h"
#include "Tests/SHGTestConstants.h"
#include "Misc/AutomationTest.h"
#include "Camera/CameraActor.h"
#include "Player/SHGPlayerCharacter.h"

using namespace Tests;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRenderingShouldBeCorrect, "Shooter_Game.Screenshots.RenderingShouldBeCorrect",
                                 EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::MediumPriority |
                                     EAutomationTestFlags::NonNullRHI);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FHealthWidgetShouldBeRenderedCorrectlyAfterDamage, "Shooter_Game.Screenshots.HealthWidgetShouldBeRenderedCorrectlyAfterDamage",
                                 EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::MediumPriority |
                                     EAutomationTestFlags::NonNullRHI);

bool FRenderingShouldBeCorrect::RunTest(const FString& Parameters)
{
    const LevelScope Level(SandboxLevelName);

    UWorld* World = GetCurrentWorld();
    if (!TestNotNull("World must exists.", World))
        return false;

    APlayerController* PC = World->GetFirstPlayerController();
    if (!TestNotNull("Player controller must exists.", PC))
        return false;

    auto* Camera = World->SpawnActor<ACameraActor>(ACameraActor::StaticClass(), FTransform{
                                                                                    FRotator{0.0, -119.9, 0.0},
                                                                                    FVector{80.0f, -1270.0f, 190.0f},
                                                                                });
    if (!TestNotNull("Camera must exists.", Camera))
        return false;

    PC->SetViewTarget(Camera);

    auto Options = UAutomationBlueprintFunctionLibrary::GetDefaultScreenshotOptionsForGameplay();
    ADD_LATENT_AUTOMATION_COMMAND(FTakeGameScreenshotLatentCommand("RenderingScreenshot", Options));

    return true;
}

bool FHealthWidgetShouldBeRenderedCorrectlyAfterDamage::RunTest(const FString& Parameters)
{
    const LevelScope Level(SandboxLevelName);

    UWorld* World = GetCurrentWorld();
    if (!TestNotNull("World must exists.", World))
        return false;

    const APlayerController* PC = World->GetFirstPlayerController();
    if (!TestNotNull("Player controller must exists.", PC))
        return false;

    auto* Player = PC->GetPawn<ASHGPlayerCharacter>();
    if (!TestNotNull("Player must exists.", Player))
        return false;

    Player->TakeDamage(20.0f, {}, nullptr, nullptr);

    auto Options = UAutomationBlueprintFunctionLibrary::GetDefaultScreenshotOptionsForGameplay();
    Options.Delay = 1.0f;
    Options.VisualizeBuffer = "Opacity";
    ADD_LATENT_AUTOMATION_COMMAND(FTakeUIScreenshotLatentCommand("HealthWidgetScreenshot", Options));

    return true;
}

#endif    // WITH_AUTOMATION_TESTS
