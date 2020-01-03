// Fill out your copyright notice in the Description page of Project Settings.

#define USE_IMPORT_EXPORT

#include "GameLiftClientMatchmaking.h"
#include "GameLiftClientObject.h"
#include "GameLiftClientGlobals.h"
#include "GameLiftClientHelper.h"
#if WITH_GAMELIFTCLIENTSDK
#include "aws/core/utils/Outcome.h"
#include "aws/gamelift/GameLiftClient.h"
#include "aws/gamelift/model/StartMatchmakingRequest.h"
#include "aws/gamelift/model/DescribeMatchmakingRequest.h"
#include "aws/gamelift/model/StopMatchmakingRequest.h"
#include "aws/gamelift/model/AcceptMatchRequest.h"
#include "aws/gamelift/model/StartMatchBackfillRequest.h"
#endif

UGameLiftStartMatchmakingAsync * UGameLiftStartMatchmakingAsync::StartMatchmaking(const UObject * WorldContextObject, const UGameLiftClientObject * GameLiftClientObject, FString ConfigurationName, TArray<FGameLiftPlayer> Players, FString TicketId)
{
#if WITH_GAMELIFTCLIENTSDK
	UGameLiftStartMatchmakingAsync* Proxy = NewObject<UGameLiftStartMatchmakingAsync>();
	Proxy->WorldContextObject = WorldContextObject;
	if (GameLiftClientObject) {
		Proxy->GameLiftClient = GameLiftClientObject->GameLiftClient;
	}
	Proxy->ConfigurationName = ConfigurationName;
	Proxy->Players = Players;
	Proxy->TicketId = TicketId;
	return Proxy;
#endif
	return nullptr;
}

