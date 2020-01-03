// Fill out your copyright notice in the Description page of Project Settings.

#define USE_IMPORT_EXPORT

#include "GameLiftClientSession.h"
#include "GameLiftClientObject.h"
#include "GameLiftClientGlobals.h"
#include "GameLiftClientHelper.h"
#if WITH_GAMELIFTCLIENTSDK
#include "aws/core/utils/Outcome.h"
#include "aws/gamelift/GameLiftClient.h"
#include "aws/gamelift/model/StartGameSessionPlacementRequest.h"
#include "aws/gamelift/model/DescribeGameSessionPlacementRequest.h"
#include "aws/gamelift/model/StopGameSessionPlacementRequest.h"
#endif

UGameLiftStartGameSessionPlacementAsync * UGameLiftStartGameSessionPlacementAsync::StartGameSessionPlacement(const UObject * WorldContextObject, const UGameLiftClientObject * GameLiftClientObject, FString GameSessionQueueName, int MaximumPlayerSessionCount, FString PlacementId, TArray<FGameLiftDesiredPlayerSession> DesiredPlayerSessions, TArray<FGameLiftMap> GameProperties, FString GameSessionData, FString GameSessionName, TArray<FGameLiftPlayerLatency> PlayerLatencies)
{
#if WITH_GAMELIFTCLIENTSDK
	UGameLiftStartGameSessionPlacementAsync* Proxy = NewObject<UGameLiftStartGameSessionPlacementAsync>();
	Proxy->WorldContextObject = WorldContextObject;
	if (GameLiftClientObject) {
		Proxy->GameLiftClient = GameLiftClientObject->GameLiftClient;
	}
	Proxy->GameSessionQueueName = GameSessionQueueName;
	Proxy->MaximumPlayerSessionCount = MaximumPlayerSessionCount;
	Proxy->PlacementId = PlacementId;
	Proxy->DesiredPlayerSessions = DesiredPlayerSessions;
	Proxy->GameProperties = GameProperties;
	Proxy->GameSessionData = GameSessionData;
	Proxy->GameSessionName = GameSessionName;
	Proxy->PlayerLatencies = PlayerLatencies;
	return Proxy;
#endif
	return nullptr;
}

