// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/BackendData.h"

#include "Interfaces/IHttpRequest.h"

void FCommandData::ApplyToRequest(const FString& BaseURl, TSharedRef<IHttpRequest> Request) const
{
	FString CompleteUrl = BaseURl + "/" + Path;

	for (auto Param : Parameters)
	{
		CompleteUrl += "/" + Param;
	}

	bool IsFirst = true;
	for (auto Element : Queries)
	{
		if (IsFirst)
		{
			CompleteUrl += "?";
			IsFirst = false;
		}
		else
		{
			CompleteUrl += "&";
		}
		CompleteUrl += Element.Key + "=" + Element.Value;
	}
	Request->SetURL(CompleteUrl);

	for (auto Element : Headers)
	{
		Request->SetHeader(Element.Key, Element.Value);
	}

	if (JsonContent.Len() > 0)
	{
		Request->SetHeader("Content-Type", "application/json");
		Request->SetContentAsString(JsonContent);
	}
}
