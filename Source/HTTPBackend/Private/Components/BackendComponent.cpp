// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/BackendComponent.h"

#include "BackendData.h"
#include "HttpBackendSubsystem.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"


// Sets default values for this component's properties
UBackendComponent::UBackendComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UBackendComponent::BeginPlay()
{
	Super::BeginPlay();
	BackendWrapper = GEngine->GetEngineSubsystem<UHttpBackendSubsystem>()->GetBackend(CompleteURL, Headers);
}

void UBackendComponent::AddHeader(const FString& Key, FString Value)
{
	BackendWrapper->AddHeader(Key, Value);
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

void UBackendComponent::Get(const FString& Path, const TArray<FString>& Parameters,
                            const FOnHttpRequestComplete& Callback)
{
	BackendWrapper->Get(Path, Parameters, Callback);
}

void UBackendComponent::Post(const FString& Path, const TArray<FString>& Parameters, const FString& Input,
                             const FOnHttpRequestComplete& Callback)
{
	BackendWrapper->Post(Path, Parameters, Input, Callback);
}

void UBackendComponent::PostJson(const FString& Path, const TArray<FString>& Parameters,
                                 const TMap<FString, FString>& InputJson, const FOnHttpRequestComplete& Callback)
{
	BackendWrapper->PostJson(Path, Parameters, InputJson, Callback);
}
