// Shooter_Game, All rights reserved.

#if WITH_AUTOMATION_TESTS

#include "Tests/SHGPlayerControllerTests.h"
#include "CoreMinimal.h"
#include "Tests/Utils/SHGTestsUtils.h"
#include "Misc/AutomationTest.h"

using namespace Tests;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGameCanBePaused, "Shooter_Game.PlayerController.GameCanBePaused",
                                 EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGameCanBeUnpaused, "Shooter_Game.PlayerController.GameCanBeUnpaused",
                                 EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

bool FGameCanBePaused::RunTest(const FString& Parameters)
{
    const LevelScope Level("/Game/Levels/SandboxLevel");

    const UWorld* World = GetCurrentWorld();
    if (!TestNotNull("World must exists.", World))
        return false;

    const APlayerController* PC = World->GetFirstPlayerController();
    if (!TestNotNull("Player controller must exists.", PC))
        return false;

    TestTrueExpr(!PC->IsPaused());
    TestTrueExpr(!PC->bShowMouseCursor);

    ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(0.66f));
    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
        [=]()
        {
            DoInputAction(PC->InputComponent, "Esc", EInputEvent::IE_Pressed, EKeys::P);
            return true;
        }));
    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
        [=]()
        {
            TestTrueExpr(PC->IsPaused());
            return true;
        }));
    ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(0.51f));
    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
        [=]()
        {
            TestTrueExpr(PC->bShowMouseCursor);
            return true;
        }));

    return true;
}

bool FGameCanBeUnpaused::RunTest(const FString& Parameters)
{
    const LevelScope Level("/Game/Levels/SandboxLevel");

    const UWorld* World = GetCurrentWorld();
    if (!TestNotNull("World must exists.", World))
        return false;

    const APlayerController* PC = World->GetFirstPlayerController();
    if (!TestNotNull("Player controller must exists.", PC))
        return false;

    TestTrueExpr(!PC->IsPaused());
    TestTrueExpr(!PC->bShowMouseCursor);

    ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(0.66f));
    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
        [=]()
        {
            DoInputAction(PC->InputComponent, "Esc", EInputEvent::IE_Pressed, EKeys::P);
            return true;
        }));
    ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(0.82f));
    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
        [=]()
        {
            DoInputAction(PC->InputComponent, "Esc", EInputEvent::IE_Pressed, EKeys::P);
            return true;
        }));
    ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(0.31f));
    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
        [=]()
        {
            TestTrueExpr(!PC->IsPaused());
            TestTrueExpr(!PC->bShowMouseCursor);
            return true;
        }));

    return true;
}

#endif    // WITH_AUTOMATION_TESTS
