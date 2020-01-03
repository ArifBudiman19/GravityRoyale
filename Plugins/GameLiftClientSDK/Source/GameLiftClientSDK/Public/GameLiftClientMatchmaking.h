// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "GameLiftClientTypes.h"
#if WITH_GAMELIFTCLIENTSDK
#include "aws/gamelift/GameLiftClient.h"
#endif

#include "GameLiftClientMatchmaking.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStartMatchmakingAsync, const FGameLiftMatchmakingTicket&, MatchmakingTicket, const FString&, ErrorMessage);
UCLASS()
class GAMELIFTCLIENTSDK_API UGameLiftStartMatchmakingAsync : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

		friend class UGameLiftClientObject;

public:
	UPROPERTY(BlueprintAssignable)
		FOnStartMatchmakingAsync Success;

	UPROPERTY(BlueprintAssignable)
		FOnStartMatchmakingAsync Failed;

private:
	Aws::GameLift::GameLiftClient* GameLiftClient;
	FString ConfigurationName;
	TArray<FGameLiftPlayer> Players;
	FString TicketId;

public:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "Game Lift Async|Matchmaking")
		static UGameLiftStartMatchmakingAsync* StartMatchmaking(const UObject* WorldContextObject, const UGameLiftClientObject* GameLiftClientObject, FString ConfigurationName, TArray<FGameLiftPlayer> Players, FString TicketId);

	virtual void Activate() override;

private:
	const UObject* WorldContextObject;

	void OnStartMatchmakingState(const Aws::GameLift::GameLiftClient* Client, const Aws::GameLift::Model::StartMatchmakingRequest& Request, const Aws::GameLift::Model::StartMatchmakingOutcome& Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context);
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDescribeMatchmakingAsync, const TArray<FGameLiftMatchmakingTicket>&, TicketList, const FString&, ErrorMessage);
UCLASS()
class GAMELIFTCLIENTSDK_API UGameLiftDescribeMatchmakingAsync : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

		friend class UGameLiftClientObject;

public:
	UPROPERTY(BlueprintAssignable)
		FOnDescribeMatchmakingAsync Success;

	UPROPERTY(BlueprintAssignable)
		FOnDescribeMatchmakingAsync Failed;

private:
	Aws::GameLift::GameLiftClient* GameLiftClient;
	TArray<FString> TicketIds;

public:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "Game Lift Async|Matchmaking")
		static UGameLiftDescribeMatchmakingAsync* DescribeMatchmaking(const UObject* WorldContextObject, const UGameLiftClientObject* GameLiftClientObject, TArray<FString> TicketIds);

	virtual void Activate() override;

private:
	const UObject* WorldContextObject;

	void OnDescribeMatchmakingState(const Aws::GameLift::GameLiftClient* Client, const Aws::GameLift::Model::DescribeMatchmakingRequest& Request, const Aws::GameLift::Model::DescribeMatchmakingOutcome& Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context);
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStopMatchmakingAsync, const FString&, ErrorMessage);
UCLASS()
class GAMELIFTCLIENTSDK_API UGameLiftStopMatchmakingAsync : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

		friend class UGameLiftClientObject;

public:
	UPROPERTY(BlueprintAssignable)
		FOnStopMatchmakingAsync Success;

	UPROPERTY(BlueprintAssignable)
		FOnStopMatchmakingAsync Failed;

private:
	Aws::GameLift::GameLiftClient* GameLiftClient;
	FString TicketId;

public:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "Game Lift Async|Matchmaking")
		static UGameLiftStopMatchmakingAsync* StopMatchmaking(const UObject* WorldContextObject, const UGameLiftClientObject* GameLiftClientObject, FString TicketId);

	virtual void Activate() override;

private:
	const UObject* WorldContextObject;

	void OnStopMatchmakingState(const Aws::GameLift::GameLiftClient* Client, const Aws::GameLift::Model::StopMatchmakingRequest& Request, const Aws::GameLift::Model::StopMatchmakingOutcome& Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context);
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAcceptMatchAsync, const FString&, ErrorMessage);
UCLASS()
class GAMELIFTCLIENTSDK_API UGameLiftAcceptMatchAsync : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

		friend class UGameLiftClientObject;

public:
	UPROPERTY(BlueprintAssignable)
		FOnAcceptMatchAsync Success;

	UPROPERTY(BlueprintAssignable)
		FOnAcceptMatchAsync Failed;

private:
	Aws::GameLift::GameLiftClient* GameLiftClient;
	EGameLiftAcceptanceType AcceptanceType;
	TArray<FString> PlayerIds;
	FString TicketId;

public:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "Game Lift Async|Matchmaking")
		static UGameLiftAcceptMatchAsync* AcceptMatch(const UObject* WorldContextObject, const UGameLiftClientObject* GameLiftClientObject, EGameLiftAcceptanceType AcceptanceType, TArray<FString> PlayerIds, FString TicketId);

	virtual void Activate() override;

private:
	const UObject* WorldContextObject;

	void OnAcceptMatchState(const Aws::GameLift::GameLiftClient* Client, const Aws::GameLift::Model::AcceptMatchRequest& Request, const Aws::GameLift::Model::AcceptMatchOutcome& Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context);
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStartMatchBackfillAsync, const FGameLiftMatchmakingTicket&, MatchmakingTicket, const FString&, ErrorMessage);
UCLASS()
class GAMELIFTCLIENTSDK_API UGameLiftStartMatchBackfillAsync : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

		friend class UGameLiftClientObject;

public:
	UPROPERTY(BlueprintAssignable)
		FOnStartMatchBackfillAsync Success;

	UPROPERTY(BlueprintAssignable)
		FOnStartMatchBackfillAsync Failed;

private:
	Aws::GameLift::GameLiftClient* GameLiftClient;
	FString ConfigurationName;
	FString GameSessionArn;
	TArray<FGameLiftPlayer> Players;
	FString TicketId;

public:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "Game Lift Async|Matchmaking")
		static UGameLiftStartMatchBackfillAsync* StartMatchBackfill(const UObject* WorldContextObject, const UGameLiftClientObject* GameLiftClientObject, FString ConfigurationName, FString GameSessionArn, TArray<FGameLiftPlayer> Players, FString TicketId);

	virtual void Activate() override;

private:
	const UObject* WorldContextObject;

	void OnStartMatchBackfillState(const Aws::GameLift::GameLiftClient* Client, const Aws::GameLift::Model::StartMatchBackfillRequest& Request, const Aws::GameLift::Model::StartMatchBackfillOutcome& Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context);
};