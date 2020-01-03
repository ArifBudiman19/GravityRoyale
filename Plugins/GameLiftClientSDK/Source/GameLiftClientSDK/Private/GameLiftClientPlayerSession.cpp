// Fill out your copyright notice in the Description page of Project Settings.


#include "GameLiftClientPlayerSession.h"
#include "GameLiftClientObject.h"
#include "GameLiftClientGlobals.h"
#include "GameLiftClientHelper.h"
#if WITH_GAMELIFTCLIENTSDK
#include "aws/core/utils/Outcome.h"
#include "aws/gamelift/GameLiftClient.h"
#include "aws/gamelift/model/CreatePlayerSessionRequest.h"
#include "aws/gamelift/model/CreatePlayerSessionsRequest.h"
#include "aws/gamelift/model/DescribePlayerSessionsRequest.h"
#endif

UGameLiftCreatePlayerSessionAsync * UGameLiftCreatePlayerSessionAsync::CreatePlayerSession(const UObject * WorldContextObject, const UGameLiftClientObject * GameLiftClientObject, const FString GameSessionId, const FString PlayerData, const FString PlayerId)
{
#if WITH_GAMELIFTCLIENTSDK
	UGameLiftCreatePlayerSessionAsync* Proxy = NewObject<UGameLiftCreatePlayerSessionAsync>();
	if (GameLiftClientObject) {
		Proxy->GameLiftClient = GameLiftClientObject->GameLiftClient;
	}

	Proxy->WorldContextObject = WorldContextObject;
	Proxy->GameSessionId = GameSessionId;
	Proxy->PlayerData = PlayerData;
	Proxy->PlayerId = PlayerId;
	return Proxy;
#endif
	return nullptr;
}

