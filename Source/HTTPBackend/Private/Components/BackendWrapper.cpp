// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/BackendWrapper.h"

#include "BackendData.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

// Sets default values for this component's properties
UBackendWrapper::UBackendWrapper()
{
}

TSharedRef<IHttpRequest> UBackendWrapper::MakeRequest(const FString& Verbose)
{
	const TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();

	Request->SetVerb(Verbose);
	Request->SetTimeout(Timeout);

	if (!Headers.IsEmpty())
	{
		for (auto Header : Headers)
		{
			Request->SetHeader(Header.Key, Header.Value);
		}
	}

	return Request;
}

void UBackendWrapper::Initialise(const FString& InUrl, const TMap<FString, FString>& InHeaders)
{
	Headers = InHeaders;
	Url = InUrl;
}

void UBackendWrapper::AddHeader(const FString& Key, FString Value)
{
	if (Headers.Contains(Key))
	{
		Headers[Key] = Value;
	}
	else
	{
		Headers.Add(Key, Value);
	}
}

void UBackendWrapper::AddAuthorization(const FString& Auth)
{
	AddHeader("Authorization", Auth);
}

void UBackendWrapper::AddBearerToken(const FString& Token)
{
	AddHeader("Authorization", "Bearer " + Token);
}

void UBackendWrapper::AddToken(const FString& Token)
{
	AddHeader("Token", Token);
}

void UBackendWrapper::Get(const FString& Path, const TArray<FString>& Parameters,
                          const FOnHttpRequestComplete& Callback)
{
	const TSharedRef<IHttpRequest> Request = MakeRequest(HttpVerb::NAME_Get); //FHttpModule::Get().CreateRequest();

	FString CompleteUrl = Url + "/" + Path;

	for (auto Param : Parameters)
	{
		CompleteUrl += "/" + Param;
	}

	// Set the URL for the request
	Request->SetURL(CompleteUrl);

	// Bind the response callback
	Request->OnProcessRequestComplete().BindLambda(
		[Callback](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			FBackendResponse ResponseData;
			if (bWasSuccessful && Response.IsValid())
			{
				ResponseData.Success = true;
				ResponseData.StatusCode = Response.Get()->GetResponseCode();
				ResponseData.ContentType = Response.Get()->GetContentType();
				ResponseData.Content = Response.Get()->GetContentAsString();
				UE_LOG(LogTemp, Warning, TEXT("Response: %s"), *Response.Get()->GetContentAsString());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Response: error"));
			}
			Callback.Execute(ResponseData);
		});

	// Send the request
	Request->ProcessRequest();
}

void UBackendWrapper::SendCommandBP(const FCommandData Data, const FOnHttpRequestComplete& Callback)
{
	const TSharedRef<IHttpRequest> Request = MakeRequest(HttpVerb::Get(Data.Verbose));

	Data.ApplyToRequest(Url, Request);

	// Bind the response callback
	Request->OnProcessRequestComplete().BindLambda(
		[Callback](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			FBackendResponse ResponseData;
			if (bWasSuccessful && Response.IsValid())
			{
				ResponseData.Success = true;
				ResponseData.StatusCode = Response.Get()->GetResponseCode();
				ResponseData.ContentType = Response.Get()->GetContentType();
				ResponseData.Content = Response.Get()->GetContentAsString();
				UE_LOG(LogTemp, Warning, TEXT("Response: %s"), *Response.Get()->GetContentAsString());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Response: error"));
			}
			Callback.Execute(ResponseData);
		});

	// Send the request
	Request->ProcessRequest();
}

void UBackendWrapper::SendCommand(const FCommandData& Data, FOnHttpRequestCompleteRaw Callback)
{
	const TSharedRef<IHttpRequest> Request = MakeRequest(HttpVerb::Get(Data.Verbose));

	Data.ApplyToRequest(Url, Request);

	// Bind the response callback
	Request->OnProcessRequestComplete().BindLambda(
		[Callback](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			FBackendResponse ResponseData;
			if (bWasSuccessful && Response.IsValid())
			{
				ResponseData.Success = true;
				ResponseData.StatusCode = Response.Get()->GetResponseCode();
				ResponseData.ContentType = Response.Get()->GetContentType();
				ResponseData.Content = Response.Get()->GetContentAsString();
				UE_LOG(LogTemp, Warning, TEXT("Response: %s"), *Response.Get()->GetContentAsString());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Response: error"));
			}
			UE_LOG(LogTemp, Warning, TEXT("SendCommand %d status:%d"), ResponseData.Success,ResponseData.StatusCode);
			Callback.Execute(ResponseData);
		});

	// Send the request
	Request->ProcessRequest();
}

void UBackendWrapper::Post(const FString& Path, const TArray<FString>& Parameters, const FString& Input,
                           const FOnHttpRequestComplete& Callback)
{
	const TSharedRef<IHttpRequest> Request = MakeRequest(HttpVerb::NAME_Post);

	FString CompleteUrl = Url + "/" + Path;

	for (auto Param : Parameters)
	{
		CompleteUrl += "/" + Param;
	}

	Request->SetHeader("Content-Type", "application/json");
	Request->SetContentAsString(Input);

	Request->SetURL(CompleteUrl);

	// Bind the response callback
	Request->OnProcessRequestComplete().BindLambda(
		[Callback](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			FBackendResponse ResponseData;
			if (bWasSuccessful && Response.IsValid())
			{
				ResponseData.Success = true;
				ResponseData.StatusCode = Response.Get()->GetResponseCode();
				ResponseData.ContentType = Response.Get()->GetContentType();
				ResponseData.Content = Response.Get()->GetContentAsString();
				UE_LOG(LogTemp, Warning, TEXT("Response: %s"), *Response.Get()->GetContentAsString());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Response: error"));
			}
			Callback.Execute(ResponseData);
		});

	// Send the request
	Request->ProcessRequest();
}

void UBackendWrapper::PostJson(const FString& Path, const TArray<FString>& Parameters,
                               const TMap<FString, FString>& InputJson, const FOnHttpRequestComplete& Callback)
{
	const TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	for (auto Element : InputJson)
	{
		JsonObject->SetStringField(Element.Key, Element.Value);
	}

	FString OutputString;
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	Post(Path, Parameters, OutputString, Callback);
}
