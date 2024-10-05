// Copyright Epic Games, Inc. All Rights Reserved.

#include "HttpBackendBPLibrary.h"
#include "HttpBackend.h"
#include "Commands/Command.h"

UHttpBackendBPLibrary::UHttpBackendBPLibrary(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}


float UHttpBackendBPLibrary::HTTPBackendSampleFunction(float Param)
{
	return -1;
}


void UHttpBackendBPLibrary::SetAuth(FString Token)
{
	if (FModuleManager::Get().IsModuleLoaded("HTTPBackend"))
	{
		FHttpBackendModule& Backend = FModuleManager::Get().GetModuleChecked<FHttpBackendModule>("HTTPBackend");
	}
}

int UHttpBackendBPLibrary::GetIntCode()
{
	return GEngine->GetEngineSubsystem<UHttpBackendSubsystem>()->GetCode();
}

void UHttpBackendBPLibrary::SetIntCode(int a)
{
	GEngine->GetEngineSubsystem<UHttpBackendSubsystem>()->SetCode(a);
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
