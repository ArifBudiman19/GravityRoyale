// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "GameLiftClientTypes.h"
#if WITH_GAMELIFTCLIENTSDK
#include "aws/gamelift/GameLiftClient.h"
#endif


#include "GameLiftClientPlayerSession.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCreatePlayerSessionAsync, const FGameLiftPlayerSession&, PlayerSession, const FString&, ErrorMessage);
UCLASS()
class GAMELIFTCLIENTSDK_API UGameLiftCreatePlayerSessionAsync : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

		friend class UGameLiftClientObject;

public:
	UPROPERTY(BlueprintAssignable)
		FOnCreatePlayerSessionAsync Success;

	UPROPERTY(BlueprintAssignable)
		FOnCreatePlayerSessionAsync Failed;

private:
	Aws::GameLift::GameLiftClient* GameLiftClient;
	FString GameSessionId;
	FString PlayerData;
	FString PlayerId;

public:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "Game Lift Async|Player Session")
		static UGameLiftCreatePlayerSessionAsync* CreatePlayerSession(const UObject* WorldContextObject, const UGameLiftClientObject* GameLiftClientObject, const FString GameSessionId, const FString PlayerData, const FString PlayerId );

	virtual void Activate() override;

private:
	const UObject* WorldContextObject;

	void OnCreatePlayerSessionState(const Aws::GameLift::GameLiftClient* Client, const Aws::GameLift::Model::CreatePlayerSessionRequest& Request, const Aws::GameLift::Model::CreatePlayerSessionOutcome& Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context);
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCreatePlayerSessionsAsync, const TArray<FGameLiftPlayerSession>&, PlayerSessions, const FString&, ErrorMessage);
UCLASS()
class GAMELIFTCLIENTSDK_API UGameLiftCreatePlayerSessionsAsync : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

		friend class UGameLiftClientObject;

public:
	UPROPERTY(BlueprintAssignable)
		FOnCreatePlayerSessionsAsync Success;

	UPROPERTY(BlueprintAssignable)
		FOnCreatePlayerSessionsAsync Failed;

private:
	Aws::GameLift::GameLiftClient* GameLiftClient;
	FString GameSessionId;
	TMap<FString, FString> PlayerDataMap;
	TArray<FString> PlayerIds;

public:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "Game Lift Async|Player Session")
		static UGameLiftCreatePlayerSessionsAsync* CreatePlayerSessions(const UObject* WorldContextObject, const UGameLiftClientObject* GameLiftClientObject, const FString GameSessionId, const TMap<FString, FString> PlayerDataMap, const TArray<FString> PlayerIds);

	virtual void Activate() override;

private:
	const UObject* WorldContextObject;

	void OnCreatePlayerSessionsState(const Aws::GameLift::GameLiftClient* Client, const Aws::GameLift::Model::CreatePlayerSessionsRequest& Request, const Aws::GameLift::Model::CreatePlayerSessionsOutcome& Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context);
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDescribePlayerSessionsAsync, const TArray<FGameLiftPlayerSession>&, PlayerSessions, const FString&, NextToken, const FString&, ErrorMessage);
UCLASS()
class GAMELIFTCLIENTSDK_API UGameLiftDescribePlayerSessionsAsync : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

		friend class UGameLiftClientObject;

public:
	UPROPERTY(BlueprintAssignable)
		FOnDescribePlayerSessionsAsync Success;

	UPROPERTY(BlueprintAssignable)
		FOnDescribePlayerSessionsAsync Failed;

private:
	Aws::GameLift::GameLiftClient* GameLiftClient;
	FString GameSessionId;
	int Limit;
	FString NextToken;
	FString PlayerId;
	FString PlayerSessionId;
	FString PlayerSessionStatusFillter;

public:
	/**
	* @Param PlayerSessionStatusFillter Player session status to filter results on. Possible player session statuses include RESERVED,
	* ACTIVE, COMPLETED, TIMEDOUT.
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "Game Lift Async|Player Session")
	static UGameLiftDescribePlayerSessionsAsync* DescribePlayerSessions(const UObject* WorldContextObject, const UGameLiftClientObject* GameLiftClientObject, const FString GameSessionId, const int Limit, const FString NextToken, const FString PlayerId, const FString PlayerSessionId, const FString PlayerSessionStatusFillter);

	virtual void Activate() override;

private:
	const UObject* WorldContextObject;

	void OnDescribePlayerSessionsState(const Aws::GameLift::GameLiftClient* Client, const Aws::GameLift::Model::DescribePlayerSessionsRequest& Request, const Aws::GameLift::Model::DescribePlayerSessionsOutcome& Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context);
};