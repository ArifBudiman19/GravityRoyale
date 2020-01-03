#define USE_IMPORT_EXPORT

#include "GameLiftClientGameSession.h"
#include "GameLiftClientObject.h"
#include "GameLiftClientGlobals.h"
#include "GameLiftClientHelper.h"
#if WITH_GAMELIFTCLIENTSDK
#include "aws/core/utils/Outcome.h"
#include "aws/gamelift/GameLiftClient.h"
#include "aws/gamelift/model/CreateGameSessionRequest.h"
#include "aws/gamelift/model/GameProperty.h"
#include "aws/gamelift/model/DescribeGameSessionDetailsRequest.h"
#include "aws/gamelift/model/DescribeGameSessionsRequest.h"
#include "aws/gamelift/model/SearchGameSessionsRequest.h"
#include "aws/gamelift/model/UpdateGameSessionRequest.h"
#endif

UGameLiftCreateGameSessionAsync * UGameLiftCreateGameSessionAsync::CreateGameSession(const UObject * WorldContextObject, const UGameLiftClientObject * GameLiftClientObject, FGameLiftGameSessionConfig GameSessionProperties)
{
#if WITH_GAMELIFTCLIENTSDK
	UGameLiftCreateGameSessionAsync* Proxy = NewObject<UGameLiftCreateGameSessionAsync>();
	Proxy->WorldContextObject = WorldContextObject;
	if (GameLiftClientObject) {
		Proxy->GameLiftClient = GameLiftClientObject->GameLiftClient;
		Proxy->bIsUsingGameLiftLocal = GameLiftClientObject->bIsUsingGameLiftLocal;
	}
	Proxy->SessionConfig = GameSessionProperties;
	return Proxy;
#endif
	return nullptr;
}

