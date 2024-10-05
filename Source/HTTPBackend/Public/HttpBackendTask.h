// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTask.h"
#include "Interfaces/IHttpRequest.h"
#include "BackendData.h"
#include "HttpBackendBPLibrary.h"
#include "HttpBackendTask.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBackendEvent, const FBackendResponse&, Result);

/**
 * 
 */
UCLASS()
class HTTPBACKEND_API UHttpBackendTask : public UGameplayTask
{
	GENERATED_BODY()

public:
	UHttpBackendTask(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintAssignable)
	FBackendEvent OnComplete;

	virtual void Activate() override;
	void OnResponse(FBackendResponse& Response);

	/** Return debug string describing task */
	virtual FString GetDebugString() const override;

	/** Wait specified time. This is functionally the same as a standard Delay node. */
	UFUNCTION(BlueprintCallable, Category = "Backend", meta = (AdvancedDisplay = "TaskOwner, Priority", DefaultToSelf = "TaskOwner", BlueprintInternalUseOnly = "TRUE"))
	static UHttpBackendTask* TaskSendRequest(TScriptInterface<IGameplayTaskOwnerInterface> TaskOwner, const FCommandData& Data, const uint8 Priority = 127);

	UPROPERTY(BlueprintReadOnly)
	double ExecutionDuration;

private:
	void OnRequestFinish(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Success);
	void OnRequestFinishTask();

	CommandGet* Command;

	//FBackendSettings Settings;
	FCommandData Data;

	double Time;
	double TimeStarted;
};
