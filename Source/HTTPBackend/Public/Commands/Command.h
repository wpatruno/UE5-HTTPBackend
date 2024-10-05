#pragma once

#include "CoreMinimal.h"
#include "HttpBackendSubsystem.h"

struct FCommandDataRaw
{
	FString AdditivePath = FString();
	TMap<FString, FString> Parameters = TMap<FString, FString>();
	FString BaseURL = FString();
	FString OutputJson = FString();
	int32 StatusCode = 0;
};

class CommandGet
{
public:
	FCommandDataRaw Data = FCommandDataRaw();

	FCommandBaseCallback OnRequestFinish;

	virtual ~CommandGet() = default;

	void AddAdditivePath(const FString& NewPath);

	void AddParameter(const FString& Key, const FString& Value);

	virtual void SetOuput(FString JsonContent);

	FString GetRequestCompleteURL() const;
};


class CommandPost : public CommandGet
{
public:
	FString InputJson;

	CommandPost(FString I)
	{
		InputJson = I;
	}
};
