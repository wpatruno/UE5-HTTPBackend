// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/BackendComponent.h"

#include "BackendData.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"


// Sets default values for this component's properties
UBackendComponent::UBackendComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

TSharedRef<IHttpRequest> UBackendComponent::MakeRequest(const FString& Verbose)
{
	const TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();

	Request->SetVerb(Verbose);
	Request->SetTimeout(Timeout);
	for (auto Header : Headers)
	{
		Request->SetHeader(Header.Key, Header.Value);
	}

	return Request;
}

void UBackendComponent::AddHeader(const FString& Key, FString Value)
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

void UBackendComponent::AddAuthorization(const FString& Auth)
{
	AddHeader("Authorization", Auth);
}

void UBackendComponent::AddBearerToken(const FString& Token)
{
	AddHeader("Authorization", "Bearer " + Token);
}

void UBackendComponent::AddToken(const FString& Token)
{
	AddHeader("Token", Token);
}

void UBackendComponent::Get(const FString& Path, const TArray<FString>& Parameters, const FOnHttpRequestComplete& Callback)
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

void UBackendComponent::Post(const FString& Path, const TArray<FString>& Parameters, const FString& Input, const FOnHttpRequestComplete& Callback)
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

void UBackendComponent::PostJson(const FString& Path, const TArray<FString>& Parameters, const TMap<FString, FString>& InputJson, const FOnHttpRequestComplete& Callback)
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