void UGameLiftCreatePlayerSessionAsync::Activate()
{
#if WITH_GAMELIFTCLIENTSDK
	if (GameLiftClient)
	{
		LOG_NORMAL("Preparing to create player session...");

		Aws::GameLift::Model::CreatePlayerSessionRequest Request;

		Request.SetGameSessionId(TCHAR_TO_UTF8(*GameSessionId));
		Request.SetPlayerId(TCHAR_TO_UTF8(*PlayerId));

		if (!PlayerData.IsEmpty()) {
			Request.SetPlayerData(TCHAR_TO_UTF8(*PlayerData));
		}

		Aws::GameLift::CreatePlayerSessionResponseReceivedHandler Handler;
		Handler = std::bind(&UGameLiftCreatePlayerSessionAsync::OnCreatePlayerSessionState, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

		LOG_NORMAL("Requesting to create player session")
		GameLiftClient->CreatePlayerSessionAsync(Request, Handler);
	}
	else
	{
		const FString MyErrorMessage = "Game lift object is null";
		LOG_ERROR("Received OnCreatePlayerSession with failed outcome. Error: " + MyErrorMessage);
	}
#else
	const FString MyErrorMessage = "no GameLiftSDK activated";
	LOG_ERROR("Received OnCreatePlayerSession with failed outcome. Error: " + MyErrorMessage);
#endif
}

void UGameLiftCreatePlayerSessionAsync::OnCreatePlayerSessionState(const Aws::GameLift::GameLiftClient * Client, const Aws::GameLift::Model::CreatePlayerSessionRequest & Request, const Aws::GameLift::Model::CreatePlayerSessionOutcome & Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context)
{
#if WITH_GAMELIFTCLIENTSDK
	if (Outcome.IsSuccess())
	{
		Aws::GameLift::Model::PlayerSession Session = Outcome.GetResult().GetPlayerSession();		
		LOG_NORMAL("Received OnCreatePlayerSession with Success outcome.");
		Success.Broadcast(UGameLiftClientHelper::ParsePlayerSession(&Session), FString());
	}
	else
	{
		const FString MyErrorMessage = FString(Outcome.GetError().GetMessage().c_str());
		LOG_ERROR("Received OnCreatePlayerSession with failed outcome. Error: " + MyErrorMessage);
		Failed.Broadcast(FGameLiftPlayerSession(), MyErrorMessage);
	}
#endif
}

UGameLiftCreatePlayerSessionsAsync * UGameLiftCreatePlayerSessionsAsync::CreatePlayerSessions(const UObject * WorldContextObject, const UGameLiftClientObject * GameLiftClientObject, const FString GameSessionId, const TMap<FString, FString> PlayerDataMap, const TArray<FString> PlayerIds)
{
#if WITH_GAMELIFTCLIENTSDK
	UGameLiftCreatePlayerSessionsAsync* Proxy = NewObject<UGameLiftCreatePlayerSessionsAsync>();
	if (GameLiftClientObject) {
		Proxy->GameLiftClient = GameLiftClientObject->GameLiftClient;
	}

	Proxy->WorldContextObject = WorldContextObject;
	Proxy->GameSessionId = GameSessionId;
	Proxy->PlayerDataMap = PlayerDataMap;
	Proxy->PlayerIds = PlayerIds;
	return Proxy;
#endif
	return nullptr;
}

void UGameLiftCreatePlayerSessionsAsync::Activate()
{
#if WITH_GAMELIFTCLIENTSDK
	if (GameLiftClient)
	{
		LOG_NORMAL("Preparing to create player sessions...");

		Aws::GameLift::Model::CreatePlayerSessionsRequest Request;

		Request.SetGameSessionId(TCHAR_TO_UTF8(*GameSessionId));
		for (FString PlayerId : PlayerIds) {
			Request.AddPlayerIds(TCHAR_TO_UTF8(*PlayerId));
		}

		for (auto& PlayerData : PlayerDataMap) {
			Request.AddPlayerDataMap(TCHAR_TO_UTF8(*PlayerData.Key), TCHAR_TO_UTF8(*PlayerData.Value));
		}

		Aws::GameLift::CreatePlayerSessionsResponseReceivedHandler Handler;
		Handler = std::bind(&UGameLiftCreatePlayerSessionsAsync::OnCreatePlayerSessionsState, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

		LOG_NORMAL("Requesting to create player sessions")
		GameLiftClient->CreatePlayerSessionsAsync(Request, Handler);
	}
	else
	{
		const FString MyErrorMessage = "Game lift object is null";
		LOG_ERROR("Received OnCreatePlayerSessions with failed outcome. Error: " + MyErrorMessage);
	}
#else
	const FString MyErrorMessage = "no GameLiftSDK activated";
	LOG_ERROR("Received OnCreatePlayerSessions with failed outcome. Error: " + MyErrorMessage);
#endif
}

void UGameLiftCreatePlayerSessionsAsync::OnCreatePlayerSessionsState(const Aws::GameLift::GameLiftClient * Client, const Aws::GameLift::Model::CreatePlayerSessionsRequest & Request, const Aws::GameLift::Model::CreatePlayerSessionsOutcome & Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context)
{
#if WITH_GAMELIFTCLIENTSDK
	TArray<FGameLiftPlayerSession> PlayerSessions;
	if (Outcome.IsSuccess())
	{
		for (auto It = Outcome.GetResult().GetPlayerSessions().begin(); It != Outcome.GetResult().GetPlayerSessions().end(); ++It) {
			PlayerSessions.Add(UGameLiftClientHelper::ParsePlayerSession(It._Ptr));
		}
		LOG_NORMAL("Received OnCreatePlayerSessions with Success outcome.");
		Success.Broadcast(PlayerSessions, FString());
	}
	else
	{
		const FString MyErrorMessage = FString(Outcome.GetError().GetMessage().c_str());
		LOG_ERROR("Received OnCreatePlayerSessions with failed outcome. Error: " + MyErrorMessage);
		Failed.Broadcast(PlayerSessions, MyErrorMessage);
	}
#endif
}

UGameLiftDescribePlayerSessionsAsync * UGameLiftDescribePlayerSessionsAsync::DescribePlayerSessions(const UObject * WorldContextObject, const UGameLiftClientObject * GameLiftClientObject, const FString GameSessionId, const int Limit, const FString NextToken, const FString PlayerId, const FString PlayerSessionId, const FString PlayerSessionStatusFillter)
{
#if WITH_GAMELIFTCLIENTSDK
	UGameLiftDescribePlayerSessionsAsync* Proxy = NewObject<UGameLiftDescribePlayerSessionsAsync>();
	if (GameLiftClientObject) {
		Proxy->GameLiftClient = GameLiftClientObject->GameLiftClient;
	}

	Proxy->WorldContextObject = WorldContextObject;
	Proxy->GameSessionId = GameSessionId;
	Proxy->PlayerId = PlayerId;
	Proxy->Limit = Limit;
	Proxy->NextToken = NextToken;
	Proxy->PlayerSessionId = PlayerSessionId;
	Proxy->PlayerSessionStatusFillter = PlayerSessionStatusFillter;
	return Proxy;
#endif
	return nullptr;
}

void UGameLiftDescribePlayerSessionsAsync::Activate()
{
#if WITH_GAMELIFTCLIENTSDK
	if (GameLiftClient)
	{
		LOG_NORMAL("Preparing to describe player sessions...");

		Aws::GameLift::Model::DescribePlayerSessionsRequest Request;

		Request.SetGameSessionId(TCHAR_TO_UTF8(*GameSessionId));
		
		if (Limit > 0) {
			Request.SetLimit(Limit);
		}

		if (!NextToken.IsEmpty()) {
			Request.SetNextToken(TCHAR_TO_UTF8(*NextToken));
		}

		if (!PlayerId.IsEmpty()) {
			Request.SetPlayerId(TCHAR_TO_UTF8(*PlayerId));
		}

		if (!PlayerSessionId.IsEmpty()) {
			Request.SetPlayerSessionId(TCHAR_TO_UTF8(*PlayerSessionId));
		}

		if (!PlayerSessionStatusFillter.IsEmpty()) {
			Request.SetPlayerSessionStatusFilter(TCHAR_TO_UTF8(*PlayerSessionStatusFillter.ToUpper()));
		}

		Aws::GameLift::DescribePlayerSessionsResponseReceivedHandler Handler;
		Handler = std::bind(&UGameLiftDescribePlayerSessionsAsync::OnDescribePlayerSessionsState, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

		LOG_NORMAL("Requesting to describe player sessions")
		GameLiftClient->DescribePlayerSessionsAsync(Request, Handler);
	}
	else
	{
		const FString MyErrorMessage = "Game lift object is null";
		LOG_ERROR("Received OnDescribePlayerSessions with failed outcome. Error: " + MyErrorMessage);
	}
#else
	const FString MyErrorMessage = "no GameLiftSDK activated";
	LOG_ERROR("Received OnDescribePlayerSessions with failed outcome. Error: " + MyErrorMessage);
#endif
}

void UGameLiftDescribePlayerSessionsAsync::OnDescribePlayerSessionsState(const Aws::GameLift::GameLiftClient * Client, const Aws::GameLift::Model::DescribePlayerSessionsRequest & Request, const Aws::GameLift::Model::DescribePlayerSessionsOutcome & Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context)
{
#if WITH_GAMELIFTCLIENTSDK
	TArray<FGameLiftPlayerSession> PlayerSessions;
	if (Outcome.IsSuccess())
	{
		FString Next = Outcome.GetResult().GetNextToken().c_str();
		for (auto It = Outcome.GetResult().GetPlayerSessions().begin(); It != Outcome.GetResult().GetPlayerSessions().end(); ++It) {
			PlayerSessions.Add(UGameLiftClientHelper::ParsePlayerSession(It._Ptr));
		}
		LOG_NORMAL("Received OnCreatePlayerSessions with Success outcome.");
		Success.Broadcast(PlayerSessions, Next, FString());
	}
	else
	{
		const FString MyErrorMessage = FString(Outcome.GetError().GetMessage().c_str());
		LOG_ERROR("Received OnCreatePlayerSessions with failed outcome. Error: " + MyErrorMessage);
		Failed.Broadcast(PlayerSessions, FString(), MyErrorMessage);
	}
#endif
}
