// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#include "BackendData.h"
#include "Commands/Command.h"
#include "Interfaces/IHttpRequest.h"
#include "Modules/ModuleManager.h"


/* CHECK WebAPI */
class FHttpBackendModule : public IModuleInterface
{
public:
	int Value;
	int GetValue() const { return Value; }
	FBackendSettings Setting;

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	void SetAuthentification(FString NewToken);
	TMap<FHttpRequestPtr, CommandGet*> Requests;
	void Response(TSharedPtr<IHttpRequest> HttpRequest, TSharedPtr<IHttpResponse> HttpResponse, bool bArg);
	void SendGet(CommandGet& Command);
	void SendPost(CommandPost& Command, const FCommandCallback& Callback) const;
	void SendRequestWebApi();

private:
	TSharedRef<IHttpRequest> MakeBaseRequest() const;
};
