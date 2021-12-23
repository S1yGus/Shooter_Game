#pragma once

#include "Animation/AnimSequenceBase.h"

class AnimUtils
{
public:
    template <class T> 
    static T* FindNotifyByClass(UAnimSequenceBase* Animation)
    {
        if (!Animation)
            return nullptr;

        const auto AnimNotifyEvents = Animation->Notifies;
        for (const auto AnimNotifyEvent : AnimNotifyEvents)
        {
            T* AnimNotify = Cast<T>(AnimNotifyEvent.Notify);
            if (AnimNotify)
                return AnimNotify;
        }

        return nullptr;
    }
};
