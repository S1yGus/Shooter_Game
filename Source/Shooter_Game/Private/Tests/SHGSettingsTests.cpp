// Shooter_Game, All rights reserved.

#if WITH_AUTOMATION_TESTS

#include "Tests/SHGSettingsTests.h"
#include "CoreMinimal.h"
#include "Tests/Utils/SHGTestsUtils.h"
#include "Misc/AutomationTest.h"
#include "Settings/SHGGameUserSettings.h"

using namespace Tests;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGameUserSettingsShouldExists, "Shooter_Game.Settings.GameUserSettingsShouldExists",
                                 EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

bool FGameUserSettingsShouldExists::RunTest(const FString& Parameters)
{
    TestNotNull("SHGGameUserSettings should exists", USHGGameUserSettings::Get());

    return true;
}

#endif    // WITH_AUTOMATION_TESTS
