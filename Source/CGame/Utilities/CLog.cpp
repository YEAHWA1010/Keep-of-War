#include "Utilities/CLog.h"
#include "Engine.h"

DEFINE_LOG_CATEGORY_STATIC(Game, Display, All)

void CLog::Log(int32 InValue)
{
	//GLog->Log("Game", ELogVerbosity::Display, FString::FromInt(InValue));

	UE_LOG(Game, Display, L"%d", InValue);
}

void CLog::Log(float InValue)
{
	UE_LOG(Game, Display, L"%f", InValue);
}

void CLog::Log(const FString & InValue)
{
	UE_LOG(Game, Display, L"%s", *InValue);
}

void CLog::Log(const FVector & InValue)
{
	UE_LOG(Game, Display, L"%s", *InValue.ToString());
}

void CLog::Log(const FRotator & InValue)
{
	UE_LOG(Game, Display, L"%s", *InValue.ToString());
}

void CLog::Log(const UObject * InValue)
{
	FString str;

	if (!!InValue)
		str.Append(InValue->GetName());

	str.Append(!!InValue ? " Not Null" : "Null");

	UE_LOG(Game, Display, L"%s", *str);
}

void CLog::Print(int32 InValue, int32 InKey, float InDuration, FColor InColor)
{
	GEngine->AddOnScreenDebugMessage(InKey, InDuration, InColor, FString::FromInt(InValue));
}

void CLog::Print(float InValue, int32 InKey, float InDuration, FColor InColor)
{
	GEngine->AddOnScreenDebugMessage(InKey, InDuration, InColor, FString::SanitizeFloat(InValue));
}

void CLog::Print(const FString& InValue, int32 InKey, float InDuration, FColor InColor)
{
	GEngine->AddOnScreenDebugMessage(InKey, InDuration, InColor, InValue);
}

void CLog::Print(const FVector& InValue, int32 InKey, float InDuration, FColor InColor)
{
	GEngine->AddOnScreenDebugMessage(InKey, InDuration, InColor, InValue.ToString());
}

void CLog::Print(const FRotator& InValue, int32 InKey, float InDuration, FColor InColor)
{
	GEngine->AddOnScreenDebugMessage(InKey, InDuration, InColor, InValue.ToString());
}

void CLog::Print(const UObject* InValue, int32 InKey, float InDuration, FColor InColor)
{
	FString str;

	if (!!InValue)
		str.Append(InValue->GetName());

	str.Append(!!InValue ? " Not Null" : "Null");

	GEngine->AddOnScreenDebugMessage(InKey, InDuration, InColor, str);
}