// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "BackendWrapper.generated.h"

struct FBackendResponse;
class IHttpRequest;

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnHttpRequestComplete, const FBackendResponse&, ResponseContent);

DECLARE_DELEGATE_OneParam(FOnHttpRequestCompleteRaw, const FBackendResponse&);

/**
 * 
 */
UCLASS()
class HTTPBACKEND_API UBackendWrapper : public UObject
{
	GENERATED_BODY()

	UBackendWrapper();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Timeout = 300;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TMap<FString, FString> Headers;


	TSharedRef<IHttpRequest> MakeRequest(const FString& Verbose);

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString Url;
	void Initialise(const FString& Url, const TMap<FString, FString>& Headers = TMap<FString, FString>());
	UFUNCTION(BlueprintCallable)
	void AddHeader(const FString& Key, FString Value);
	UFUNCTION(BlueprintCallable)
	void AddAuthorization(const FString& Auth);
	UFUNCTION(BlueprintCallable)
	void AddBearerToken(const FString& Token);
	UFUNCTION(BlueprintCallable)
	void AddToken(const FString& Token);

	UFUNCTION(BlueprintCallable)
	void Get(const FString& Path, const TArray<FString>& Parameters, const FOnHttpRequestComplete& Callback);
	UFUNCTION(BlueprintCallable)
	void SendCommandBP(const FCommandData Data, const FOnHttpRequestComplete& Callback);
	void SendCommand(const FCommandData& Data, FOnHttpRequestCompleteRaw Callback);
	UFUNCTION(BlueprintCallable)
	void Post(const FString& Path, const TArray<FString>& Parameters, const FString& Input,
	          const FOnHttpRequestComplete& Callback);
	UFUNCTION(BlueprintCallable)
	void PostJson(const FString& Path, const TArray<FString>& Parameters, const TMap<FString, FString>& InputJson,
	              const FOnHttpRequestComplete& Callback);
};
