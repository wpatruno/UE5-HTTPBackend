// Copyright Epic Games, Inc. All Rights Reserved.

#include "HttpBackend.h"

#include "HttpModule.h"
#include "Commands/Command.h"
#include "Interfaces/IHttpResponse.h"

#define LOCTEXT_NAMESPACE "FHTTPBackendModule"

void FHttpBackendModule::StartupModule()
{
	Value = 8;
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	UE_LOG(LogTemp, Warning, TEXT("FHTTPBackendModule::START"));
}

void FHttpBackendModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	UE_LOG(LogTemp, Warning, TEXT("FHTTPBackendModule::SHUTDOWN"));
}

void FHttpBackendModule::SetAuthentification(FString NewToken)
{
	Setting.Token = NewToken;
}

void FHttpBackendModule::Response(TSharedPtr<IHttpRequest> HttpRequest, TSharedPtr<IHttpResponse> HttpResponse, bool bArg)
{
	if (Requests.Contains(HttpRequest))
	{
		auto Cmd = Requests.FindAndRemoveChecked(HttpRequest);
		if (HttpResponse.IsValid())
		{
			Cmd->Data.StatusCode = HttpResponse.Get()->GetResponseCode();
			Cmd->Data.OutputJson = HttpResponse.Get()->GetContentAsString();
			UE_LOG(LogTemp, Warning, TEXT("Response: %s"), *HttpResponse.Get()->GetContentAsString());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Response: error"));
		}
		if (Cmd && Cmd != nullptr)
		{
			Cmd->OnRequestFinish.Execute();
			UE_LOG(LogTemp, Warning, TEXT("OnRequestFinish"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("CMD is null"));
		}
	}
}

void FHttpBackendModule::SendGet(CommandGet& Command)
{
	const TSharedRef<IHttpRequest> HttpRequest = MakeBaseRequest();
	HttpRequest->SetVerb(HttpVerb::NAME_Get);
	HttpRequest->SetURL(Command.GetRequestCompleteURL());
	Requests.Add(HttpRequest, &Command);


	HttpRequest->OnProcessRequestComplete().BindRaw(this, &FHttpBackendModule::Response);

	HttpRequest->ProcessRequest();
}

void FHttpBackendModule::SendPost(CommandPost& Command, const FCommandCallback& Callback) const
{
	const TSharedRef<IHttpRequest> HttpRequest = MakeBaseRequest();
	HttpRequest->SetVerb(HttpVerb::NAME_Post);
	HttpRequest->SetURL(Command.GetRequestCompleteURL());
	if (!Command.InputJson.IsEmpty()) HttpRequest->SetContentAsString(Command.InputJson);

	HttpRequest->OnProcessRequestComplete().BindLambda([&, Callback](FHttpRequestPtr Request, FHttpResponsePtr Response, bool Success)
	{
		if (Response.IsValid())
		{
			Command.Data.StatusCode = Response->GetResponseCode();
			Command.SetOuput(Response.Get()->GetContentAsString());
		}
		Command.OnRequestFinish.ExecuteIfBound();
	});

	HttpRequest->ProcessRequest();
}

void FHttpBackendModule::SendRequestWebApi()
{
	// Using WebApi
	//auto Request = IWebAPIModuleInterface::CreateRequest<FString>(TEXT("https://localhost:7168/backend"));
}

TSharedRef<IHttpRequest> FHttpBackendModule::MakeBaseRequest() const
{
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();

	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json; charset=utf-8"));
	if (!Setting.Token.IsEmpty()) HttpRequest->SetHeader(TEXT("Auth"), Setting.Token);
	return HttpRequest;
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FHttpBackendModule, HTTPBackend)
