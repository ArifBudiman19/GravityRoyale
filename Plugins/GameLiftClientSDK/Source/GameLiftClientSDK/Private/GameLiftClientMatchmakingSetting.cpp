// Fill out your copyright notice in the Description page of Project Settings.

#define USE_IMPORT_EXPORT

#include "GameLiftClientMatchmakingSetting.h"
#include "GameLiftClientObject.h"
#include "GameLiftClientGlobals.h"
#include "GameLiftClientHelper.h"
#if WITH_GAMELIFTCLIENTSDK
#include "aws/core/utils/Outcome.h"
#include "aws/gamelift/GameLiftClient.h"
#include "aws/gamelift/model/CreateMatchmakingConfigurationRequest.h"
#include "aws/gamelift/model/DescribeMatchmakingConfigurationsRequest.h"
#include "aws/gamelift/model/UpdateMatchmakingConfigurationRequest.h"
#include "aws/gamelift/model/DeleteMatchmakingConfigurationRequest.h"
#include "aws/gamelift/model/CreateMatchmakingRuleSetRequest.h"
#include "aws/gamelift/model/DescribeMatchmakingRuleSetsRequest.h"
#include "aws/gamelift/model/ValidateMatchmakingRuleSetRequest.h"
#include "aws/gamelift/model/DeleteMatchmakingRuleSetRequest.h"
#endif

UGameLiftCreateMatchmakingConfigurationAsync * UGameLiftCreateMatchmakingConfigurationAsync::CreateMatchmakingConfiguration(const UObject * WorldContextObject, const UGameLiftClientObject * GameLiftClientObject, bool AcceptanceRequired, TArray<FString> GameSessionQueueArns, FString Name, int RequestTimeoutSeconds, FString RuleSetName, int AcceptanceTimeoutSeconds, int AdditionalPlayerCount, EGameLiftBackfillMode BackfillMode, FString CustomEventData, FString Description, TArray<FGameLiftMap> GameProperties, FString GameSessionData, FString NotificationTarget)
{
#if WITH_GAMELIFTCLIENTSDK
	UGameLiftCreateMatchmakingConfigurationAsync* Proxy = NewObject<UGameLiftCreateMatchmakingConfigurationAsync>();
	Proxy->WorldContextObject = WorldContextObject;
	if (GameLiftClientObject) {
		Proxy->GameLiftClient = GameLiftClientObject->GameLiftClient;
	}
	Proxy->AcceptanceRequired = AcceptanceRequired;
	Proxy->GameSessionQueueArns = GameSessionQueueArns;
	Proxy->Name = Name;
	Proxy->RequestTimeoutSeconds = RequestTimeoutSeconds;
	Proxy->RuleSetName = RuleSetName;
	Proxy->AcceptanceTimeoutSeconds = AcceptanceTimeoutSeconds;
	Proxy->AdditionalPlayerCount = AdditionalPlayerCount;
	Proxy->BackfillMode = BackfillMode;
	Proxy->CustomEventData = CustomEventData;
	Proxy->Description = Description;
	Proxy->GameProperties = GameProperties;
	Proxy->GameSessionData = GameSessionData;
	Proxy->NotificationTarget = NotificationTarget;
	return Proxy;
#endif
	return nullptr;
}

