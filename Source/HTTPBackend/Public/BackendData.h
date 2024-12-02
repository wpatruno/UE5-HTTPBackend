// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BackendData.generated.h"

class IHttpRequest;

UENUM()
enum EHttpMethod : uint8
{
	POST,
	GET,
	PUT,
	PATCH,
	DELETE,
	HEAD,
	OPTIONS,
	TRACE
};

namespace HttpVerb
{
	static const FString NAME_Post = FString(TEXT("POST"));
	static const FString NAME_Get = FString(TEXT("GET"));
	static const FString NAME_Put = FString(TEXT("PUT"));
	static const FString NAME_Patch = FString(TEXT("PATCH"));
	static const FString NAME_Delete = FString(TEXT("DELETE"));
	static const FString NAME_Head = FString(TEXT("HEAD"));
	static const FString NAME_Options = FString(TEXT("OPTIONS"));
	static const FString NAME_Trace = FString(TEXT("TRACE"));

	static FString Get(const EHttpMethod& Method)
	{
		switch (Method)
		{
		case POST: return NAME_Post;
		case GET: return NAME_Get;
		case PUT: return NAME_Put;
		case PATCH: return NAME_Patch;
		case DELETE: return NAME_Delete;
		case HEAD: return NAME_Head;
		case OPTIONS: return NAME_Options;
		case TRACE: return NAME_Trace;
		}
		return FString();
	};
}


USTRUCT(BlueprintType)
struct HTTPBACKEND_API FBackendSettings
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FString BaseURL;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FString Token;
};

USTRUCT(BlueprintType)
struct HTTPBACKEND_API FBackendResponse
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool Success;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 StatusCode;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FString ContentType;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FString Content;
};

USTRUCT(BlueprintType, Blueprintable)
struct FCommandData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EHttpMethod> Verbose = GET;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Path = "";
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> Parameters = TArray<FString>();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, FString> Headers = TMap<FString, FString>();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, FString> Queries = TMap<FString, FString>();
	UPROPERTY(BlueprintReadWrite)
	FString JsonContent = "";

	void ApplyToRequest(const FString& BaseURl, TSharedRef<IHttpRequest> Request) const;
};


USTRUCT(BlueprintType, Blueprintable)
struct FPostCommandData : public FCommandData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString InputJson;
};