void UGameLiftStartGameSessionPlacementAsync::Activate()
{
#if WITH_GAMELIFTCLIENTSDK
	if (GameLiftClient)
	{
		LOG_NORMAL("Preparing to start game session placement...");

		Aws::GameLift::Model::StartGameSessionPlacementRequest Request;

		Request.SetGameSessionQueueName(TCHAR_TO_UTF8(*GameSessionQueueName));
		Request.SetMaximumPlayerSessionCount(MaximumPlayerSessionCount);
		Request.SetPlacementId(TCHAR_TO_UTF8(*PlacementId));
		
		for (FGameLiftDesiredPlayerSession dps : DesiredPlayerSessions) {
			Aws::GameLift::Model::DesiredPlayerSession Itm;
			Itm.SetPlayerId(TCHAR_TO_UTF8(*dps.PlayerId));
			Itm.SetPlayerData(TCHAR_TO_UTF8(*dps.PlayerData));
			Request.AddDesiredPlayerSessions(Itm);
		}

		for (FGameLiftMap gp : GameProperties) {
			Aws::GameLift::Model::GameProperty Itm;
			Itm.SetKey(TCHAR_TO_UTF8(*gp.Key));
			Itm.SetValue(TCHAR_TO_UTF8(*gp.Value));
			Request.AddGameProperties(Itm);
		}

		if (!GameSessionData.IsEmpty()) {
			Request.SetGameSessionData(TCHAR_TO_UTF8(*GameSessionData));
		}

		if (!GameSessionName.IsEmpty()) {
			Request.SetGameSessionName(TCHAR_TO_UTF8(*GameSessionName));
		}

		for (FGameLiftPlayerLatency pl : PlayerLatencies) {
			Aws::GameLift::Model::PlayerLatency Itm;
			Itm.SetLatencyInMilliseconds(pl.LatencyInMilliseconds);
			Itm.SetPlayerId(TCHAR_TO_UTF8(*pl.PlayerId));
			Itm.SetRegionIdentifier(TCHAR_TO_UTF8(*pl.RegionIdentifier));
			Request.AddPlayerLatencies(Itm);
		}

		Aws::GameLift::StartGameSessionPlacementResponseReceivedHandler Handler;
		Handler = std::bind(&UGameLiftStartGameSessionPlacementAsync::OnStartGameSessionPlacementState, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

		LOG_NORMAL("Requesting to start game session placement")
		GameLiftClient->StartGameSessionPlacementAsync(Request, Handler);
	}
	else
	{
		const FString MyErrorMessage = "Game lift object is null";
		LOG_ERROR("Received StartGameSessionPlacement with failed outcome. Error: " + MyErrorMessage);
	}
#else
	const FString MyErrorMessage = "no GameLiftSDK activated";
	LOG_ERROR("Received StartGameSessionPlacement with failed outcome. Error: " + MyErrorMessage);
#endif
}

void UGameLiftStartGameSessionPlacementAsync::OnStartGameSessionPlacementState(const Aws::GameLift::GameLiftClient * Client, const Aws::GameLift::Model::StartGameSessionPlacementRequest & Request, const Aws::GameLift::Model::StartGameSessionPlacementOutcome & Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context)
{
#if WITH_GAMELIFTCLIENTSDK
	if (Outcome.IsSuccess())
	{
		Aws::GameLift::Model::GameSessionPlacement GameSessionPlacement = Outcome.GetResult().GetGameSessionPlacement();
		LOG_NORMAL("Received OnStartGameSessionPlacement with Success outcome.");
		Success.Broadcast(UGameLiftClientHelper::ParseGameSessionPlacement(&GameSessionPlacement), FString());
	}
	else
	{
		TArray<FGameLiftGameSession> GameSessionPlacement;
		const FString MyErrorMessage = FString(Outcome.GetError().GetMessage().c_str());
		LOG_ERROR("Received OnStartGameSessionPlacement with failed outcome. Error: " + MyErrorMessage);
		Failed.Broadcast(FGameLiftGameSessionPlacement(), MyErrorMessage);
	}
#endif
}

UGameLiftDescribeGameSessionPlacementAsync * UGameLiftDescribeGameSessionPlacementAsync::DescribeGameSessionPlacement(const UObject * WorldContextObject, const UGameLiftClientObject * GameLiftClientObject, FString PlacementId)
{
#if WITH_GAMELIFTCLIENTSDK
	UGameLiftDescribeGameSessionPlacementAsync* Proxy = NewObject<UGameLiftDescribeGameSessionPlacementAsync>();
	Proxy->WorldContextObject = WorldContextObject;
	if (GameLiftClientObject) {
		Proxy->GameLiftClient = GameLiftClientObject->GameLiftClient;
	}
	Proxy->PlacementId = PlacementId;
	return Proxy;
#endif
	return nullptr;
}

void UGameLiftDescribeGameSessionPlacementAsync::Activate()
{
#if WITH_GAMELIFTCLIENTSDK
	if (GameLiftClient)
	{
		LOG_NORMAL("Preparing to describe game session placement...");

		Aws::GameLift::Model::DescribeGameSessionPlacementRequest Request;

		Request.SetPlacementId(TCHAR_TO_UTF8(*PlacementId));

		Aws::GameLift::DescribeGameSessionPlacementResponseReceivedHandler Handler;
		Handler = std::bind(&UGameLiftDescribeGameSessionPlacementAsync::OnDescribeGameSessionPlacementState, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

		LOG_NORMAL("Requesting to describe game session placement")
		GameLiftClient->DescribeGameSessionPlacementAsync(Request, Handler);
	}
	else
	{
		const FString MyErrorMessage = "Game lift object is null";
		LOG_ERROR("Received DescribeGameSessionPlacement with failed outcome. Error: " + MyErrorMessage);
	}
#else
	const FString MyErrorMessage = "no GameLiftSDK activated";
	LOG_ERROR("Received DescribeGameSessionPlacement with failed outcome. Error: " + MyErrorMessage);
#endif
}

void UGameLiftDescribeGameSessionPlacementAsync::OnDescribeGameSessionPlacementState(const Aws::GameLift::GameLiftClient * Client, const Aws::GameLift::Model::DescribeGameSessionPlacementRequest & Request, const Aws::GameLift::Model::DescribeGameSessionPlacementOutcome & Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context)
{
#if WITH_GAMELIFTCLIENTSDK
	if (Outcome.IsSuccess())
	{
		Aws::GameLift::Model::GameSessionPlacement GameSessionPlacement = Outcome.GetResult().GetGameSessionPlacement();
		LOG_NORMAL("Received OnDescribeGameSessionPlacement with Success outcome.");
		Success.Broadcast(UGameLiftClientHelper::ParseGameSessionPlacement(&GameSessionPlacement), FString());
	}
	else
	{
		TArray<FGameLiftGameSession> GameSessionPlacement;
		const FString MyErrorMessage = FString(Outcome.GetError().GetMessage().c_str());
		LOG_ERROR("Received OnDescribeGameSessionPlacement with failed outcome. Error: " + MyErrorMessage);
		Failed.Broadcast(FGameLiftGameSessionPlacement(), MyErrorMessage);
	}
#endif
}

UGameLiftStopGameSessionPlacementAsync * UGameLiftStopGameSessionPlacementAsync::StopGameSessionPlacement(const UObject * WorldContextObject, const UGameLiftClientObject * GameLiftClientObject, FString PlacementId)
{
#if WITH_GAMELIFTCLIENTSDK
	UGameLiftStopGameSessionPlacementAsync* Proxy = NewObject<UGameLiftStopGameSessionPlacementAsync>();
	Proxy->WorldContextObject = WorldContextObject;
	if (GameLiftClientObject) {
		Proxy->GameLiftClient = GameLiftClientObject->GameLiftClient;
	}
	Proxy->PlacementId = PlacementId;
	return Proxy;
#endif
	return nullptr;
}

void UGameLiftStopGameSessionPlacementAsync::Activate()
{
#if WITH_GAMELIFTCLIENTSDK
	if (GameLiftClient)
	{
		LOG_NORMAL("Preparing to stop game session placement...");

		Aws::GameLift::Model::StopGameSessionPlacementRequest Request;

		Request.SetPlacementId(TCHAR_TO_UTF8(*PlacementId));

		Aws::GameLift::StopGameSessionPlacementResponseReceivedHandler Handler;
		Handler = std::bind(&UGameLiftStopGameSessionPlacementAsync::OnStopGameSessionPlacementState, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

		LOG_NORMAL("Requesting to describe game session placement")
		GameLiftClient->StopGameSessionPlacementAsync(Request, Handler);
	}
	else
	{
		const FString MyErrorMessage = "Game lift object is null";
		LOG_ERROR("Received StopGameSessionPlacement with failed outcome. Error: " + MyErrorMessage);
	}
#else
	const FString MyErrorMessage = "no GameLiftSDK activated";
	LOG_ERROR("Received StopGameSessionPlacement with failed outcome. Error: " + MyErrorMessage);
#endif
}

void UGameLiftStopGameSessionPlacementAsync::OnStopGameSessionPlacementState(const Aws::GameLift::GameLiftClient * Client, const Aws::GameLift::Model::StopGameSessionPlacementRequest & Request, const Aws::GameLift::Model::StopGameSessionPlacementOutcome & Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context)
{
#if WITH_GAMELIFTCLIENTSDK
	if (Outcome.IsSuccess())
	{
		Aws::GameLift::Model::GameSessionPlacement GameSessionPlacement = Outcome.GetResult().GetGameSessionPlacement();
		LOG_NORMAL("Received OnStopGameSessionPlacement with Success outcome.");
		Success.Broadcast(UGameLiftClientHelper::ParseGameSessionPlacement(&GameSessionPlacement), FString());
	}
	else
	{
		TArray<FGameLiftGameSession> GameSessionPlacement;
		const FString MyErrorMessage = FString(Outcome.GetError().GetMessage().c_str());
		LOG_ERROR("Received OnStopGameSessionPlacement with failed outcome. Error: " + MyErrorMessage);
		Failed.Broadcast(FGameLiftGameSessionPlacement(), MyErrorMessage);
	}
#endif
}
