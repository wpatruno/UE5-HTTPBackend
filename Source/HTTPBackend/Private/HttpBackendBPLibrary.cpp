// Copyright Epic Games, Inc. All Rights Reserved.

#include "HttpBackendBPLibrary.h"
#include "HttpBackend.h"

UHttpBackendBPLibrary::UHttpBackendBPLibrary(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}


float UHttpBackendBPLibrary::HTTPBackendSampleFunction(float Param)
{
	return -1;
}


// void UHTTPBackendBPLibrary::SendGet(CommandGet& Command)
// {
// 	if (FModuleManager::Get().IsModuleLoaded("HTTPBackend"))
// 	{
// 		const FHTTPBackendModule& Backend = FModuleManager::Get().GetModuleChecked<FHTTPBackendModule>("HTTPBackend");
// 		Backend.SendGet(Command);
// 	}
// }

//
// void UHTTPBackendBPLibrary::SendPost(FCommandPost& Command, const FCommandCallback& Callback)
// {
// 	if (FModuleManager::Get().IsModuleLoaded("HTTPBackend"))
// 	{
// 		FHTTPBackendModule& Backend = FModuleManager::Get().GetModuleChecked<FHTTPBackendModule>("HTTPBackend");
// 		Backend.SendPost(Command, Callback);
// 	}
// }
