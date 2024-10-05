// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/IHttpRequest.h"
#include "BackendComponent.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnHttpRequestComplete, const FBackendResponse&, ResponseContent);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HTTPBACKEND_API UBackendComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBackendComponent();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Timeout = 300;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TMap<FString, FString> Headers;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString Url;

	TSharedRef<IHttpRequest> MakeRequest(const FString& Verbose);

public:
	UFUNCTION(BlueprintCallable)
	void AddHeader(const FString& Key, FString Value);
	UFUNCTION(BlueprintCallable)
	void AddAuthorization(const FString& Auth);
	UFUNCTION(BlueprintCallable)
	void AddToken(const FString& Token);

	UFUNCTION(BlueprintCallable)
	void Get(const FString& Path, const TArray<FString>& Parameters, const FOnHttpRequestComplete& Callback);
	UFUNCTION(BlueprintCallable)
	void Post(const FString& Path, const TArray<FString>& Parameters, const FString& Input, const FOnHttpRequestComplete& Callback);
};
