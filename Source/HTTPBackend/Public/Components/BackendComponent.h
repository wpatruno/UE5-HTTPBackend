// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BackendWrapper.h"
#include "Components/ActorComponent.h"
#include "Interfaces/IHttpRequest.h"
#include "BackendComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HTTPBACKEND_API UBackendComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBackendComponent();

	virtual void BeginPlay() override;

protected:
	UPROPERTY()
	TObjectPtr<UBackendWrapper> BackendWrapper;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Backend")
	FString CompleteURL;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Backend")
	TMap<FString, FString> Headers;

public:
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
	void Post(const FString& Path, const TArray<FString>& Parameters, const FString& Input,
	          const FOnHttpRequestComplete& Callback);
	UFUNCTION(BlueprintCallable)
	void PostJson(const FString& Path, const TArray<FString>& Parameters, const TMap<FString, FString>& InputJson,
	              const FOnHttpRequestComplete& Callback);
};
