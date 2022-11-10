#include "Tests/SHGTestsUtils.h"
#include "Misc/OutputDeviceNull.h"

UWorld* Tests::GetCurrentWorld()
{
    if (!GEngine)
        return nullptr;

    for (const auto& Context : GEngine->GetWorldContexts())
    {
        if ((Context.WorldType == EWorldType::PIE || Context.WorldType == EWorldType::GamePreview) && Context.World() != nullptr)
        {
            return Context.World();
        }
    }

    return nullptr;
}

void Tests::CallFunctionByNameWithParameters(UObject* Object, const FString& FuncName, const TArray<FString>& Params)
{
    FString Command{FuncName};
    for (const auto& Param : Params)
    {
        Command.Append(" ").Append(Param);
    }

    FOutputDeviceNull OutputDeviceNull;
    Object->CallFunctionByNameWithArguments(*Command, OutputDeviceNull, nullptr, true);
}
