#pragma once

namespace Debug
{
    static void Print(const FString& Msg, const FColor& Color = FColor::MakeRandomColor(),int32 InKey = -1)
    {
        if(GEngine)
        {
            GEngine->AddOnScreenDebugMessage(InKey,7.f,Color,Msg);
            UE_LOG(LogTemp,Warning,TEXT("%s"),*Msg);
        }
    }
    static void Print(const FString& Msg, float FloatValueToPrint,const FColor& Color = FColor::MakeRandomColor(),int32 InKey = -1)
    {
        if(GEngine)
        {
            const FString FinalMsg = Msg + TEXT(": ") + FString::SanitizeFloat(FloatValueToPrint);

            GEngine->AddOnScreenDebugMessage(InKey,7.f,Color,FinalMsg);
            UE_LOG(LogTemp,Warning,TEXT("%s"),*FinalMsg);
        }
    }

}