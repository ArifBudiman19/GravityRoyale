// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "GameLiftClientTypes.h"
#if WITH_GAMELIFTCLIENTSDK
#include "aws/gamelift/GameLiftClient.h"
#endif

#include "GameLiftClientGameSessionQueue.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCreateGameSessionQueueAsync, const FGameLiftGameSessionQueue&, GameSessionQueue, const FString&, ErrorMessage);
UCLASS()
class GAMELIFTCLIENTSDK_API UGameLiftCreateGameSessionQueueAsync : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

	friend class UGameLiftClientObject;

public:
	UPROPERTY(BlueprintAssignable)
	FOnCreateGameSessionQueueAsync Success;

	UPROPERTY(BlueprintAssignable)
	FOnCreateGameSessionQueueAsync Failed;

private:
	Aws::GameLift::GameLiftClient* GameLiftClient;
	FString Name;
	TArray<FGameLiftDestination> Destinations;
	TArray<FGameLiftPlayerLatencyPolicy> PlayerLatencyPolicies;
	int TimeoutInSeconds;

public:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "Game Lift Async|Game Session Queue")
	static UGameLiftCreateGameSessionQueueAsync* CreateGameSessionQueue(const UObject* WorldContextObject, const UGameLiftClientObject* GameLiftClientObject, const FString Name, const TArray<FGameLiftDestination> Destinations, const TArray<FGameLiftPlayerLatencyPolicy> PlayerLatencyPolicies, int TimeoutInSeconds);

	virtual void Activate() override;

private:
	const UObject* WorldContextObject;

	void OnCreateGameSessionQueueState(const Aws::GameLift::GameLiftClient* Client, const Aws::GameLift::Model::CreateGameSessionQueueRequest& Request, const Aws::GameLift::Model::CreateGameSessionQueueOutcome& Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context);
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDescribeGameSessionQueuesAsync, const TArray<FGameLiftGameSessionQueue>&, GameSessionQueue, const FString&, NextToken, const FString&, ErrorMessage);
UCLASS()
class GAMELIFTCLIENTSDK_API UGameLiftDescribeGameSessionQueuesAsync : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

		friend class UGameLiftClientObject;

public:
	UPROPERTY(BlueprintAssignable)
		FOnDescribeGameSessionQueuesAsync Success;

	UPROPERTY(BlueprintAssignable)
		FOnDescribeGameSessionQueuesAsync Failed;

private:
	Aws::GameLift::GameLiftClient* GameLiftClient;
	TArray<FString> Names;
	int Limit;
	FString NextToken;

public:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "Game Lift Async|Game Session Queue")
		static UGameLiftDescribeGameSessionQueuesAsync* DescribeGameSessionQueues(const UObject* WorldContextObject, const UGameLiftClientObject* GameLiftClientObject, const TArray<FString> Names, const int Limit, const FString NextToken);

	virtual void Activate() override;

private:
	const UObject* WorldContextObject;

	void OnDescribeGameSessionQueuesState(const Aws::GameLift::GameLiftClient* Client, const Aws::GameLift::Model::DescribeGameSessionQueuesRequest& Request, const Aws::GameLift::Model::DescribeGameSessionQueuesOutcome& Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context);
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpdateGameSessionQueueAsync, const FGameLiftGameSessionQueue&, GameSessionQueue, const FString&, ErrorMessage);
UCLASS()
class GAMELIFTCLIENTSDK_API UGameLiftUpdateGameSessionQueueAsync : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

		friend class UGameLiftClientObject;

public:
	UPROPERTY(BlueprintAssignable)
		FOnUpdateGameSessionQueueAsync Success;

	UPROPERTY(BlueprintAssignable)
		FOnUpdateGameSessionQueueAsync Failed;

private:
	Aws::GameLift::GameLiftClient* GameLiftClient;
	FString Name;
	TArray<FGameLiftDestination> Destinations;
	TArray<FGameLiftPlayerLatencyPolicy> PlayerLatencyPolicies;
	int TimeoutInSeconds;

public:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "Game Lift Async|Game Session Queue")
		static UGameLiftUpdateGameSessionQueueAsync* UpdateGameSessionQueue(const UObject* WorldContextObject, const UGameLiftClientObject* GameLiftClientObject, const FString Name, const TArray<FGameLiftDestination> Destinations, const TArray<FGameLiftPlayerLatencyPolicy> PlayerLatencyPolicies, int TimeoutInSeconds);

	virtual void Activate() override;

private:
	const UObject* WorldContextObject;

	void OnUpdateGameSessionQueueState(const Aws::GameLift::GameLiftClient* Client, const Aws::GameLift::Model::UpdateGameSessionQueueRequest& Request, const Aws::GameLift::Model::UpdateGameSessionQueueOutcome& Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context);
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeleteGameSessionQueueAsync, const FString&, ErrorMessage);
UCLASS()
class GAMELIFTCLIENTSDK_API UGameLiftDeleteGameSessionQueueAsync : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

		friend class UGameLiftClientObject;

public:
	UPROPERTY(BlueprintAssignable)
		FOnDeleteGameSessionQueueAsync Success;

	UPROPERTY(BlueprintAssignable)
		FOnDeleteGameSessionQueueAsync Failed;

private:
	Aws::GameLift::GameLiftClient* GameLiftClient;
	FString Name;

public:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "Game Lift Async|Game Session Queue")
		static UGameLiftDeleteGameSessionQueueAsync* DeleteGameSessionQueue(const UObject* WorldContextObject, const UGameLiftClientObject* GameLiftClientObject, const FString Name);

	virtual void Activate() override;

private:
	const UObject* WorldContextObject;

	void OnDeleteGameSessionQueueState(const Aws::GameLift::GameLiftClient* Client, const Aws::GameLift::Model::DeleteGameSessionQueueRequest& Request, const Aws::GameLift::Model::DeleteGameSessionQueueOutcome& Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context);
};