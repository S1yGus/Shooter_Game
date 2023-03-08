// Shooter_Game, All rights reserved.

#if WITH_AUTOMATION_TESTS

#include "Tests/SHGUtilsTests.h"
#include "CoreMinimal.h"
#include "Tests/SHGTestsUtils.h"
#include "Misc/AutomationTest.h"
#include "SHGUtils.h"

using namespace Tests;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFormatStringFromIntTest, "Shooter_Game.Utils.FormatStringFromInt",
                                 EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

bool FFormatStringFromIntTest::RunTest(const FString& Parameters)
{
    AddInfo("FormatStringFromInt test.");

    using TestPayloadArray = TArray<TestPayload<TTuple<int32, int32, TCHAR>, FString>>;
    TestPayloadArray TestPayload{{{42, 6, 'x'}, "xxxx42"},    //
                                 {{42, 4, ' '}, "  42"},      //
                                 {{42, 2, ' '}, "42"},        //
                                 {{42, 1, ' '}, "42"},        //
                                 {{42, 0, ' '}, "42"},        //
                                 {{42, -42, ' '}, "42"}};

    for (const auto& OnePayload : TestPayload)
    {
        const auto& [InValue, MinLen, FillChar] = OnePayload.TestValue;
        TestTrueExpr(SHGUtils::FormatStringFromInt(InValue, MinLen, FillChar) == OnePayload.ExpectedValue);
    }

    return true;
}

#endif    // WITH_AUTOMATION_TESTS
