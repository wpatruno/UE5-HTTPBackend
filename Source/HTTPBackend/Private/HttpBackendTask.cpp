// Fill out your copyright notice in the Description page of Project Settings.
#include "HttpBackendTask.h"

#include "HttpBackend.h"
#include "Interfaces/IHttpResponse.h"
#include "HttpBackendBPLibrary.h"
#include "HttpModule.h"

UHttpBackendTask::UHttpBackendTask(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Data = FCommandData();
	Time = 0.f;
	TimeStarted = 0.f;
	ExecutionDuration = 0.f;
	Command = nullptr;
}

UHttpBackendTask* UHttpBackendTask::TaskSendRequest(TScriptInterface<IGameplayTaskOwnerInterface> TaskOwner, const FCommandData& Data, const uint8 Priority)
{
	UHttpBackendTask* MyTask = NewTaskUninitialized<UHttpBackendTask>();
	if (MyTask && TaskOwner.GetInterface() != nullptr)
	{
		MyTask->InitTask(*TaskOwner, Priority);
		MyTask->Data = Data;
	}
	return MyTask;
}

void UHttpBackendTask::Activate()
{
	if (GetWorld()) TimeStarted = GetWorld()->GetTimeSeconds();

	UHttpBackendSubsystem* MySubsystem = GEngine->GetEngineSubsystem<UHttpBackendSubsystem>();
	UE_LOG(LogTemp, Warning, TEXT("Code: %d"), MySubsystem->GetCode());

	//MySubsystem->Get(Data, TArray<FString>(), &UHttpBackendTask::OnResponse);
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();

	// Set the URL for the request
	Request->SetURL(Data.Path);

	// Set the verb (GET, POST, PUT, DELETE, etc.)
	Request->SetVerb("GET");

	// Set headers
	Request->SetHeader("Content-Type", "application/json");

	// Bind the response callback
	Request->OnProcessRequestComplete().BindUObject(this, &UHttpBackendTask::OnRequestFinish);

	// Send the request
	Request->ProcessRequest();

	UE_LOG(LogTemp, Warning, TEXT("Module HTTPBackend is loaded ! %d"), 10);
}

void UHttpBackendTask::OnResponse(FBackendResponse& Response)
{
}

void UHttpBackendTask::OnRequestFinishTask()
{
	UE_LOG(LogTemp, Warning, TEXT("Output: %s"), *Command->Data.OutputJson);
	FBackendResponse Response;
	Response.StatusCode = Command->Data.StatusCode;
	Response.Content = Command->Data.OutputJson;

	if (GetWorld()) ExecutionDuration = GetWorld()->GetTimeSeconds() - TimeStarted;

	OnComplete.Broadcast(Response);
	EndTask();
}

void UHttpBackendTask::OnRequestFinish(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Success)
{
	FBackendResponse ResponseData;
	if (Success && Response.IsValid())
	{
		ResponseData.Success = true;
		ResponseData.StatusCode = Response.Get()->GetResponseCode();
		ResponseData.ContentType = Response.Get()->GetContentType();
		ResponseData.Content = Response.Get()->GetContentAsString();
		UE_LOG(LogTemp, Warning, TEXT("Response: %s"), *Response.Get()->GetContentAsString());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Response: error"));
	}
	EndTask();
	OnComplete.Broadcast(ResponseData);
}

FString UHttpBackendTask::GetDebugString() const
{
	const double TimeLeft = Time - GetWorld()->TimeSince(TimeStarted);
	return FString::Printf(TEXT("WaitDelay. Time: %.2f. TimeLeft: %.2f"), Time, TimeLeft);
}
