// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "GameLiftClientTypes.h"
#if WITH_GAMELIFTCLIENTSDK
#include "aws/gamelift/GameLiftClient.h"
#endif

#include "GameLiftClientSession.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStartGameSessionPlacementAsync, const FGameLiftGameSessionPlacement&, GameSessionPlacement, const FString&, ErrorMessage);
UCLASS()
class GAMELIFTCLIENTSDK_API UGameLiftStartGameSessionPlacementAsync : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

		friend class UGameLiftClientObject;

public:
	UPROPERTY(BlueprintAssignable)
		FOnStartGameSessionPlacementAsync Success;

	UPROPERTY(BlueprintAssignable)
		FOnStartGameSessionPlacementAsync Failed;

private:
	Aws::GameLift::GameLiftClient* GameLiftClient;
	TArray<FGameLiftDesiredPlayerSession> DesiredPlayerSessions;
	TArray<FGameLiftMap> GameProperties;
	FString GameSessionData;
	FString GameSessionName;
	FString GameSessionQueueName;
	int MaximumPlayerSessionCount;
	FString PlacementId;
	TArray<FGameLiftPlayerLatency> PlayerLatencies;

public:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "Game Lift Async|Game Session Placement")
		static UGameLiftStartGameSessionPlacementAsync* StartGameSessionPlacement(const UObject* WorldContextObject, const UGameLiftClientObject* GameLiftClientObject, FString GameSessionQueueName, int MaximumPlayerSessionCount, FString PlacementId, TArray<FGameLiftDesiredPlayerSession> DesiredPlayerSessions, TArray<FGameLiftMap> GameProperties, FString GameSessionData, FString GameSessionName, TArray<FGameLiftPlayerLatency> PlayerLatencies);

	virtual void Activate() override;

private:
	const UObject* WorldContextObject;

	void OnStartGameSessionPlacementState(const Aws::GameLift::GameLiftClient* Client, const Aws::GameLift::Model::StartGameSessionPlacementRequest& Request, const Aws::GameLift::Model::StartGameSessionPlacementOutcome& Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context);
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDescribeGameSessionPlacementAsync, const FGameLiftGameSessionPlacement&, GameSessionPlacement, const FString&, ErrorMessage);
UCLASS()
class GAMELIFTCLIENTSDK_API UGameLiftDescribeGameSessionPlacementAsync : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

		friend class UGameLiftClientObject;

public:
	UPROPERTY(BlueprintAssignable)
		FOnDescribeGameSessionPlacementAsync Success;

	UPROPERTY(BlueprintAssignable)
		FOnDescribeGameSessionPlacementAsync Failed;

private:
	Aws::GameLift::GameLiftClient* GameLiftClient;
	FString PlacementId;

public:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "Game Lift Async|Game Session Placement")
		static UGameLiftDescribeGameSessionPlacementAsync* DescribeGameSessionPlacement(const UObject* WorldContextObject, const UGameLiftClientObject* GameLiftClientObject, FString PlacementId);

	virtual void Activate() override;

private:
	const UObject* WorldContextObject;

	void OnDescribeGameSessionPlacementState(const Aws::GameLift::GameLiftClient* Client, const Aws::GameLift::Model::DescribeGameSessionPlacementRequest& Request, const Aws::GameLift::Model::DescribeGameSessionPlacementOutcome& Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context);
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStopGameSessionPlacementAsync, const FGameLiftGameSessionPlacement&, GameSessionPlacement, const FString&, ErrorMessage);
UCLASS()
class GAMELIFTCLIENTSDK_API UGameLiftStopGameSessionPlacementAsync : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

		friend class UGameLiftClientObject;

public:
	UPROPERTY(BlueprintAssignable)
		FOnStopGameSessionPlacementAsync Success;

	UPROPERTY(BlueprintAssignable)
		FOnStopGameSessionPlacementAsync Failed;

private:
	Aws::GameLift::GameLiftClient* GameLiftClient;
	FString PlacementId;

public:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "Game Lift Async|Game Session Placement")
		static UGameLiftStopGameSessionPlacementAsync* StopGameSessionPlacement(const UObject* WorldContextObject, const UGameLiftClientObject* GameLiftClientObject, FString PlacementId);

	virtual void Activate() override;

private:
	const UObject* WorldContextObject;

	void OnStopGameSessionPlacementState(const Aws::GameLift::GameLiftClient* Client, const Aws::GameLift::Model::StopGameSessionPlacementRequest& Request, const Aws::GameLift::Model::StopGameSessionPlacementOutcome& Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context);
};