void UGameLiftCreateGameSessionAsync::Activate()
{
#if WITH_GAMELIFTCLIENTSDK
	if (GameLiftClient)
	{
		LOG_NORMAL("Preparing to create game session...");
		Aws::GameLift::Model::CreateGameSessionRequest GameSessionRequest;

		GameSessionRequest.SetMaximumPlayerSessionCount(SessionConfig.GetMaxPlayers());

		if (bIsUsingGameLiftLocal)
		{
			GameSessionRequest.SetFleetId(TCHAR_TO_UTF8(*SessionConfig.GetGameLiftLocalFleetID()));
		}
		else
		{
			GameSessionRequest.SetAliasId(TCHAR_TO_UTF8(*SessionConfig.GetAliasID()));
		}

		if (!SessionConfig.GetName().IsEmpty()) {
			GameSessionRequest.SetName(TCHAR_TO_UTF8(*SessionConfig.GetName()));
		}

		if (!SessionConfig.GetGameSessionData().IsEmpty()) {
			GameSessionRequest.SetGameSessionData(TCHAR_TO_UTF8(*SessionConfig.GetGameSessionData()));
		}

		LOG_NORMAL("Setting Alias ID: " + SessionConfig.GetAliasID());
		for (FGameLiftMap ServerSetting : SessionConfig.GetGameSessionProperties())
		{
			LOG_NORMAL("********************************************************************");
			Aws::GameLift::Model::GameProperty MapProperty;
			MapProperty.SetKey(TCHAR_TO_UTF8(*ServerSetting.Key));
			MapProperty.SetValue(TCHAR_TO_UTF8(*ServerSetting.Value));
			GameSessionRequest.AddGameProperties(MapProperty);
			LOG_NORMAL(FString::Printf(TEXT("New GameProperty added. Key: (%s) Value: (%s)"), *ServerSetting.Key, *ServerSetting.Value));
		}

		Aws::GameLift::CreateGameSessionResponseReceivedHandler Handler;

		Handler = std::bind(&UGameLiftCreateGameSessionAsync::OnCreateGameSession, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

		LOG_NORMAL("Starting game session...");
		GameLiftClient->CreateGameSessionAsync(GameSessionRequest, Handler);
	}
	else
	{
		const FString MyErrorMessage = "Game lift object is null";
		LOG_ERROR("Received OnCreateGameSession with failed outcome. Error: " + MyErrorMessage);
	}
	
#else
	const FString MyErrorMessage = "no GameLiftSDK activated";
	LOG_ERROR("Received OnCreateGameSession with failed outcome. Error: " + MyErrorMessage);
#endif
}

void UGameLiftCreateGameSessionAsync::OnCreateGameSession(const Aws::GameLift::GameLiftClient * Client, const Aws::GameLift::Model::CreateGameSessionRequest & Request, const Aws::GameLift::Model::CreateGameSessionOutcome & Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context)
{
#if WITH_GAMELIFTCLIENTSDK
	if (Outcome.IsSuccess())
	{
		Aws::GameLift::Model::GameSession Session = Outcome.GetResult().GetGameSession();
		FGameLiftGameSession GameSession = UGameLiftClientHelper::ParseGameSession(&Session);
		LOG_NORMAL("Received OnCreateGameSession with Success outcome.");
		Success.Broadcast(GameSession,FString());
	}
	else
	{
		const FString MyErrorMessage = FString(Outcome.GetError().GetMessage().c_str());
		LOG_ERROR("Received OnCreateGameSession with failed outcome. Error: " + MyErrorMessage);
		Failed.Broadcast(FGameLiftGameSession(),MyErrorMessage);
	}
#endif
}

UGameLiftDescribeGameSessionDetailsAsync * UGameLiftDescribeGameSessionDetailsAsync::DescribeGameSessionDetails(const UObject * WorldContextObject, const UGameLiftClientObject * GameLiftClientObject, FString GameSessionID, FString AliasID, int Limit, FString NextToken, FString StatusFillter)
{
#if WITH_GAMELIFTCLIENTSDK
	UGameLiftDescribeGameSessionDetailsAsync* Proxy = NewObject<UGameLiftDescribeGameSessionDetailsAsync>();
	if (GameLiftClientObject) {
		Proxy->GameLiftClient = GameLiftClientObject->GameLiftClient;	
	}

	Proxy->WorldContextObject = WorldContextObject;
	Proxy->SessionID = GameSessionID;
	Proxy->AliasID = AliasID;
	Proxy->Limit = Limit;
	Proxy->NextToken = NextToken;
	Proxy->StatusFillter = StatusFillter;
	return Proxy;
#endif
	return nullptr;
}

void UGameLiftDescribeGameSessionDetailsAsync::Activate()
{
#if WITH_GAMELIFTCLIENTSDK
	if (GameLiftClient)
	{
		LOG_NORMAL("Preparing to describe game session details...");

		Aws::GameLift::Model::DescribeGameSessionDetailsRequest Request;

		if (!SessionID.IsEmpty()) {
			Request.SetGameSessionId(TCHAR_TO_UTF8(*SessionID));
		}

		if (!AliasID.IsEmpty()) {
			Request.SetAliasId(TCHAR_TO_UTF8(*AliasID));
		}

		if (Limit > 0) {
			Request.SetLimit(Limit);
		}

		if (!StatusFillter.IsEmpty()) {
			Request.SetStatusFilter(TCHAR_TO_UTF8(*StatusFillter.ToUpper()));
		}

		if (!NextToken.IsEmpty()) {
			Request.SetNextToken(TCHAR_TO_UTF8(*NextToken));
		}

		Aws::GameLift::DescribeGameSessionDetailsResponseReceivedHandler Handler;
		Handler = std::bind(&UGameLiftDescribeGameSessionDetailsAsync::OnDescribeGameSessionState, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

		LOG_NORMAL("Requesting to describe game session details with ID: " + SessionID);
		GameLiftClient->DescribeGameSessionDetailsAsync(Request, Handler);
	}
	else
	{
		const FString MyErrorMessage = "Game lift object is null";
		LOG_ERROR("Received OnDescribeGameSessionDetails with failed outcome. Error: " + MyErrorMessage);
	}
#else
	const FString MyErrorMessage = "no GameLiftSDK activated";
	LOG_ERROR("Received OnCDescribeGameSessionDetails with failed outcome. Error: " + MyErrorMessage);
#endif
}

void UGameLiftDescribeGameSessionDetailsAsync::OnDescribeGameSessionState(const Aws::GameLift::GameLiftClient * Client, const Aws::GameLift::Model::DescribeGameSessionDetailsRequest & Request, const Aws::GameLift::Model::DescribeGameSessionDetailsOutcome & Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context)
{
#if WITH_GAMELIFTCLIENTSDK
	if (Outcome.IsSuccess())
	{
		FString Next = Outcome.GetResult().GetNextToken().c_str();
		TArray<FGameLiftGameSessionDetail> GameSessionDetails;

		for (auto it = Outcome.GetResult().GetGameSessionDetails().begin(); it != Outcome.GetResult().GetGameSessionDetails().end(); ++it) {
			FGameLiftGameSessionDetail Detail;
			Aws::GameLift::Model::GameSession Session = it->GetGameSession();
			Detail.GameSession = UGameLiftClientHelper::ParseGameSession(&Session);

			switch (Outcome.GetResult().GetGameSessionDetails().data()->GetProtectionPolicy())
			{
			case Aws::GameLift::Model::ProtectionPolicy::NOT_SET:
				Detail.ProtectionPolicy = EGameLiftProtectionPolicy::NOT_SET;
				break;
			case Aws::GameLift::Model::ProtectionPolicy::NoProtection:
				Detail.ProtectionPolicy = EGameLiftProtectionPolicy::NoProtection;
				break;
			case Aws::GameLift::Model::ProtectionPolicy::FullProtection:
				Detail.ProtectionPolicy = EGameLiftProtectionPolicy::FullProtection;
				break;
			default:
				break;
			}

			GameSessionDetails.Add(Detail);
		}

		LOG_NORMAL("Received OnDescribeGameSessionDetails with Success outcome.");
		Success.Broadcast(GameSessionDetails, Next, FString());
	}
	else
	{
		TArray<FGameLiftGameSessionDetail> GameSessionDetails;
		const FString MyErrorMessage = FString(Outcome.GetError().GetMessage().c_str());
		LOG_ERROR("Received OnDescribeGameSessionDetails with failed outcome. Error: " + MyErrorMessage);
		Failed.Broadcast(GameSessionDetails, FString(), MyErrorMessage);
	}
#endif
}

UGameLiftDescribeGameSessionsAsync * UGameLiftDescribeGameSessionsAsync::DescribeGameSessions(const UObject * WorldContextObject, const UGameLiftClientObject * GameLiftClientObject, FString GameSessionID, FString AliasID, int Limit, FString NextToken, FString StatusFillter)
{
#if WITH_GAMELIFTCLIENTSDK
	UGameLiftDescribeGameSessionsAsync* Proxy = NewObject<UGameLiftDescribeGameSessionsAsync>();
	if (GameLiftClientObject) {
		Proxy->GameLiftClient = GameLiftClientObject->GameLiftClient;
	}

	Proxy->WorldContextObject = WorldContextObject;
	Proxy->SessionID = GameSessionID;
	Proxy->AliasID = AliasID;
	Proxy->Limit = Limit;
	Proxy->NextToken = NextToken;
	Proxy->StatusFillter = StatusFillter;
	return Proxy;
#endif
	return nullptr;
}

void UGameLiftDescribeGameSessionsAsync::Activate()
{
#if WITH_GAMELIFTCLIENTSDK
	if (GameLiftClient)
	{
		LOG_NORMAL("Preparing to describe game sessions...");

		Aws::GameLift::Model::DescribeGameSessionsRequest Request;

		if (!SessionID.IsEmpty()) {
			Request.SetGameSessionId(TCHAR_TO_UTF8(*SessionID));
		}

		if (!AliasID.IsEmpty()) {
			Request.SetAliasId(TCHAR_TO_UTF8(*AliasID));
		}

		if (Limit > 0) {
			Request.SetLimit(Limit);
		}

		if (!StatusFillter.IsEmpty()) {
			Request.SetStatusFilter(TCHAR_TO_UTF8(*StatusFillter.ToUpper()));
		}

		if (!NextToken.IsEmpty()) {
			Request.SetNextToken(TCHAR_TO_UTF8(*NextToken));
		}

		Aws::GameLift::DescribeGameSessionsResponseReceivedHandler Handler;
		Handler = std::bind(&UGameLiftDescribeGameSessionsAsync::OnDescribeGameSessionState, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

		LOG_NORMAL("Requesting to describe game session details with ID: " + SessionID);
		GameLiftClient->DescribeGameSessionsAsync(Request, Handler);
	}
	else
	{
		const FString MyErrorMessage = "Game lift object is null";
		LOG_ERROR("Received OnDescribeGameSessions with failed outcome. Error: " + MyErrorMessage);
	}
#else
	const FString MyErrorMessage = "no GameLiftSDK activated";
	LOG_ERROR("Received OnDescribeGameSessions with failed outcome. Error: " + MyErrorMessage);
#endif
}

void UGameLiftDescribeGameSessionsAsync::OnDescribeGameSessionState(const Aws::GameLift::GameLiftClient * Client, const Aws::GameLift::Model::DescribeGameSessionsRequest & Request, const Aws::GameLift::Model::DescribeGameSessionsOutcome & Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context)
{
#if WITH_GAMELIFTCLIENTSDK
	if (Outcome.IsSuccess())
	{
		FString Next = Outcome.GetResult().GetNextToken().c_str();
		TArray<FGameLiftGameSession> GameSessions;

		for (auto it = Outcome.GetResult().GetGameSessions().begin(); it != Outcome.GetResult().GetGameSessions().end(); ++it) {
			GameSessions.Add(UGameLiftClientHelper::ParseGameSession(it._Ptr));
		}

		LOG_NORMAL("Received OnDescribeGameSessions with Success outcome.");
		Success.Broadcast(GameSessions, Next, FString());
	}
	else
	{
		TArray<FGameLiftGameSession> GameSessions;
		const FString MyErrorMessage = FString(Outcome.GetError().GetMessage().c_str());
		LOG_ERROR("Received OnDescribeGameSessions with failed outcome. Error: " + MyErrorMessage);
		Failed.Broadcast(GameSessions, FString(), MyErrorMessage);
	}
#endif
}

UGameLiftSearchGameSessionsAsync * UGameLiftSearchGameSessionsAsync::SearchGameSessions(const UObject * WorldContextObject, const UGameLiftClientObject * GameLiftClientObject, FString AliasID, FString FillterExpresion, int Limit, FString NextToken, FString SortExpression)
{
#if WITH_GAMELIFTCLIENTSDK
	UGameLiftSearchGameSessionsAsync* Proxy = NewObject<UGameLiftSearchGameSessionsAsync>();
	if (GameLiftClientObject) {
		Proxy->GameLiftClient = GameLiftClientObject->GameLiftClient;
	}

	Proxy->Active = false;
	Proxy->WorldContextObject = WorldContextObject;
	Proxy->FillterExpression = FillterExpresion;
	Proxy->AliasID = AliasID;
	Proxy->Limit = Limit;
	Proxy->NextToken = NextToken;
	Proxy->SortExpression = SortExpression;
	return Proxy;
#endif
	return nullptr;
}

void UGameLiftSearchGameSessionsAsync::Activate()
{
	if (WorldContextObject == nullptr) {
		return;
	}

	if (Active) {
		return;
	}


#if WITH_GAMELIFTCLIENTSDK
	if (GameLiftClient)
	{
		LOG_NORMAL("Preparing to describe game sessions...");

		Aws::GameLift::Model::SearchGameSessionsRequest Request;

		if (!FillterExpression.IsEmpty()) {
			Request.SetFilterExpression(TCHAR_TO_UTF8(*FillterExpression));
		}

		if (!AliasID.IsEmpty()) {
			Request.SetAliasId(TCHAR_TO_UTF8(*AliasID));
		}

		if (Limit > 0) {
			Request.SetLimit(Limit);
		}

		if (!SortExpression.IsEmpty()) {
			Request.SetSortExpression(TCHAR_TO_UTF8(*SortExpression));
		}

		if (!NextToken.IsEmpty()) {
			Request.SetNextToken(TCHAR_TO_UTF8(*NextToken));
		}

		Active = true;

		Aws::GameLift::SearchGameSessionsResponseReceivedHandler Handler;
		Handler = std::bind(&UGameLiftSearchGameSessionsAsync::OnSearchGameSessionState, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

		LOG_NORMAL("Requesting to search game sessions")
		GameLiftClient->SearchGameSessionsAsync(Request, Handler);
	}
	else
	{
		const FString MyErrorMessage = "Game lift object is null";
		LOG_ERROR("Received OnSearchGameSessions with failed outcome. Error: " + MyErrorMessage);
	}
#else
	const FString MyErrorMessage = "no GameLiftSDK activated";
	LOG_ERROR("Received OnSearchGameSessions with failed outcome. Error: " + MyErrorMessage);
#endif
}

void UGameLiftSearchGameSessionsAsync::OnSearchGameSessionState(const Aws::GameLift::GameLiftClient * Client, const Aws::GameLift::Model::SearchGameSessionsRequest & Request, const Aws::GameLift::Model::SearchGameSessionsOutcome & Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context)
{
#if WITH_GAMELIFTCLIENTSDK

	Active = false;

	if (Outcome.IsSuccess())
	{
		FString Next = Outcome.GetResult().GetNextToken().c_str();
		TArray<FGameLiftGameSession> GameSessions;

		for (auto it = Outcome.GetResult().GetGameSessions().begin(); it != Outcome.GetResult().GetGameSessions().end(); ++it) {
			GameSessions.Add(UGameLiftClientHelper::ParseGameSession(it._Ptr));
		}

		LOG_NORMAL("Received OnSearchGameSessions with Success outcome.");
		Success.Broadcast(GameSessions, Next, FString());
	}
	else
	{
		TArray<FGameLiftGameSession> GameSessions;
		const FString MyErrorMessage = FString(Outcome.GetError().GetMessage().c_str());
		LOG_ERROR("Received OnSearchGameSessions with failed outcome. Error: " + MyErrorMessage);
		Failed.Broadcast(GameSessions, FString(), MyErrorMessage);
	}
#endif
}


UGameLiftUpdateGameSessionAsync * UGameLiftUpdateGameSessionAsync::UpdateGameSession(const UObject * WorldContextObject, const UGameLiftClientObject * GameLiftClientObject, FString SessionID, FString Name, EGameLiftPlayerSessionCreationPolicy PlayerSessionCreationPolicy, EGameLiftProtectionPolicy ProtectionPolicy, int MaximumPlayerSessionCount)
{
#if WITH_GAMELIFTCLIENTSDK
	UGameLiftUpdateGameSessionAsync* Proxy = NewObject<UGameLiftUpdateGameSessionAsync>();
	if (GameLiftClientObject) {
		Proxy->GameLiftClient = GameLiftClientObject->GameLiftClient;
	}

	Proxy->WorldContextObject = WorldContextObject;
	Proxy->SessionID = SessionID;
	Proxy->Name = Name;
	Proxy->PlayerSessionCreationPolicy = PlayerSessionCreationPolicy;
	Proxy->ProtectionPolicy = ProtectionPolicy;
	return Proxy;
#endif
	return nullptr;
}

void UGameLiftUpdateGameSessionAsync::Activate()
{
#if WITH_GAMELIFTCLIENTSDK
	if (GameLiftClient)
	{
		LOG_NORMAL("Preparing to update game sessions...");

		Aws::GameLift::Model::UpdateGameSessionRequest Request;

		Request.SetGameSessionId(TCHAR_TO_UTF8(*SessionID));

		if (MaximumPlayerSessionCount > 0) {
			Request.SetMaximumPlayerSessionCount(MaximumPlayerSessionCount);
		}

		if (!Name.IsEmpty()) {
			Request.SetName(TCHAR_TO_UTF8(*Name));
		}

		if (PlayerSessionCreationPolicy != EGameLiftPlayerSessionCreationPolicy::NOT_SET) {
			switch (PlayerSessionCreationPolicy)
			{
			case EGameLiftPlayerSessionCreationPolicy::ACCEPT_ALL:
				Request.SetPlayerSessionCreationPolicy(Aws::GameLift::Model::PlayerSessionCreationPolicy::ACCEPT_ALL);
				break;
			case EGameLiftPlayerSessionCreationPolicy::DENY_ALL:
				Request.SetPlayerSessionCreationPolicy(Aws::GameLift::Model::PlayerSessionCreationPolicy::DENY_ALL);
				break;
			default:
				break;
			}
		}

		if (ProtectionPolicy != EGameLiftProtectionPolicy::NOT_SET) {
			switch (ProtectionPolicy)
			{
			case EGameLiftProtectionPolicy::NoProtection:
				Request.SetProtectionPolicy(Aws::GameLift::Model::ProtectionPolicy::NoProtection);
				break;
			case EGameLiftProtectionPolicy::FullProtection:
				Request.SetProtectionPolicy(Aws::GameLift::Model::ProtectionPolicy::FullProtection);
				break;
			default:
				break;
			}
		}

		Aws::GameLift::UpdateGameSessionResponseReceivedHandler Handler;
		Handler = std::bind(&UGameLiftUpdateGameSessionAsync::OnUpdateGameSessionState, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
		
		LOG_NORMAL("Requesting to update game sessions")
		GameLiftClient->UpdateGameSessionAsync(Request, Handler);
	}
	else
	{
		const FString MyErrorMessage = "Game lift object is null";
		LOG_ERROR("Received OnUpdateGameSession with failed outcome. Error: " + MyErrorMessage);
	}
#else
	const FString MyErrorMessage = "no GameLiftSDK activated";
	LOG_ERROR("Received OnUpdateGameSession with failed outcome. Error: " + MyErrorMessage);
#endif
}

void UGameLiftUpdateGameSessionAsync::OnUpdateGameSessionState(const Aws::GameLift::GameLiftClient * Client, const Aws::GameLift::Model::UpdateGameSessionRequest & Request, const Aws::GameLift::Model::UpdateGameSessionOutcome & Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context)
{
#if WITH_GAMELIFTCLIENTSDK
	if (Outcome.IsSuccess())
	{
		Aws::GameLift::Model::GameSession Session = Outcome.GetResult().GetGameSession();
		FGameLiftGameSession GameSession = UGameLiftClientHelper::ParseGameSession(&Session);
		LOG_NORMAL("Received OnUpdateGameSession with Success outcome.");
		Success.Broadcast(GameSession, FString());
	}
	else
	{
		const FString MyErrorMessage = FString(Outcome.GetError().GetMessage().c_str());
		LOG_ERROR("Received OnUpdateGameSession with failed outcome. Error: " + MyErrorMessage);
		Failed.Broadcast(FGameLiftGameSession(), MyErrorMessage);
	}
#endif
}
