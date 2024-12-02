// Fill out your copyright notice in the Description page of Project Settings.


#include "HttpBackendSubsystem.h"
#include "Components/BackendWrapper.h"

UBackendWrapper* UHttpBackendSubsystem::GetBackend(const FString& InUrl, const TMap<FString, FString>& InHeaders)
{
	auto BackendWrapper = NewObject<UBackendWrapper>();
	BackendWrapper->Initialise(InUrl, InHeaders);
	return BackendWrapper;
}
