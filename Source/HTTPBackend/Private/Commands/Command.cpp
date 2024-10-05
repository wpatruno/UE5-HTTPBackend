#include "Commands/Command.h"

void CommandGet::AddAdditivePath(const FString& NewPath)
{
	Data.AdditivePath += "/" + NewPath;
}

void CommandGet::AddParameter(const FString& Key, const FString& Value)
{
	Data.Parameters.Add(Key, Value);
}

void CommandGet::SetOuput(FString JsonContent)
{
	Data.OutputJson = JsonContent;
}

FString CommandGet::GetRequestCompleteURL() const
{
	FString Url = Data.BaseURL;
	if (!Data.AdditivePath.IsEmpty())
	{
		Url += "/" + Data.AdditivePath;
	}
	if (Data.Parameters.Num() > 0)
	{
		Url += TEXT("?");
		bool IsFirst = true;
		for (auto Param : Data.Parameters)
		{
			if (IsFirst)
			{
				IsFirst = false;
				Url += Param.Key + "=" + Param.Value;
			}
			else
			{
				Url += "&" + Param.Key + "=" + Param.Value;
			}
		}
	}
	return Url;
}
