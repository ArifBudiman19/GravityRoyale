// Fill out your copyright notice in the Description page of Project Settings.
#define USE_IMPORT_EXPORT

#include "GameLiftClientGameSessionQueue.h"
#include "GameLiftClientObject.h"
#include "GameLiftClientGlobals.h"
#include "GameLiftClientHelper.h"
#if WITH_GAMELIFTCLIENTSDK
#include "aws/core/utils/Outcome.h"
#include "aws/gamelift/GameLiftClient.h"
#include "aws/gamelift/model/CreateGameSessionQueueRequest.h"
#include "aws/gamelift/model/DescribeGameSessionQueuesRequest.h"
#include "aws/gamelift/model/UpdateGameSessionQueueRequest.h"
#include "aws/gamelift/model/DeleteGameSessionQueueRequest.h"
#endif

UGameLiftCreateGameSessionQueueAsync * UGameLiftCreateGameSessionQueueAsync::CreateGameSessionQueue(const UObject * WorldContextObject, const UGameLiftClientObject * GameLiftClientObject, const FString Name, const TArray<FGameLiftDestination> Destinations, const TArray<FGameLiftPlayerLatencyPolicy> PlayerLatencyPolicies, int TimeoutInSeconds)
{
#if WITH_GAMELIFTCLIENTSDK
	UGameLiftCreateGameSessionQueueAsync* Proxy = NewObject<UGameLiftCreateGameSessionQueueAsync>();
	if (GameLiftClientObject) {
		Proxy->GameLiftClient = GameLiftClientObject->GameLiftClient;
	}

	Proxy->WorldContextObject = WorldContextObject;
	Proxy->Name = Name;
	Proxy->Destinations = Destinations;
	Proxy->PlayerLatencyPolicies = PlayerLatencyPolicies;
	Proxy->TimeoutInSeconds = TimeoutInSeconds;
	return Proxy;
#endif
	return nullptr;
}

