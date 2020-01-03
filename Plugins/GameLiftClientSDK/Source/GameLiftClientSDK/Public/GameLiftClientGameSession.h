// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "GameLiftClientTypes.h"
#if WITH_GAMELIFTCLIENTSDK
#include "aws/gamelift/GameLiftClient.h"
#endif

#include "GameLiftClientGameSession.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCreateGameSessionAsync, const FGameLiftGameSession&, GameSession, const FString&, ErrorMessage);
UCLASS()
class GAMELIFTCLIENTSDK_API UGameLiftCreateGameSessionAsync : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

	friend class UGameLiftClientObject;

public:
	UPROPERTY(BlueprintAssignable)
	FOnCreateGameSessionAsync Success;

	UPROPERTY(BlueprintAssignable)
	FOnCreateGameSessionAsync Failed;
	
private:
	Aws::GameLift::GameLiftClient* GameLiftClient;
	FGameLiftGameSessionConfig SessionConfig;

	bool bIsUsingGameLiftLocal;


public:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "Game Lift Async|Game Session")
		static UGameLiftCreateGameSessionAsync* CreateGameSession(const UObject* WorldContextObject, const UGameLiftClientObject* GameLiftClientObject, FGameLiftGameSessionConfig GameSessionProperties);

	virtual void Activate() override;

private:
	const UObject* WorldContextObject;

	void OnCreateGameSession(const Aws::GameLift::GameLiftClient* Client, const Aws::GameLift::Model::CreateGameSessionRequest& Request, const Aws::GameLift::Model::CreateGameSessionOutcome& Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context);
};

USTRUCT(BlueprintType)
struct FGameLiftGameSessionDetail{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FGameLiftGameSession GameSession;

	UPROPERTY(BlueprintReadWrite)
	EGameLiftProtectionPolicy ProtectionPolicy;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDescribeGameSessionDetailsAsync, const TArray<FGameLiftGameSessionDetail>&, GameSessionDetails, const FString&, NextToken, const FString&, ErrorMessage);
UCLASS()
class GAMELIFTCLIENTSDK_API UGameLiftDescribeGameSessionDetailsAsync : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

	friend class UGameLiftClientObject;

public:
	UPROPERTY(BlueprintAssignable)
		FOnDescribeGameSessionDetailsAsync Success;

	UPROPERTY(BlueprintAssignable)
		FOnDescribeGameSessionDetailsAsync Failed;

private:
	Aws::GameLift::GameLiftClient* GameLiftClient;
	FString SessionID;
	FString AliasID;
	int Limit;
	FString NextToken;
	FString StatusFillter;