void UGameLiftCreateMatchmakingConfigurationAsync::Activate()
{
#if WITH_GAMELIFTCLIENTSDK
	if (GameLiftClient)
	{
		LOG_NORMAL("Preparing to create matchmaking configuration...");

		Aws::GameLift::Model::CreateMatchmakingConfigurationRequest Request;

		Request.SetAcceptanceRequired(AcceptanceRequired);

		for (FString GameSessionQueueArn : GameSessionQueueArns) {
			Request.AddGameSessionQueueArns(TCHAR_TO_UTF8(*GameSessionQueueArn));
		}

		Request.SetName(TCHAR_TO_UTF8(*Name));
		Request.SetRuleSetName(TCHAR_TO_UTF8(*RuleSetName));

		if (AcceptanceTimeoutSeconds > 0) {
			Request.SetAcceptanceTimeoutSeconds(AcceptanceTimeoutSeconds);
		}

		if (AdditionalPlayerCount > 0) {
			Request.SetAdditionalPlayerCount(AdditionalPlayerCount);
		}

		if (BackfillMode != EGameLiftBackfillMode::NOT_SET) {
			switch (BackfillMode)
			{
			case EGameLiftBackfillMode::AUTOMATIC:
				Request.SetBackfillMode(Aws::GameLift::Model::BackfillMode::AUTOMATIC);
				break;
			case EGameLiftBackfillMode::MANUAL:
				Request.SetBackfillMode(Aws::GameLift::Model::BackfillMode::MANUAL);
				break;
			}
		}

		if (!CustomEventData.IsEmpty()) {
			Request.SetCustomEventData(TCHAR_TO_UTF8(*CustomEventData));
		}

		if (!Description.IsEmpty()) {
			Request.SetDescription(TCHAR_TO_UTF8(*Description));
		}

		for (FGameLiftMap p : GameProperties) {
			Aws::GameLift::Model::GameProperty Item;
			Item.SetKey(TCHAR_TO_UTF8(*p.Key));
			Item.SetValue(TCHAR_TO_UTF8(*p.Value));
			Request.AddGameProperties(Item);
		}

		if (!GameSessionData.IsEmpty()) {
			Request.SetGameSessionData(TCHAR_TO_UTF8(*GameSessionData));
		}

		if (!NotificationTarget.IsEmpty()) {
			Request.SetNotificationTarget(TCHAR_TO_UTF8(*NotificationTarget));
		}

		Aws::GameLift::CreateMatchmakingConfigurationResponseReceivedHandler Handler;
		Handler = std::bind(&UGameLiftCreateMatchmakingConfigurationAsync::OnCreateMatchmakingConfigurationState, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

		LOG_NORMAL("Requesting to create matchmaking configuration")
			GameLiftClient->CreateMatchmakingConfigurationAsync(Request, Handler);
	}
	else
	{
		const FString MyErrorMessage = "Game lift object is null";
		LOG_ERROR("Received CreateMatchmakingConfiguration with failed outcome. Error: " + MyErrorMessage);
	}
#else
	const FString MyErrorMessage = "no GameLiftSDK activated";
	LOG_ERROR("Received CreateMatchmakingConfiguration with failed outcome. Error: " + MyErrorMessage);
#endif
}

void UGameLiftCreateMatchmakingConfigurationAsync::OnCreateMatchmakingConfigurationState(const Aws::GameLift::GameLiftClient * Client, const Aws::GameLift::Model::CreateMatchmakingConfigurationRequest & Request, const Aws::GameLift::Model::CreateMatchmakingConfigurationOutcome & Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context)
{
#if WITH_GAMELIFTCLIENTSDK
	if (Outcome.IsSuccess())
	{
		Aws::GameLift::Model::MatchmakingConfiguration MatchmakingConfiguration = Outcome.GetResult().GetConfiguration();
		LOG_NORMAL("Received OnCreateMatchmakingConfiguration with Success outcome.");
		Success.Broadcast(UGameLiftClientHelper::ParseMatchmakingConfiguration(&MatchmakingConfiguration), FString());
	}
	else
	{
		const FString MyErrorMessage = FString(Outcome.GetError().GetMessage().c_str());
		LOG_ERROR("Received OnCreateMatchmakingConfiguration with failed outcome. Error: " + MyErrorMessage);
		Failed.Broadcast(FGameLiftMatchmakingConfiguration(), MyErrorMessage);
	}
#endif
}

UGameLiftDescribeMatchmakingConfigurationsAsync * UGameLiftDescribeMatchmakingConfigurationsAsync::DescribeMatchmakingConfigurations(const UObject * WorldContextObject, const UGameLiftClientObject * GameLiftClientObject, int Limit, TArray<FString> Names, FString NextToken, FString RuleSetName)
{
#if WITH_GAMELIFTCLIENTSDK
	UGameLiftDescribeMatchmakingConfigurationsAsync* Proxy = NewObject<UGameLiftDescribeMatchmakingConfigurationsAsync>();
	Proxy->WorldContextObject = WorldContextObject;
	if (GameLiftClientObject) {
		Proxy->GameLiftClient = GameLiftClientObject->GameLiftClient;
	}
	Proxy->Limit = Limit;
	Proxy->Names = Names;
	Proxy->NextToken = NextToken;
	Proxy->RuleSetName = RuleSetName;
	return Proxy;
#endif
	return nullptr;
}

void UGameLiftDescribeMatchmakingConfigurationsAsync::Activate()
{
#if WITH_GAMELIFTCLIENTSDK
	if (GameLiftClient)
	{
		LOG_NORMAL("Preparing to describe matchmaking configurations...");

		Aws::GameLift::Model::DescribeMatchmakingConfigurationsRequest Request;

		if (Limit > 0) {
			Request.SetLimit(Limit);
		}

		for (FString Name : Names) {
			Request.AddNames(TCHAR_TO_UTF8(*Name));
		}

		if (!NextToken.IsEmpty()) {
			Request.SetNextToken(TCHAR_TO_UTF8(*NextToken));
		}

		if (!RuleSetName.IsEmpty()) {
			Request.SetRuleSetName(TCHAR_TO_UTF8(*RuleSetName));
		}

		Aws::GameLift::DescribeMatchmakingConfigurationsResponseReceivedHandler Handler;
		Handler = std::bind(&UGameLiftDescribeMatchmakingConfigurationsAsync::OnDescribeMatchmakingConfigurationsState, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

		LOG_NORMAL("Requesting to describe matchmaking configurations")
			GameLiftClient->DescribeMatchmakingConfigurationsAsync(Request, Handler);
	}
	else
	{
		const FString MyErrorMessage = "Game lift object is null";
		LOG_ERROR("Received DescribeMatchmakingConfigurations with failed outcome. Error: " + MyErrorMessage);
	}
#else
	const FString MyErrorMessage = "no GameLiftSDK activated";
	LOG_ERROR("Received DescribeMatchmakingConfigurations with failed outcome. Error: " + MyErrorMessage);
#endif
}

void UGameLiftDescribeMatchmakingConfigurationsAsync::OnDescribeMatchmakingConfigurationsState(const Aws::GameLift::GameLiftClient * Client, const Aws::GameLift::Model::DescribeMatchmakingConfigurationsRequest & Request, const Aws::GameLift::Model::DescribeMatchmakingConfigurationsOutcome & Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context)
{
#if WITH_GAMELIFTCLIENTSDK
	TArray<FGameLiftMatchmakingConfiguration> Configurations;
	if (Outcome.IsSuccess())
	{
		for (auto It = Outcome.GetResult().GetConfigurations().begin(); It != Outcome.GetResult().GetConfigurations().end(); ++It) {
			Configurations.Add(UGameLiftClientHelper::ParseMatchmakingConfiguration(It._Ptr));
		}
		LOG_NORMAL("Received OnDescribeMatchmakingConfigurations with Success outcome.");
		Success.Broadcast(Configurations, FString());
	}
	else
	{
		const FString MyErrorMessage = FString(Outcome.GetError().GetMessage().c_str());
		LOG_ERROR("Received OnDescribeMatchmakingConfigurations with failed outcome. Error: " + MyErrorMessage);
		Failed.Broadcast(Configurations, MyErrorMessage);
	}
#endif
}

UGameLiftUpdateMatchmakingConfigurationAsync * UGameLiftUpdateMatchmakingConfigurationAsync::UpdateMatchmakingConfiguration(const UObject * WorldContextObject, const UGameLiftClientObject * GameLiftClientObject, FString Name, bool SetAcceptanceRequired, bool AcceptanceRequired, TArray<FString> GameSessionQueueArns, int RequestTimeoutSeconds, FString RuleSetName, int AcceptanceTimeoutSeconds, int AdditionalPlayerCount, EGameLiftBackfillMode BackfillMode, FString CustomEventData, FString Description, TArray<FGameLiftMap> GameProperties, FString GameSessionData, FString NotificationTarget)
{
#if WITH_GAMELIFTCLIENTSDK
	UGameLiftUpdateMatchmakingConfigurationAsync* Proxy = NewObject<UGameLiftUpdateMatchmakingConfigurationAsync>();
	Proxy->WorldContextObject = WorldContextObject;
	if (GameLiftClientObject) {
		Proxy->GameLiftClient = GameLiftClientObject->GameLiftClient;
	}
	Proxy->SetAcceptanceRequired = SetAcceptanceRequired;
	Proxy->AcceptanceRequired = AcceptanceRequired;
	Proxy->GameSessionQueueArns = GameSessionQueueArns;
	Proxy->Name = Name;
	Proxy->RequestTimeoutSeconds = RequestTimeoutSeconds;
	Proxy->RuleSetName = RuleSetName;
	Proxy->AcceptanceTimeoutSeconds = AcceptanceTimeoutSeconds;
	Proxy->AdditionalPlayerCount = AdditionalPlayerCount;
	Proxy->BackfillMode = BackfillMode;
	Proxy->CustomEventData = CustomEventData;
	Proxy->Description = Description;
	Proxy->GameProperties = GameProperties;
	Proxy->GameSessionData = GameSessionData;
	Proxy->NotificationTarget = NotificationTarget;
	return Proxy;
#endif
	return nullptr;
}

void UGameLiftUpdateMatchmakingConfigurationAsync::Activate()
{
#if WITH_GAMELIFTCLIENTSDK
	if (GameLiftClient)
	{
		LOG_NORMAL("Preparing to update matchmaking configuration...");

		Aws::GameLift::Model::UpdateMatchmakingConfigurationRequest Request;

		Request.SetName(TCHAR_TO_UTF8(*Name));

		if (SetAcceptanceRequired) {
			Request.SetAcceptanceRequired(AcceptanceRequired);
		}

		for (FString GameSessionQueueArn : GameSessionQueueArns) {
			Request.AddGameSessionQueueArns(TCHAR_TO_UTF8(*GameSessionQueueArn));
		}

		if (!RuleSetName.IsEmpty()) {
			Request.SetRuleSetName(TCHAR_TO_UTF8(*RuleSetName));
		}

		if (AcceptanceTimeoutSeconds > 0) {
			Request.SetAcceptanceTimeoutSeconds(AcceptanceTimeoutSeconds);
		}

		if (AdditionalPlayerCount > 0) {
			Request.SetAdditionalPlayerCount(AdditionalPlayerCount);
		}

		if (BackfillMode != EGameLiftBackfillMode::NOT_SET) {
			switch (BackfillMode)
			{
			case EGameLiftBackfillMode::AUTOMATIC:
				Request.SetBackfillMode(Aws::GameLift::Model::BackfillMode::AUTOMATIC);
				break;
			case EGameLiftBackfillMode::MANUAL:
				Request.SetBackfillMode(Aws::GameLift::Model::BackfillMode::MANUAL);
				break;
			}
		}

		if (!CustomEventData.IsEmpty()) {
			Request.SetCustomEventData(TCHAR_TO_UTF8(*CustomEventData));
		}

		if (!Description.IsEmpty()) {
			Request.SetDescription(TCHAR_TO_UTF8(*Description));
		}

		for (FGameLiftMap p : GameProperties) {
			Aws::GameLift::Model::GameProperty Item;
			Item.SetKey(TCHAR_TO_UTF8(*p.Key));
			Item.SetValue(TCHAR_TO_UTF8(*p.Value));
			Request.AddGameProperties(Item);
		}

		if (!GameSessionData.IsEmpty()) {
			Request.SetGameSessionData(TCHAR_TO_UTF8(*GameSessionData));
		}

		if (!NotificationTarget.IsEmpty()) {
			Request.SetNotificationTarget(TCHAR_TO_UTF8(*NotificationTarget));
		}

		Aws::GameLift::UpdateMatchmakingConfigurationResponseReceivedHandler Handler;
		Handler = std::bind(&UGameLiftUpdateMatchmakingConfigurationAsync::OnUpdateMatchmakingConfigurationState, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

		LOG_NORMAL("Requesting to create matchmaking configuration")
			GameLiftClient->UpdateMatchmakingConfigurationAsync(Request, Handler);
	}
	else
	{
		const FString MyErrorMessage = "Game lift object is null";
		LOG_ERROR("Received UpdateMatchmakingConfiguration with failed outcome. Error: " + MyErrorMessage);
	}
#else
const FString MyErrorMessage = "no GameLiftSDK activated";
LOG_ERROR("Received UpdateMatchmakingConfiguration with failed outcome. Error: " + MyErrorMessage);
#endif
}

void UGameLiftUpdateMatchmakingConfigurationAsync::OnUpdateMatchmakingConfigurationState(const Aws::GameLift::GameLiftClient * Client, const Aws::GameLift::Model::UpdateMatchmakingConfigurationRequest & Request, const Aws::GameLift::Model::UpdateMatchmakingConfigurationOutcome & Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context)
{
#if WITH_GAMELIFTCLIENTSDK
	if (Outcome.IsSuccess())
	{
		Aws::GameLift::Model::MatchmakingConfiguration MatchmakingConfiguration = Outcome.GetResult().GetConfiguration();
		LOG_NORMAL("Received OnUpdateMatchmakingConfiguration with Success outcome.");
		Success.Broadcast(UGameLiftClientHelper::ParseMatchmakingConfiguration(&MatchmakingConfiguration), FString());
	}
	else
	{
		const FString MyErrorMessage = FString(Outcome.GetError().GetMessage().c_str());
		LOG_ERROR("Received OnUpdateMatchmakingConfiguration with failed outcome. Error: " + MyErrorMessage);
		Failed.Broadcast(FGameLiftMatchmakingConfiguration(), MyErrorMessage);
	}
#endif
}

UGameLiftDeleteMatchmakingConfigurationAsync * UGameLiftDeleteMatchmakingConfigurationAsync::DeleteMatchmakingConfiguration(const UObject * WorldContextObject, const UGameLiftClientObject * GameLiftClientObject, FString Name)
{
#if WITH_GAMELIFTCLIENTSDK
	UGameLiftDeleteMatchmakingConfigurationAsync* Proxy = NewObject<UGameLiftDeleteMatchmakingConfigurationAsync>();
	Proxy->WorldContextObject = WorldContextObject;
	if (GameLiftClientObject) {
		Proxy->GameLiftClient = GameLiftClientObject->GameLiftClient;
	}
	Proxy->Name = Name;
	return Proxy;
#endif
	return nullptr;
}

void UGameLiftDeleteMatchmakingConfigurationAsync::Activate()
{
#if WITH_GAMELIFTCLIENTSDK
	if (GameLiftClient)
	{
		LOG_NORMAL("Preparing to delete matchmaking configuration...");

		Aws::GameLift::Model::DeleteMatchmakingConfigurationRequest Request;

		Request.SetName(TCHAR_TO_UTF8(*Name));


		Aws::GameLift::DeleteMatchmakingConfigurationResponseReceivedHandler Handler;
		Handler = std::bind(&UGameLiftDeleteMatchmakingConfigurationAsync::OnDeleteMatchmakingConfigurationState, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

		LOG_NORMAL("Requesting to create matchmaking configuration")
			GameLiftClient->DeleteMatchmakingConfigurationAsync(Request, Handler);
	}
	else
	{
		const FString MyErrorMessage = "Game lift object is null";
		LOG_ERROR("Received DeleteMatchmakingConfiguration with failed outcome. Error: " + MyErrorMessage);
	}
#else
	const FString MyErrorMessage = "no GameLiftSDK activated";
	LOG_ERROR("Received DeleteMatchmakingConfiguration with failed outcome. Error: " + MyErrorMessage);
#endif
}

void UGameLiftDeleteMatchmakingConfigurationAsync::OnDeleteMatchmakingConfigurationState(const Aws::GameLift::GameLiftClient * Client, const Aws::GameLift::Model::DeleteMatchmakingConfigurationRequest & Request, const Aws::GameLift::Model::DeleteMatchmakingConfigurationOutcome & Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context)
{
#if WITH_GAMELIFTCLIENTSDK
	if (Outcome.IsSuccess())
	{
		LOG_NORMAL("Received OnDeleteMatchmakingConfiguration with Success outcome.");
		Success.Broadcast(FString());
	}
	else
	{
		const FString MyErrorMessage = FString(Outcome.GetError().GetMessage().c_str());
		LOG_ERROR("Received OnDeleteMatchmakingConfiguration with failed outcome. Error: " + MyErrorMessage);
		Failed.Broadcast(MyErrorMessage);
	}
#endif
}

UGameLiftCreateMatchmakingRuleSetAsync * UGameLiftCreateMatchmakingRuleSetAsync::CreateMatchmakingRuleSet(const UObject * WorldContextObject, const UGameLiftClientObject * GameLiftClientObject, FString Name, FString RuleSetBody)
{
#if WITH_GAMELIFTCLIENTSDK
	UGameLiftCreateMatchmakingRuleSetAsync* Proxy = NewObject<UGameLiftCreateMatchmakingRuleSetAsync>();
	Proxy->WorldContextObject = WorldContextObject;
	if (GameLiftClientObject) {
		Proxy->GameLiftClient = GameLiftClientObject->GameLiftClient;
	}
	Proxy->Name = Name;
	Proxy->RuleSetBody = RuleSetBody;
	return Proxy;
#endif
	return nullptr;
}

void UGameLiftCreateMatchmakingRuleSetAsync::Activate()
{
#if WITH_GAMELIFTCLIENTSDK
	if (GameLiftClient)
	{
		LOG_NORMAL("Preparing to create matchmaking configuration...");

		Aws::GameLift::Model::CreateMatchmakingRuleSetRequest Request;

		Request.SetName(TCHAR_TO_UTF8(*Name));
		Request.SetRuleSetBody(TCHAR_TO_UTF8(*RuleSetBody));

		Aws::GameLift::CreateMatchmakingRuleSetResponseReceivedHandler Handler;
		Handler = std::bind(&UGameLiftCreateMatchmakingRuleSetAsync::OnCreateMatchmakingRuleSetState, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

		LOG_NORMAL("Requesting to create matchmaking configuration")
			GameLiftClient->CreateMatchmakingRuleSetAsync(Request, Handler);
	}
	else
	{
		const FString MyErrorMessage = "Game lift object is null";
		LOG_ERROR("Received CreateMatchmakingRuleSet with failed outcome. Error: " + MyErrorMessage);
	}
#else
	const FString MyErrorMessage = "no GameLiftSDK activated";
	LOG_ERROR("Received CreateMatchmakingRuleSet with failed outcome. Error: " + MyErrorMessage);
#endif
}

void UGameLiftCreateMatchmakingRuleSetAsync::OnCreateMatchmakingRuleSetState(const Aws::GameLift::GameLiftClient * Client, const Aws::GameLift::Model::CreateMatchmakingRuleSetRequest & Request, const Aws::GameLift::Model::CreateMatchmakingRuleSetOutcome & Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context)
{
#if WITH_GAMELIFTCLIENTSDK
	if (Outcome.IsSuccess())
	{
		Aws::GameLift::Model::MatchmakingRuleSet RuleSet = Outcome.GetResult().GetRuleSet();
		LOG_NORMAL("Received OnCreateMatchmakingRuleSet with Success outcome.");
		Success.Broadcast(UGameLiftClientHelper::ParseMatchmakingRuleSet(&RuleSet), FString());
	}
	else
	{
		const FString MyErrorMessage = FString(Outcome.GetError().GetMessage().c_str());
		LOG_ERROR("Received OnCreateMatchmakingRuleSet with failed outcome. Error: " + MyErrorMessage);
		Failed.Broadcast(FGameLiftMatchmakingRuleSet(), MyErrorMessage);
	}
#endif
}

UGameLiftDescribeMatchmakingRuleSetsAsync * UGameLiftDescribeMatchmakingRuleSetsAsync::DescribeMatchmakingRuleSets(const UObject * WorldContextObject, const UGameLiftClientObject * GameLiftClientObject, int Limit, TArray<FString> Names, FString NextToken)
{
#if WITH_GAMELIFTCLIENTSDK
	UGameLiftDescribeMatchmakingRuleSetsAsync* Proxy = NewObject<UGameLiftDescribeMatchmakingRuleSetsAsync>();
	Proxy->WorldContextObject = WorldContextObject;
	if (GameLiftClientObject) {
		Proxy->GameLiftClient = GameLiftClientObject->GameLiftClient;
	}
	Proxy->Limit = Limit;
	Proxy->Names = Names;
	Proxy->NextToken = NextToken;
	return Proxy;
#endif
	return nullptr;
}

void UGameLiftDescribeMatchmakingRuleSetsAsync::Activate()
{
#if WITH_GAMELIFTCLIENTSDK
	if (GameLiftClient)
	{
		LOG_NORMAL("Preparing to describe matchmaking rule sets...");

		Aws::GameLift::Model::DescribeMatchmakingRuleSetsRequest Request;

		if (Limit > 0) {
			Request.SetLimit(Limit);
		}

		for (FString Name : Names) {
			Request.AddNames(TCHAR_TO_UTF8(*Name));
		}

		if (!NextToken.IsEmpty()) {
			Request.SetNextToken(TCHAR_TO_UTF8(*NextToken));
		}

		Aws::GameLift::DescribeMatchmakingRuleSetsResponseReceivedHandler Handler;
		Handler = std::bind(&UGameLiftDescribeMatchmakingRuleSetsAsync::OnDescribeMatchmakingRuleSetsState, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

		LOG_NORMAL("Requesting to describe matchmaking rule sets")
			GameLiftClient->DescribeMatchmakingRuleSetsAsync(Request, Handler);
	}
	else
	{
		const FString MyErrorMessage = "Game lift object is null";
		LOG_ERROR("Received DescribeMatchmakingRuleSets with failed outcome. Error: " + MyErrorMessage);
	}
#else
	const FString MyErrorMessage = "no GameLiftSDK activated";
	LOG_ERROR("Received DescribeMatchmakingRuleSets with failed outcome. Error: " + MyErrorMessage);
#endif
}

void UGameLiftDescribeMatchmakingRuleSetsAsync::OnDescribeMatchmakingRuleSetsState(const Aws::GameLift::GameLiftClient * Client, const Aws::GameLift::Model::DescribeMatchmakingRuleSetsRequest & Request, const Aws::GameLift::Model::DescribeMatchmakingRuleSetsOutcome & Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context)
{
#if WITH_GAMELIFTCLIENTSDK
	TArray<FGameLiftMatchmakingRuleSet> RuleSets;
	if (Outcome.IsSuccess())
	{
		for (auto It = Outcome.GetResult().GetRuleSets().begin(); It != Outcome.GetResult().GetRuleSets().end(); ++It) {
			RuleSets.Add(UGameLiftClientHelper::ParseMatchmakingRuleSet(It._Ptr));
		}
		LOG_NORMAL("Received OnDescribeMatchmakingRuleSets with Success outcome.");
		Success.Broadcast(RuleSets, FString());
	}
	else
	{
		const FString MyErrorMessage = FString(Outcome.GetError().GetMessage().c_str());
		LOG_ERROR("Received OnDescribeMatchmakingRuleSets with failed outcome. Error: " + MyErrorMessage);
		Failed.Broadcast(RuleSets, MyErrorMessage);
	}
#endif
}

UGameLiftValidateMatchmakingRuleSetAsync * UGameLiftValidateMatchmakingRuleSetAsync::ValidateMatchmakingRuleSet(const UObject * WorldContextObject, const UGameLiftClientObject * GameLiftClientObject, FString RuleSetBody)
{
#if WITH_GAMELIFTCLIENTSDK
	UGameLiftValidateMatchmakingRuleSetAsync* Proxy = NewObject<UGameLiftValidateMatchmakingRuleSetAsync>();
	Proxy->WorldContextObject = WorldContextObject;
	if (GameLiftClientObject) {
		Proxy->GameLiftClient = GameLiftClientObject->GameLiftClient;
	}
	Proxy->RuleSetBody = RuleSetBody;
	return Proxy;
#endif
	return nullptr;
}

void UGameLiftValidateMatchmakingRuleSetAsync::Activate()
{
#if WITH_GAMELIFTCLIENTSDK
	if (GameLiftClient)
	{
		LOG_NORMAL("Preparing to validate matchmaking configuration...");

		Aws::GameLift::Model::ValidateMatchmakingRuleSetRequest Request;

		Request.SetRuleSetBody(TCHAR_TO_UTF8(*RuleSetBody));

		Aws::GameLift::ValidateMatchmakingRuleSetResponseReceivedHandler Handler;
		Handler = std::bind(&UGameLiftValidateMatchmakingRuleSetAsync::OnValidateMatchmakingRuleSetState, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

		LOG_NORMAL("Requesting to validate matchmaking configuration")
			GameLiftClient->ValidateMatchmakingRuleSetAsync(Request, Handler);
	}
	else
	{
		const FString MyErrorMessage = "Game lift object is null";
		LOG_ERROR("Received ValidateMatchmakingRuleSet with failed outcome. Error: " + MyErrorMessage);
	}
#else
	const FString MyErrorMessage = "no GameLiftSDK activated";
	LOG_ERROR("Received ValidateMatchmakingRuleSet with failed outcome. Error: " + MyErrorMessage);
#endif
}

void UGameLiftValidateMatchmakingRuleSetAsync::OnValidateMatchmakingRuleSetState(const Aws::GameLift::GameLiftClient * Client, const Aws::GameLift::Model::ValidateMatchmakingRuleSetRequest & Request, const Aws::GameLift::Model::ValidateMatchmakingRuleSetOutcome & Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context)
{
#if WITH_GAMELIFTCLIENTSDK
	if (Outcome.IsSuccess())
	{
		LOG_NORMAL("Received OnDescribeMatchmakingRuleSets with Success outcome.");
		Success.Broadcast(Outcome.GetResult().GetValid(), FString());
	}
	else
	{
		const FString MyErrorMessage = FString(Outcome.GetError().GetMessage().c_str());
		LOG_ERROR("Received OnDescribeMatchmakingRuleSets with failed outcome. Error: " + MyErrorMessage);
		Failed.Broadcast(false, MyErrorMessage);
	}
#endif
}

UGameLiftDeleteMatchmakingRuleSetAsync * UGameLiftDeleteMatchmakingRuleSetAsync::DeleteMatchmakingRuleSet(const UObject * WorldContextObject, const UGameLiftClientObject * GameLiftClientObject, FString Name)
{
#if WITH_GAMELIFTCLIENTSDK
	UGameLiftDeleteMatchmakingRuleSetAsync* Proxy = NewObject<UGameLiftDeleteMatchmakingRuleSetAsync>();
	Proxy->WorldContextObject = WorldContextObject;
	if (GameLiftClientObject) {
		Proxy->GameLiftClient = GameLiftClientObject->GameLiftClient;
	}
	Proxy->Name = Name;
	return Proxy;
#endif
	return nullptr;
}

void UGameLiftDeleteMatchmakingRuleSetAsync::Activate()
{
#if WITH_GAMELIFTCLIENTSDK
	if (GameLiftClient)
	{
		LOG_NORMAL("Preparing to delete matchmaking rule set...");

		Aws::GameLift::Model::DeleteMatchmakingRuleSetRequest Request;

		Request.SetName(TCHAR_TO_UTF8(*Name));


		Aws::GameLift::DeleteMatchmakingRuleSetResponseReceivedHandler Handler;
		Handler = std::bind(&UGameLiftDeleteMatchmakingRuleSetAsync::OnDeleteMatchmakingRuleSetState, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

		LOG_NORMAL("Requesting to create matchmaking rule set")
			GameLiftClient->DeleteMatchmakingRuleSetAsync(Request, Handler);
	}
	else
	{
		const FString MyErrorMessage = "Game lift object is null";
		LOG_ERROR("Received DeleteMatchmakingRuleSet with failed outcome. Error: " + MyErrorMessage);
	}
#else
	const FString MyErrorMessage = "no GameLiftSDK activated";
	LOG_ERROR("Received DeleteMatchmakingRuleSet with failed outcome. Error: " + MyErrorMessage);
#endif
}

void UGameLiftDeleteMatchmakingRuleSetAsync::OnDeleteMatchmakingRuleSetState(const Aws::GameLift::GameLiftClient * Client, const Aws::GameLift::Model::DeleteMatchmakingRuleSetRequest & Request, const Aws::GameLift::Model::DeleteMatchmakingRuleSetOutcome & Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context)
{
#if WITH_GAMELIFTCLIENTSDK
	if (Outcome.IsSuccess())
	{
		LOG_NORMAL("Received OnDeleteMatchmakingRuleSet with Success outcome.");
		Success.Broadcast(FString());
	}
	else
	{
		const FString MyErrorMessage = FString(Outcome.GetError().GetMessage().c_str());
		LOG_ERROR("Received OnDeleteMatchmakingRuleSet with failed outcome. Error: " + MyErrorMessage);
		Failed.Broadcast(MyErrorMessage);
	}
#endif
}
