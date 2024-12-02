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

public:
	UFUNCTION(BlueprintCallable)
	UBackendWrapper* GetBackend(const FString& InUrl, const TMap<FString, FString>& InHeaders);
};