public:
	/**
	* @Param StatusFillter Game session status to filter results on. Possible game session statuses include ACTIVE,
	* TERMINATED, ACTIVATING, and TERMINATING (the last two are transitory).
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "Game Lift Async|Game Session")
		static UGameLiftDescribeGameSessionDetailsAsync* DescribeGameSessionDetails(const UObject* WorldContextObject, const UGameLiftClientObject* GameLiftClientObject, FString GameSessionID, FString AliasID, int Limit, FString NextToken, FString StatusFillter);

	virtual void Activate() override;

private:
	const UObject* WorldContextObject;

	void OnDescribeGameSessionState(const Aws::GameLift::GameLiftClient* Client, const Aws::GameLift::Model::DescribeGameSessionDetailsRequest& Request, const Aws::GameLift::Model::DescribeGameSessionDetailsOutcome& Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context);
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDescribeGameSessionsAsync, const TArray<FGameLiftGameSession>&, GameSessions, const FString&, NextToken, const FString&, ErrorMessage);
UCLASS()
class GAMELIFTCLIENTSDK_API UGameLiftDescribeGameSessionsAsync : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

		friend class UGameLiftClientObject;

public:
	UPROPERTY(BlueprintAssignable)
		FOnDescribeGameSessionsAsync Success;

	UPROPERTY(BlueprintAssignable)
		FOnDescribeGameSessionsAsync Failed;

private:
	Aws::GameLift::GameLiftClient* GameLiftClient;
	FString SessionID;
	FString AliasID;
	int Limit;
	FString NextToken;
	FString StatusFillter;

public:
	/**
	* @Param StatusFillter Game session status to filter results on. Possible game session statuses include ACTIVE,
	* TERMINATED, ACTIVATING, and TERMINATING (the last two are transitory).
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "Game Lift Async|Game Session")
		static UGameLiftDescribeGameSessionsAsync* DescribeGameSessions(const UObject* WorldContextObject, const UGameLiftClientObject* GameLiftClientObject, FString GameSessionID, FString AliasID, int Limit, FString NextToken, FString StatusFillter);

	virtual void Activate() override;

private:
	const UObject* WorldContextObject;

	void OnDescribeGameSessionState(const Aws::GameLift::GameLiftClient* Client, const Aws::GameLift::Model::DescribeGameSessionsRequest& Request, const Aws::GameLift::Model::DescribeGameSessionsOutcome& Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context);
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSearchGameSessionsAsync, const TArray<FGameLiftGameSession>&, GameSessions, const FString&, NextToken, const FString&, ErrorMessage);
UCLASS()
class GAMELIFTCLIENTSDK_API UGameLiftSearchGameSessionsAsync : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

		friend class UGameLiftClientObject;

public:
	UPROPERTY(BlueprintAssignable)
		FOnSearchGameSessionsAsync Success;

	UPROPERTY(BlueprintAssignable)
		FOnSearchGameSessionsAsync Failed;

private:
	Aws::GameLift::GameLiftClient* GameLiftClient;
	FString AliasID;
	FString FillterExpression;
	int Limit;
	FString NextToken;
	FString SortExpression;
	bool Active;

public:
	/**
	* @Param StatusFillter Game session status to filter results on. Possible game session statuses include ACTIVE,
	* TERMINATED, ACTIVATING, and TERMINATING (the last two are transitory).
	*/
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "Game Lift Async|Game Session")
		static UGameLiftSearchGameSessionsAsync* SearchGameSessions(const UObject* WorldContextObject, const UGameLiftClientObject* GameLiftClientObject, FString AliasID, FString FillterExpresion, int Limit, FString NextToken, FString SortExpression);

	virtual void Activate() override;

private:
	const UObject* WorldContextObject;

	void OnSearchGameSessionState(const Aws::GameLift::GameLiftClient* Client, const Aws::GameLift::Model::SearchGameSessionsRequest& Request, const Aws::GameLift::Model::SearchGameSessionsOutcome& Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context);
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpdateGameSessionAsync, const FGameLiftGameSession&, GameSession, const FString&, ErrorMessage);
UCLASS()
class GAMELIFTCLIENTSDK_API UGameLiftUpdateGameSessionAsync : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

		friend class UGameLiftClientObject;

public:
	UPROPERTY(BlueprintAssignable)
		FOnUpdateGameSessionAsync Success;

	UPROPERTY(BlueprintAssignable)
		FOnUpdateGameSessionAsync Failed;

private:
	Aws::GameLift::GameLiftClient* GameLiftClient;
	FString SessionID;
	int MaximumPlayerSessionCount;
	FString Name;
	EGameLiftPlayerSessionCreationPolicy PlayerSessionCreationPolicy;
	EGameLiftProtectionPolicy ProtectionPolicy;

public:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "Game Lift Async|Game Session")
		static UGameLiftUpdateGameSessionAsync* UpdateGameSession(const UObject* WorldContextObject, const UGameLiftClientObject* GameLiftClientObject, FString SessionID, FString Name, EGameLiftPlayerSessionCreationPolicy PlayerSessionCreationPolicy, EGameLiftProtectionPolicy ProtectionPolicy, int MaximumPlayerSessionCount);

	virtual void Activate() override;

private:
	const UObject* WorldContextObject;

	void OnUpdateGameSessionState(const Aws::GameLift::GameLiftClient* Client, const Aws::GameLift::Model::UpdateGameSessionRequest& Request, const Aws::GameLift::Model::UpdateGameSessionOutcome& Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context);
};