void UGameLiftStartMatchmakingAsync::Activate()
{
#if WITH_GAMELIFTCLIENTSDK
	if (GameLiftClient)
	{
		LOG_NORMAL("Preparing to start matchmaking...");

		Aws::GameLift::Model::StartMatchmakingRequest Request;

		Request.SetConfigurationName(TCHAR_TO_UTF8(*ConfigurationName));

		for (auto p : Players) {
			Aws::GameLift::Model::Player Player;
			Player.SetPlayerId(TCHAR_TO_UTF8(*p.PlayerId));
			Player.SetTeam(TCHAR_TO_UTF8(*p.Team));
			
			for (auto a = p.PlayerAttributes.begin(); a != p.PlayerAttributes.end(); ++a) {
				Aws::GameLift::Model::AttributeValue Attribute;

				Attribute.SetN(a.Value().N);
				if (!a.Value().S.IsEmpty()) {
					Attribute.SetS(TCHAR_TO_UTF8(*a.Value().S));
				}

				for (auto sl : a.Value().SL) {
					Attribute.AddSL(TCHAR_TO_UTF8(*sl));
				}

				for (auto sdm = a.Value().SDM.begin(); sdm != a.Value().SDM.end(); ++sdm) {
					Attribute.AddSDM(TCHAR_TO_UTF8(*sdm.Key()), sdm.Value());
				}

				Player.AddPlayerAttributes(TCHAR_TO_UTF8(*a.Key()), Attribute);
			}

			for (auto l = p.LatencyInMs.begin(); l != p.LatencyInMs.end(); ++l) {
				Player.AddLatencyInMs(TCHAR_TO_UTF8(*l.Key()), l.Value());
			}

			Request.AddPlayers(Player);

		}
		
		if (!TicketId.IsEmpty()) {
			Request.SetTicketId(TCHAR_TO_UTF8(*TicketId));
		}

		Aws::GameLift::StartMatchmakingResponseReceivedHandler Handler;
		Handler = std::bind(&UGameLiftStartMatchmakingAsync::OnStartMatchmakingState, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

		LOG_NORMAL("Requesting to start matchmaking")
		GameLiftClient->StartMatchmakingAsync(Request, Handler);
	}
	else
	{
		const FString MyErrorMessage = "Game lift object is null";
		LOG_ERROR("Received StartMatchmaking with failed outcome. Error: " + MyErrorMessage);
	}
#else
	const FString MyErrorMessage = "no GameLiftSDK activated";
	LOG_ERROR("Received StartMatchmaking with failed outcome. Error: " + MyErrorMessage);
#endif
}

void UGameLiftStartMatchmakingAsync::OnStartMatchmakingState(const Aws::GameLift::GameLiftClient * Client, const Aws::GameLift::Model::StartMatchmakingRequest & Request, const Aws::GameLift::Model::StartMatchmakingOutcome & Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context)
{
#if WITH_GAMELIFTCLIENTSDK
	if (Outcome.IsSuccess())
	{
		Aws::GameLift::Model::MatchmakingTicket MatchmakingTicket = Outcome.GetResult().GetMatchmakingTicket();
		LOG_NORMAL("Received OnStartMatchmaking with Success outcome.");
		Success.Broadcast(UGameLiftClientHelper::ParseMatchmakingTicket(&MatchmakingTicket), FString());
	}
	else
	{
		const FString MyErrorMessage = FString(Outcome.GetError().GetMessage().c_str());
		LOG_ERROR("Received OnStartMatchmaking with failed outcome. Error: " + MyErrorMessage);
		Failed.Broadcast(FGameLiftMatchmakingTicket(), MyErrorMessage);
	}
#endif
}

UGameLiftDescribeMatchmakingAsync * UGameLiftDescribeMatchmakingAsync::DescribeMatchmaking(const UObject * WorldContextObject, const UGameLiftClientObject * GameLiftClientObject, TArray<FString> TicketIds)
{
#if WITH_GAMELIFTCLIENTSDK
	UGameLiftDescribeMatchmakingAsync* Proxy = NewObject<UGameLiftDescribeMatchmakingAsync>();
	Proxy->WorldContextObject = WorldContextObject;
	if (GameLiftClientObject) {
		Proxy->GameLiftClient = GameLiftClientObject->GameLiftClient;
	}
	Proxy->TicketIds = TicketIds;
	return Proxy;
#endif
	return nullptr;
}

void UGameLiftDescribeMatchmakingAsync::Activate()
{
#if WITH_GAMELIFTCLIENTSDK
	if (GameLiftClient)
	{
		LOG_NORMAL("Preparing to describe matchmaking...");

		Aws::GameLift::Model::DescribeMatchmakingRequest Request;

		for (FString TicketId : TicketIds) {
			Request.AddTicketIds(TCHAR_TO_UTF8(*TicketId));
		}

		Aws::GameLift::DescribeMatchmakingResponseReceivedHandler Handler;
		Handler = std::bind(&UGameLiftDescribeMatchmakingAsync::OnDescribeMatchmakingState, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

		LOG_NORMAL("Requesting to describe matchmaking")
		GameLiftClient->DescribeMatchmakingAsync(Request, Handler);
	}
	else
	{
		const FString MyErrorMessage = "Game lift object is null";
		LOG_ERROR("Received DescribeMatchmaking with failed outcome. Error: " + MyErrorMessage);
	}
#else
	const FString MyErrorMessage = "no GameLiftSDK activated";
	LOG_ERROR("Received DescribeMatchmaking with failed outcome. Error: " + MyErrorMessage);
#endif
}

void UGameLiftDescribeMatchmakingAsync::OnDescribeMatchmakingState(const Aws::GameLift::GameLiftClient * Client, const Aws::GameLift::Model::DescribeMatchmakingRequest & Request, const Aws::GameLift::Model::DescribeMatchmakingOutcome & Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context)
{
#if WITH_GAMELIFTCLIENTSDK
	TArray<FGameLiftMatchmakingTicket> TicketList;
	if (Outcome.IsSuccess())
	{
		for (auto It = Outcome.GetResult().GetTicketList().begin(); It != Outcome.GetResult().GetTicketList().end(); ++It) {
			TicketList.Add(UGameLiftClientHelper::ParseMatchmakingTicket(It._Ptr));
		}

		LOG_NORMAL("Received OnStartMatchmaking with Success outcome.");
		Success.Broadcast(TicketList, FString());
	}
	else
	{
		const FString MyErrorMessage = FString(Outcome.GetError().GetMessage().c_str());
		LOG_ERROR("Received OnStartMatchmaking with failed outcome. Error: " + MyErrorMessage);
		Failed.Broadcast(TicketList, MyErrorMessage);
	}
#endif
}

UGameLiftStopMatchmakingAsync * UGameLiftStopMatchmakingAsync::StopMatchmaking(const UObject * WorldContextObject, const UGameLiftClientObject * GameLiftClientObject, FString TicketId)
{
#if WITH_GAMELIFTCLIENTSDK
	UGameLiftStopMatchmakingAsync* Proxy = NewObject<UGameLiftStopMatchmakingAsync>();
	Proxy->WorldContextObject = WorldContextObject;
	if (GameLiftClientObject) {
		Proxy->GameLiftClient = GameLiftClientObject->GameLiftClient;
	}
	Proxy->TicketId = TicketId;
	return Proxy;
#endif
	return nullptr;
}

void UGameLiftStopMatchmakingAsync::Activate()
{
#if WITH_GAMELIFTCLIENTSDK
	if (GameLiftClient)
	{
		LOG_NORMAL("Preparing to stop matchmaking...");

		Aws::GameLift::Model::StopMatchmakingRequest Request;

		Request.SetTicketId(TCHAR_TO_UTF8(*TicketId));

		Aws::GameLift::StopMatchmakingResponseReceivedHandler Handler;
		Handler = std::bind(&UGameLiftStopMatchmakingAsync::OnStopMatchmakingState, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

		LOG_NORMAL("Requesting to stop matchmaking")
			GameLiftClient->StopMatchmakingAsync(Request, Handler);
	}
	else
	{
		const FString MyErrorMessage = "Game lift object is null";
		LOG_ERROR("Received StopMatchmaking with failed outcome. Error: " + MyErrorMessage);
	}
#else
	const FString MyErrorMessage = "no GameLiftSDK activated";
	LOG_ERROR("Received StopMatchmaking with failed outcome. Error: " + MyErrorMessage);
#endif
}

void UGameLiftStopMatchmakingAsync::OnStopMatchmakingState(const Aws::GameLift::GameLiftClient * Client, const Aws::GameLift::Model::StopMatchmakingRequest & Request, const Aws::GameLift::Model::StopMatchmakingOutcome & Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context)
{
#if WITH_GAMELIFTCLIENTSDK
	if (Outcome.IsSuccess())
	{
		LOG_NORMAL("Received OnStopMatchmaking with Success outcome.");
		Success.Broadcast(FString());
	}
	else
	{
		const FString MyErrorMessage = FString(Outcome.GetError().GetMessage().c_str());
		LOG_ERROR("Received OnStopMatchmaking with failed outcome. Error: " + MyErrorMessage);
		Failed.Broadcast(MyErrorMessage);
	}
#endif
}

UGameLiftAcceptMatchAsync * UGameLiftAcceptMatchAsync::AcceptMatch(const UObject * WorldContextObject, const UGameLiftClientObject * GameLiftClientObject, EGameLiftAcceptanceType AcceptanceType, TArray<FString> PlayerIds, FString TicketId)
{
#if WITH_GAMELIFTCLIENTSDK
	UGameLiftAcceptMatchAsync* Proxy = NewObject<UGameLiftAcceptMatchAsync>();
	Proxy->WorldContextObject = WorldContextObject;
	if (GameLiftClientObject) {
		Proxy->GameLiftClient = GameLiftClientObject->GameLiftClient;
	}
	Proxy->AcceptanceType = AcceptanceType;
	Proxy->PlayerIds = PlayerIds;
	Proxy->TicketId = TicketId;
	return Proxy;
#endif
	return nullptr;
}

void UGameLiftAcceptMatchAsync::Activate()
{
#if WITH_GAMELIFTCLIENTSDK
	if (GameLiftClient)
	{
		LOG_NORMAL("Preparing to accept match...");

		Aws::GameLift::Model::AcceptMatchRequest Request;

		Request.SetTicketId(TCHAR_TO_UTF8(*TicketId));

		switch (AcceptanceType)
		{
		case EGameLiftAcceptanceType::NOT_SET:
			Request.SetAcceptanceType(Aws::GameLift::Model::AcceptanceType::NOT_SET);
			break;
		case EGameLiftAcceptanceType::ACCEPT:
			Request.SetAcceptanceType(Aws::GameLift::Model::AcceptanceType::ACCEPT);
			break;
		case EGameLiftAcceptanceType::REJECT:
			Request.SetAcceptanceType(Aws::GameLift::Model::AcceptanceType::REJECT);
			break;
		default:
			break;
		}

		for (FString PlayerId : PlayerIds) {
			Request.AddPlayerIds(TCHAR_TO_UTF8(*PlayerId));
		}

		Aws::GameLift::AcceptMatchResponseReceivedHandler Handler;
		Handler = std::bind(&UGameLiftAcceptMatchAsync::OnAcceptMatchState, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

		LOG_NORMAL("Requesting to accept match")
		GameLiftClient->AcceptMatchAsync(Request, Handler);
	}
	else
	{
		const FString MyErrorMessage = "Game lift object is null";
		LOG_ERROR("Received AcceptMatch with failed outcome. Error: " + MyErrorMessage);
	}
#else
	const FString MyErrorMessage = "no GameLiftSDK activated";
	LOG_ERROR("Received AcceptMatch with failed outcome. Error: " + MyErrorMessage);
#endif
}

void UGameLiftAcceptMatchAsync::OnAcceptMatchState(const Aws::GameLift::GameLiftClient * Client, const Aws::GameLift::Model::AcceptMatchRequest & Request, const Aws::GameLift::Model::AcceptMatchOutcome & Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context)
{
#if WITH_GAMELIFTCLIENTSDK
	if (Outcome.IsSuccess())
	{
		LOG_NORMAL("Received OnAcceptMatch with Success outcome.");
		Success.Broadcast(FString());
	}
	else
	{
		const FString MyErrorMessage = FString(Outcome.GetError().GetMessage().c_str());
		LOG_ERROR("Received OnAcceptMatch with failed outcome. Error: " + MyErrorMessage);
		Failed.Broadcast(MyErrorMessage);
	}
#endif
}

UGameLiftStartMatchBackfillAsync * UGameLiftStartMatchBackfillAsync::StartMatchBackfill(const UObject * WorldContextObject, const UGameLiftClientObject * GameLiftClientObject, FString ConfigurationName, FString GameSessionArn, TArray<FGameLiftPlayer> Players, FString TicketId)
{
#if WITH_GAMELIFTCLIENTSDK
	UGameLiftStartMatchBackfillAsync* Proxy = NewObject<UGameLiftStartMatchBackfillAsync>();
	Proxy->WorldContextObject = WorldContextObject;
	if (GameLiftClientObject) {
		Proxy->GameLiftClient = GameLiftClientObject->GameLiftClient;
	}
	Proxy->GameSessionArn = GameSessionArn;
	Proxy->ConfigurationName = ConfigurationName;
	Proxy->Players = Players;
	Proxy->TicketId = TicketId;
	return Proxy;
#endif
	return nullptr;
}

void UGameLiftStartMatchBackfillAsync::Activate()
{
#if WITH_GAMELIFTCLIENTSDK
	if (GameLiftClient)
	{
		LOG_NORMAL("Preparing to start match backfill...");

		Aws::GameLift::Model::StartMatchBackfillRequest Request;

		Request.SetConfigurationName(TCHAR_TO_UTF8(*ConfigurationName));

		Request.SetGameSessionArn(TCHAR_TO_UTF8(*GameSessionArn));

		for (auto p : Players) {
			Aws::GameLift::Model::Player Player;
			Player.SetPlayerId(TCHAR_TO_UTF8(*p.PlayerId));
			Player.SetTeam(TCHAR_TO_UTF8(*p.Team));

			for (auto a = p.PlayerAttributes.begin(); a != p.PlayerAttributes.end(); ++a) {
				Aws::GameLift::Model::AttributeValue Attribute;

				Attribute.SetN(a.Value().N);
				if (!a.Value().S.IsEmpty()) {
					Attribute.SetS(TCHAR_TO_UTF8(*a.Value().S));
				}

				for (auto sl : a.Value().SL) {
					Attribute.AddSL(TCHAR_TO_UTF8(*sl));
				}

				for (auto sdm = a.Value().SDM.begin(); sdm != a.Value().SDM.end(); ++sdm) {
					Attribute.AddSDM(TCHAR_TO_UTF8(*sdm.Key()), sdm.Value());
				}

				Player.AddPlayerAttributes(TCHAR_TO_UTF8(*a.Key()), Attribute);
			}

			for (auto l = p.LatencyInMs.begin(); l != p.LatencyInMs.end(); ++l) {
				Player.AddLatencyInMs(TCHAR_TO_UTF8(*l.Key()), l.Value());
			}

			Request.AddPlayers(Player);

		}

		if (!TicketId.IsEmpty()) {
			Request.SetTicketId(TCHAR_TO_UTF8(*TicketId));
		}

		Aws::GameLift::StartMatchBackfillResponseReceivedHandler Handler;
		Handler = std::bind(&UGameLiftStartMatchBackfillAsync::OnStartMatchBackfillState, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

		LOG_NORMAL("Requesting to start match backfill")
			GameLiftClient->StartMatchBackfillAsync(Request, Handler);
	}
	else
	{
		const FString MyErrorMessage = "Game lift object is null";
		LOG_ERROR("Received StartMatchBackfill with failed outcome. Error: " + MyErrorMessage);
	}
#else
	const FString MyErrorMessage = "no GameLiftSDK activated";
	LOG_ERROR("Received StartMatchBackfill with failed outcome. Error: " + MyErrorMessage);
#endif
}

void UGameLiftStartMatchBackfillAsync::OnStartMatchBackfillState(const Aws::GameLift::GameLiftClient * Client, const Aws::GameLift::Model::StartMatchBackfillRequest & Request, const Aws::GameLift::Model::StartMatchBackfillOutcome & Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context)
{
#if WITH_GAMELIFTCLIENTSDK
	if (Outcome.IsSuccess())
	{
		Aws::GameLift::Model::MatchmakingTicket MatchmakingTicket = Outcome.GetResult().GetMatchmakingTicket();
		LOG_NORMAL("Received OnStartMatchBackfill with Success outcome.");
		Success.Broadcast(UGameLiftClientHelper::ParseMatchmakingTicket(&MatchmakingTicket), FString());
	}
	else
	{
		const FString MyErrorMessage = FString(Outcome.GetError().GetMessage().c_str());
		LOG_ERROR("Received OnStartMatchBackfill with failed outcome. Error: " + MyErrorMessage);
		Failed.Broadcast(FGameLiftMatchmakingTicket(), MyErrorMessage);
	}
#endif
}