void UGameLiftCreateGameSessionQueueAsync::Activate()
{
#if WITH_GAMELIFTCLIENTSDK
	if (GameLiftClient)
	{
		LOG_NORMAL("Preparing to create game sessions queue...");

		Aws::GameLift::Model::CreateGameSessionQueueRequest Request;

		Request.SetName(TCHAR_TO_UTF8(*Name));

		if (TimeoutInSeconds > 0) {
			Request.SetTimeoutInSeconds(TimeoutInSeconds);
		}

		for (FGameLiftDestination Destination : Destinations) {
			if (!Destination.DestinationArn.IsEmpty()) {
			Aws::GameLift::Model::GameSessionQueueDestination Dest;
			Dest.SetDestinationArn(TCHAR_TO_UTF8(*Destination.DestinationArn));
			Request.AddDestinations(Dest);
			}
		}

		for (FGameLiftPlayerLatencyPolicy Latency: PlayerLatencyPolicies) {
			Aws::GameLift::Model::PlayerLatencyPolicy Policy;
			Policy.SetMaximumIndividualPlayerLatencyMilliseconds(Latency.MaximumIndividualPlayerLatencyMilliseconds);
			Policy.SetPolicyDurationSeconds(Latency.PolicyDurationSeconds);
			Request.AddPlayerLatencyPolicies(Policy);
		}


		Aws::GameLift::CreateGameSessionQueueResponseReceivedHandler Handler;
		Handler = std::bind(&UGameLiftCreateGameSessionQueueAsync::OnCreateGameSessionQueueState, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

		LOG_NORMAL("Requesting to create game session queue")
		GameLiftClient->CreateGameSessionQueueAsync(Request, Handler);
	}
	else
	{
		const FString MyErrorMessage = "Game lift object is null";
		LOG_ERROR("Received OnCreateGameSessionQueue with failed outcome. Error: " + MyErrorMessage);
	}
#else
	const FString MyErrorMessage = "no GameLiftSDK activated";
	LOG_ERROR("Received OnCreateGameSessionQueue with failed outcome. Error: " + MyErrorMessage);
#endif
}

void UGameLiftCreateGameSessionQueueAsync::OnCreateGameSessionQueueState(const Aws::GameLift::GameLiftClient * Client, const Aws::GameLift::Model::CreateGameSessionQueueRequest & Request, const Aws::GameLift::Model::CreateGameSessionQueueOutcome & Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context)
{
#if WITH_GAMELIFTCLIENTSDK
	if (Outcome.IsSuccess())
	{
		LOG_NORMAL("Received OnCreateGameSessionQueue with Success outcome.");
		FGameLiftGameSessionQueue GameSessionQueue;
		Aws::GameLift::Model::GameSessionQueue SessionQueue =  Outcome.GetResult().GetGameSessionQueue();

		UGameLiftClientHelper::ParseGameSessionQueue(&SessionQueue, GameSessionQueue);

		Success.Broadcast(GameSessionQueue, FString());
	}
	else
	{
		const FString MyErrorMessage = FString(Outcome.GetError().GetMessage().c_str());
		LOG_ERROR("Received OnCreateGameSessionQueues with failed outcome. Error: " + MyErrorMessage);
		Failed.Broadcast(FGameLiftGameSessionQueue(), MyErrorMessage);
	}
#endif
}

UGameLiftDescribeGameSessionQueuesAsync * UGameLiftDescribeGameSessionQueuesAsync::DescribeGameSessionQueues(const UObject * WorldContextObject, const UGameLiftClientObject * GameLiftClientObject, const TArray<FString> Names, const int Limit, const FString NextToken)
{
#if WITH_GAMELIFTCLIENTSDK
	UGameLiftDescribeGameSessionQueuesAsync* Proxy = NewObject<UGameLiftDescribeGameSessionQueuesAsync>();
	if (GameLiftClientObject) {
		Proxy->GameLiftClient = GameLiftClientObject->GameLiftClient;
	}

	Proxy->WorldContextObject = WorldContextObject;
	Proxy->Names = Names;
	Proxy->NextToken = NextToken;
	Proxy->Limit = Limit;
	return Proxy;
#endif
	return nullptr;
}

void UGameLiftDescribeGameSessionQueuesAsync::Activate()
{
#if WITH_GAMELIFTCLIENTSDK
	if (GameLiftClient)
	{
		LOG_NORMAL("Preparing to describe game session queues...");

		Aws::GameLift::Model::DescribeGameSessionQueuesRequest Request;

		for (FString Name : Names) {
			Request.AddNames(TCHAR_TO_UTF8(*Name));
		}

		if (!NextToken.IsEmpty()) {
			Request.SetNextToken(TCHAR_TO_UTF8(*NextToken));
		}

		if (Limit > 0) {
			Request.SetLimit(Limit);
		}

		Aws::GameLift::DescribeGameSessionQueuesResponseReceivedHandler Handler;
		Handler = std::bind(&UGameLiftDescribeGameSessionQueuesAsync::OnDescribeGameSessionQueuesState, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

		LOG_NORMAL("Requesting to describe game session queues")
		GameLiftClient->DescribeGameSessionQueuesAsync(Request, Handler);
	}
	else
	{
		const FString MyErrorMessage = "Game lift object is null";
		LOG_ERROR("Received OnDescribeGameSessionQueues with failed outcome. Error: " + MyErrorMessage);
	}
#else
	const FString MyErrorMessage = "no GameLiftSDK activated";
	LOG_ERROR("Received OnDescribeGameSessionQueues with failed outcome. Error: " + MyErrorMessage);
#endif
}

void UGameLiftDescribeGameSessionQueuesAsync::OnDescribeGameSessionQueuesState(const Aws::GameLift::GameLiftClient * Client, const Aws::GameLift::Model::DescribeGameSessionQueuesRequest & Request, const Aws::GameLift::Model::DescribeGameSessionQueuesOutcome & Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context)
{
#if WITH_GAMELIFTCLIENTSDK
	if (Outcome.IsSuccess())
	{
		FString Next = Outcome.GetResult().GetNextToken().c_str();
		TArray<FGameLiftGameSessionQueue> GameSessionQueues;
		Aws::Vector<Aws::GameLift::Model::GameSessionQueue> SessionQueues = Outcome.GetResult().GetGameSessionQueues();
		
		for (auto it = Outcome.GetResult().GetGameSessionQueues().begin(); it != Outcome.GetResult().GetGameSessionQueues().end(); ++it) {
			FGameLiftGameSessionQueue GameSessionQueue;
			UGameLiftClientHelper::ParseGameSessionQueue(it._Ptr, GameSessionQueue);
			GameSessionQueues.Add(GameSessionQueue);
		}

		LOG_NORMAL("Received OnDescribeGameSessionQueues with Success outcome.");
		Success.Broadcast(GameSessionQueues, Next, FString());
	}
	else
	{
		TArray<FGameLiftGameSessionQueue> GameSessionQueues;
		const FString MyErrorMessage = FString(Outcome.GetError().GetMessage().c_str());
		LOG_ERROR("Received OnDescribeGameSessionQueues with failed outcome. Error: " + MyErrorMessage);
		Failed.Broadcast(GameSessionQueues, FString(), MyErrorMessage);
	}
#endif
}

UGameLiftUpdateGameSessionQueueAsync * UGameLiftUpdateGameSessionQueueAsync::UpdateGameSessionQueue(const UObject * WorldContextObject, const UGameLiftClientObject * GameLiftClientObject, const FString Name, const TArray<FGameLiftDestination> Destinations, const TArray<FGameLiftPlayerLatencyPolicy> PlayerLatencyPolicies, int TimeoutInSeconds)
{
#if WITH_GAMELIFTCLIENTSDK
	UGameLiftUpdateGameSessionQueueAsync* Proxy = NewObject<UGameLiftUpdateGameSessionQueueAsync>();
	if (GameLiftClientObject) {
		Proxy->GameLiftClient = GameLiftClientObject->GameLiftClient;
	}

	Proxy->WorldContextObject = WorldContextObject;
	Proxy->Name = Name;
	Proxy->Destinations = Destinations;
	Proxy->PlayerLatencyPolicies = PlayerLatencyPolicies;
	Proxy->TimeoutInSeconds = TimeoutInSeconds;
	return Proxy;
#endif
	return nullptr;
}

void UGameLiftUpdateGameSessionQueueAsync::Activate()
{
#if WITH_GAMELIFTCLIENTSDK
	if (GameLiftClient)
	{
		LOG_NORMAL("Preparing to update game sessions queue...");

		Aws::GameLift::Model::UpdateGameSessionQueueRequest Request;

		Request.SetName(TCHAR_TO_UTF8(*Name));

		if (TimeoutInSeconds > 0) {
			Request.SetTimeoutInSeconds(TimeoutInSeconds);
		}

		for (FGameLiftDestination Destination : Destinations) {
			if (!Destination.DestinationArn.IsEmpty()) {
				Aws::GameLift::Model::GameSessionQueueDestination Dest;
				Dest.SetDestinationArn(TCHAR_TO_UTF8(*Destination.DestinationArn));
				Request.AddDestinations(Dest);
			}
		}

		for (FGameLiftPlayerLatencyPolicy Latency : PlayerLatencyPolicies) {
			Aws::GameLift::Model::PlayerLatencyPolicy Policy;
			Policy.SetMaximumIndividualPlayerLatencyMilliseconds(Latency.MaximumIndividualPlayerLatencyMilliseconds);
			Policy.SetPolicyDurationSeconds(Latency.PolicyDurationSeconds);
			Request.AddPlayerLatencyPolicies(Policy);
		}


		Aws::GameLift::UpdateGameSessionQueueResponseReceivedHandler Handler;
		Handler = std::bind(&UGameLiftUpdateGameSessionQueueAsync::OnUpdateGameSessionQueueState, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

		LOG_NORMAL("Requesting to update game session queue")
		GameLiftClient->UpdateGameSessionQueueAsync(Request, Handler);
	}
	else
	{
		const FString MyErrorMessage = "Game lift object is null";
		LOG_ERROR("Received OnUpdateGameSessionQueue with failed outcome. Error: " + MyErrorMessage);
	}
#else
	const FString MyErrorMessage = "no GameLiftSDK activated";
	LOG_ERROR("Received OnUpdateGameSessionQueue with failed outcome. Error: " + MyErrorMessage);
#endif
}

void UGameLiftUpdateGameSessionQueueAsync::OnUpdateGameSessionQueueState(const Aws::GameLift::GameLiftClient * Client, const Aws::GameLift::Model::UpdateGameSessionQueueRequest & Request, const Aws::GameLift::Model::UpdateGameSessionQueueOutcome & Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context)
{
#if WITH_GAMELIFTCLIENTSDK
	if (Outcome.IsSuccess())
	{
		LOG_NORMAL("Received OnUpdateGameSessionQueue with Success outcome.");
		FGameLiftGameSessionQueue GameSessionQueue;
		Aws::GameLift::Model::GameSessionQueue SessionQueue = Outcome.GetResult().GetGameSessionQueue();

		UGameLiftClientHelper::ParseGameSessionQueue(&SessionQueue, GameSessionQueue);

		Success.Broadcast(GameSessionQueue, FString());
	}
	else
	{
		const FString MyErrorMessage = FString(Outcome.GetError().GetMessage().c_str());
		LOG_ERROR("Received OnUpdateGameSessionQueue with failed outcome. Error: " + MyErrorMessage);
		Failed.Broadcast(FGameLiftGameSessionQueue(), MyErrorMessage);
	}
#endif
}

UGameLiftDeleteGameSessionQueueAsync * UGameLiftDeleteGameSessionQueueAsync::DeleteGameSessionQueue(const UObject * WorldContextObject, const UGameLiftClientObject * GameLiftClientObject, const FString Name)
{
#if WITH_GAMELIFTCLIENTSDK
	UGameLiftDeleteGameSessionQueueAsync* Proxy = NewObject<UGameLiftDeleteGameSessionQueueAsync>();
	if (GameLiftClientObject) {
		Proxy->GameLiftClient = GameLiftClientObject->GameLiftClient;
	}

	Proxy->WorldContextObject = WorldContextObject;
	Proxy->Name = Name;
	return Proxy;
#endif
	return nullptr;
}

void UGameLiftDeleteGameSessionQueueAsync::Activate()
{
#if WITH_GAMELIFTCLIENTSDK
	if (GameLiftClient)
	{
		LOG_NORMAL("Preparing to delete game sessions queue...");

		Aws::GameLift::Model::DeleteGameSessionQueueRequest Request;

		Request.SetName(TCHAR_TO_UTF8(*Name));

		Aws::GameLift::DeleteGameSessionQueueResponseReceivedHandler Handler;
		Handler = std::bind(&UGameLiftDeleteGameSessionQueueAsync::OnDeleteGameSessionQueueState, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

		LOG_NORMAL("Requesting to delete game session queue")
			GameLiftClient->DeleteGameSessionQueueAsync(Request, Handler);
	}
	else
	{
		const FString MyErrorMessage = "Game lift object is null";
		LOG_ERROR("Received OnDeleteGameSessionQueue with failed outcome. Error: " + MyErrorMessage);
	}
#else
	const FString MyErrorMessage = "no GameLiftSDK activated";
	LOG_ERROR("Received OnDeleteGameSessionQueue with failed outcome. Error: " + MyErrorMessage);
#endif
}

void UGameLiftDeleteGameSessionQueueAsync::OnDeleteGameSessionQueueState(const Aws::GameLift::GameLiftClient * Client, const Aws::GameLift::Model::DeleteGameSessionQueueRequest & Request, const Aws::GameLift::Model::DeleteGameSessionQueueOutcome & Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context)
{
#if WITH_GAMELIFTCLIENTSDK
	if (Outcome.IsSuccess())
	{
		LOG_NORMAL("Received OnDeleteGameSessionQueue with Success outcome.");
		Success.Broadcast(FString());
	}
	else
	{
		const FString MyErrorMessage = FString(Outcome.GetError().GetMessage().c_str());
		LOG_ERROR("Received OnDeleteGameSessionQueue with failed outcome. Error: " + MyErrorMessage);
		Failed.Broadcast(MyErrorMessage);
	}
#endif
}
