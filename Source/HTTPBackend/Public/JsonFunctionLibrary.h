// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JsonObjectConverter.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "JsonFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class HTTPBACKEND_API UJsonFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, CustomThunk, Category = "JSON", meta = (DefaultToSelf="Outer", CustomStructureParam = "OutStruct"))
	static bool JsonToStructBP(const FString& JsonString, UObject* Outer, UPARAM(ref) UObject*& OutStruct);

	UFUNCTION(BlueprintCallable, CustomThunk, Category = "JSON", meta = ( CustomStructureParam = "StructVal"))
	static FString StructToJsonBP(const UObject* StructVal);

	template <typename T>
	static T* JsonToStruct(const FString& JsonString, UObject* Outer)
	{
		T* ResultStruct = nullptr;

		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

		if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
		{
			ResultStruct = NewObject<T>(Outer);
			if (!FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), T::StaticStruct(), ResultStruct, 0, 0))
			{
				ResultStruct = nullptr;
			}
		}

		return ResultStruct;
	}

	template <typename T>
	static FString StructToJson(const T* StructPtr)
	{
		if (!StructPtr)
		{
			return FString();
		}

		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
		if (FJsonObjectConverter::UStructToJsonObject(T::StaticStruct(), StructPtr, JsonObject.ToSharedRef(), 0, 0))
		{
			FString OutputString;
			TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
			FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
			return OutputString;
		}

		return FString();
	}

	DECLARE_FUNCTION(execJsonToStructBP)
	{
		P_GET_PROPERTY(FStrProperty, JsonString);
		P_GET_OBJECT(UObject, Outer);

		Stack.Step(Stack.Object, nullptr);
		FProperty* Property = Stack.MostRecentProperty;
		void* StructPtr = Stack.MostRecentPropertyAddress;

		P_FINISH;

		bool bSuccess = false;

		if (FStructProperty* StructProperty = CastField<FStructProperty>(Property))
		{
			UScriptStruct* StructType = StructProperty->Struct;

			TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

			if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
			{
				void* NewStructMemory = FMemory::Malloc(StructType->GetStructureSize());
				StructType->InitializeStruct(NewStructMemory);

				if (JsonObjectToUStructWithArrays(JsonObject.ToSharedRef(), StructType, NewStructMemory))
				{
					StructProperty->CopyCompleteValue(StructPtr, NewStructMemory);
					bSuccess = true;
				}

				StructType->DestroyStruct(NewStructMemory);
				FMemory::Free(NewStructMemory);
			}
		}
		else
		{
			FBlueprintExceptionInfo ExceptionInfo(
				EBlueprintExceptionType::AccessViolation,
				FText::FromString(TEXT("Invalid property; expected a struct."))
			);
			FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, ExceptionInfo);
		}

		*static_cast<bool*>(RESULT_PARAM) = bSuccess;
	}

	DECLARE_FUNCTION(execStructToJsonBP)
	{
		Stack.Step(Stack.Object, nullptr);
		void* StructPtr = Stack.MostRecentPropertyAddress;
		FProperty* Property = Stack.MostRecentProperty;

		P_FINISH;

		FString JsonString;

		if (FStructProperty* StructProperty = CastField<FStructProperty>(Property))
		{
			UScriptStruct* StructType = StructProperty->Struct;
			TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

			if (FJsonObjectConverter::UStructToJsonObject(StructType, StructPtr, JsonObject.ToSharedRef(), 0, 0))
			{
				TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
				FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
			}
		}
		else
		{
			FBlueprintExceptionInfo ExceptionInfo(
				EBlueprintExceptionType::AccessViolation,
				FText::FromString(TEXT("Invalid property; expected a struct."))
			);
			FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, ExceptionInfo);
		}

		*static_cast<FString*>(RESULT_PARAM) = JsonString;
	}

private:
	static bool JsonObjectToUStructWithArrays(const TSharedRef<FJsonObject>& JsonObject, UScriptStruct* StructType, void* StructPtr)
	{
		for (TFieldIterator<FProperty> It(StructType); It; ++It)
		{
			FProperty* Property = *It;
			FString PropertyName = Property->GetName();

			if (JsonObject->HasField(PropertyName))
			{
				if (FArrayProperty* ArrayProperty = CastField<FArrayProperty>(Property))
				{
					FScriptArrayHelper ArrayHelper(ArrayProperty, ArrayProperty->ContainerPtrToValuePtr<void>(StructPtr));
					const TArray<TSharedPtr<FJsonValue>>* JsonArray;
					if (JsonObject->TryGetArrayField(PropertyName, JsonArray))
					{
						ArrayHelper.EmptyAndAddUninitializedValues(JsonArray->Num());
						for (int32 i = 0; i < JsonArray->Num(); i++)
						{
							const TSharedPtr<FJsonValue>& JsonValue = (*JsonArray)[i];
							void* ValuePtr = ArrayHelper.GetRawPtr(i);

							if (FStructProperty* InnerStructProp = CastField<FStructProperty>(ArrayProperty->Inner))
							{
								const TSharedPtr<FJsonObject>* InnerJsonObject;
								if (JsonValue->TryGetObject(InnerJsonObject))
								{
									JsonObjectToUStructWithArrays(InnerJsonObject->ToSharedRef(), InnerStructProp->Struct, ValuePtr);
								}
							}
							else
							{
								FJsonObjectConverter::JsonValueToUProperty(JsonValue, ArrayProperty->Inner, ValuePtr, 0, 0);
							}
						}
					}
				}
				else if (FStructProperty* StructProperty = CastField<FStructProperty>(Property))
				{
					const TSharedPtr<FJsonObject>* InnerJsonObject;
					if (JsonObject->TryGetObjectField(PropertyName, InnerJsonObject))
					{
						void* InnerStructPtr = StructProperty->ContainerPtrToValuePtr<void>(StructPtr);
						JsonObjectToUStructWithArrays(InnerJsonObject->ToSharedRef(), StructProperty->Struct, InnerStructPtr);
					}
				}
				else
				{
					void* ValuePtr = Property->ContainerPtrToValuePtr<void>(StructPtr);
					FJsonObjectConverter::JsonValueToUProperty(JsonObject->TryGetField(PropertyName), Property, ValuePtr, 0, 0);
				}
			}
		}

		return true;
	}
};
