// Fill out your copyright notice in the Description page of Project Settings.


#include "HttpBackendSubsystem.h"

#include "HttpModule.h"

int UHttpBackendSubsystem::GetCode() const
{
	return Value;
}

void UHttpBackendSubsystem::SetCode(int a)
{
	Value = a;
}

// void UHttpBackendSubsystem::Get(const FString& Url, const TArray<FString>& Parameters, TFunction<void(FHttpRequestPtr, FHttpResponsePtr, bool)> Callback)
// {
// 	const TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
//
// 	FString CompleteUrl = Url;
//
// 	if (Parameters.Num() > 0)
// 	{
// 		for (auto Param : Parameters)
// 		{
// 			CompleteUrl += "/" + Param;
// 		}
// 	}
//
// 	// Set the URL for the request
// 	Request->SetURL(CompleteUrl);
//
// 	// Set the verb (GET, POST, PUT, DELETE, etc.)
// 	Request->SetVerb(HttpVerb::NAME_Get);
//
// 	// Set headers
// 	Request->SetHeader("Content-Type", "application/json");
//
// 	// Bind the response callback
// 	Request->OnProcessRequestComplete().BindLambda(Callback);
//
// 	// Send the request
// 	Request->ProcessRequest();
// }
