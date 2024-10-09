// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Subsystems/Subsystem.h"
#include "HttpBackendSubsystem.generated.h"

/**
 *  TODO
 */
UCLASS()
class HTTPBACKEND_API UHttpBackendSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()
	int Value = 8;

public:
	int GetCode() const;
	void SetCode(int a);

	//UFUNCTION()
	//void Get(const FString& Url, const TArray<FString>& Parameters, TFunction<void(FHttpRequestPtr, FHttpResponsePtr, bool)> Callback);
};
