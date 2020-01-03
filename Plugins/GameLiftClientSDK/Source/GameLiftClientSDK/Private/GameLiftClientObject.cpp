// Fill out your copyright notice in the Description page of Project Settings.


#include "GameLiftClientObject.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#if WITH_GAMELIFTCLIENTSDK
#include "GameLiftClientGlobals.h"
#include "GameLiftClientTypes.h"
#include "GameLiftClientApi.h"
#include "aws/core/client/ClientConfiguration.h"
#include "aws/core/auth/AWSCredentials.h"
#include "aws/gamelift/GameLiftClient.h"
#endif

void UGameLiftClientObject::Internal_InitGameLiftClientSDK(const FGameLiftClientConfiguration & ClientConfig, const FGameLiftCredentials & Credentials, bool bUsingGameLiftLocal, int32 LocalPort)
{
#if WITH_GAMELIFTCLIENTSDK
	Aws::Client::ClientConfiguration Configuration;
	Aws::Auth::AWSCredentials AwsCredentials;

	bIsUsingGameLiftLocal = bUsingGameLiftLocal;

	// Scheme
	if (ClientConfig.Scheme.Equals(TEXT("https"), ESearchCase::IgnoreCase)) {
		LOG_NORMAL("Use scheme https");
		Configuration.scheme = Aws::Http::Scheme::HTTPS;
	}
	else if (ClientConfig.Scheme.Equals(TEXT("http"), ESearchCase::IgnoreCase)) {
		LOG_NORMAL("Use scheme http");
		Configuration.scheme = Aws::Http::Scheme::HTTP;
	}
	else {
		LOG_ERROR("Scheme only use HTTP or HTTPS");
	}

	Configuration.region = TCHAR_TO_UTF8(*ClientConfig.Region);
	Configuration.maxConnections = ClientConfig.MaxConnection;
	Configuration.requestTimeoutMs = ClientConfig.RequestTimeoutMs;
	Configuration.connectTimeoutMs = ClientConfig.ConnectTimeoutMs;

	if (!ClientConfig.EndpointOverride.IsEmpty()) {
		Configuration.endpointOverride = TCHAR_TO_UTF8(*ClientConfig.EndpointOverride);
		LOG_NORMAL("Client endpoint has been override");
	}

	if (!ClientConfig.ProxyHost.IsEmpty()) {
		LOG_WARNING("Setting Proxy");
		if (ClientConfig.ProxyScheme.Equals(TEXT("https"), ESearchCase::IgnoreCase)) {
			LOG_NORMAL("Use proxy scheme https");
			Configuration.proxyScheme = Aws::Http::Scheme::HTTPS;
		}
		else if (ClientConfig.ProxyScheme.Equals(TEXT("http"), ESearchCase::IgnoreCase)) {
			LOG_NORMAL("Use proxy scheme http");
			Configuration.proxyScheme = Aws::Http::Scheme::HTTP;
		}
		else {
			LOG_ERROR("Proxy scheme only use HTTP or HTTPS");
		}


		Configuration.proxyHost = TCHAR_TO_UTF8(*ClientConfig.ProxyHost);
		Configuration.proxyUserName = TCHAR_TO_UTF8(*ClientConfig.ProxyUserName);
		Configuration.proxyPassword = TCHAR_TO_UTF8(*ClientConfig.ProxyPasword);
		Configuration.proxyPort = ClientConfig.ProxyPort;
	}

	Configuration.verifySSL = ClientConfig.VerifySSL;

	if (!ClientConfig.CaFile.IsEmpty()) {
		Configuration.caFile = TCHAR_TO_UTF8(*ClientConfig.CaFile);
	}

	if (!ClientConfig.CaPath.IsEmpty()) {
		Configuration.caPath = TCHAR_TO_UTF8(*ClientConfig.CaPath);
	}

	Configuration.followRedirects = ClientConfig.FollowRedirects;
	Configuration.disableExpectHeader = ClientConfig.DisableExpectHeader;
	Configuration.enableClockSkewAdjustment = ClientConfig.EnableClockSkewAdjustment;

	if (bIsUsingGameLiftLocal)
	{
		Configuration.scheme = Aws::Http::Scheme::HTTP;
		const FString HostAddress = FString::Printf(TEXT("localhost:%i"), LocalPort);
		Configuration.endpointOverride = TCHAR_TO_UTF8(*HostAddress);
		LOG_WARNING(FString::Printf(TEXT("GameLift is currently configured to use GameLift Local at port %i."), LocalPort));
	}

	// Setting Credentials
	AwsCredentials = Aws::Auth::AWSCredentials(TCHAR_TO_UTF8(*Credentials.AccessKeyId), TCHAR_TO_UTF8(*Credentials.SecretKey));
	if (!Credentials.SessionToken.IsEmpty()) {
		LOG_NORMAL(FString::Printf(TEXT("Session Token has been set %s"), &Credentials.SessionToken));
		AwsCredentials.SetSessionToken(TCHAR_TO_UTF8(*Credentials.SessionToken));
	}

	GameLiftClient = new Aws::GameLift::GameLiftClient(AwsCredentials, Configuration);
#endif
}

UGameLiftClientObject * UGameLiftClientObject::CreateGameLiftObject(const FGameLiftClientConfiguration & ClientConfig, const FGameLiftCredentials & Credentials, bool bUsingGameLiftLocal, int32 LocalPort)
{
#if WITH_GAMELIFTCLIENTSDK
	UGameLiftClientObject* Proxy = NewObject<UGameLiftClientObject>();
	Proxy->Internal_InitGameLiftClientSDK(ClientConfig, Credentials, bUsingGameLiftLocal, LocalPort);
	return Proxy;
#endif
	return nullptr;
}

UGameLiftCreateGameSession * UGameLiftClientObject::CreateGameSession(FGameLiftGameSessionConfig GameSessionProperties)
{
#if WITH_GAMELIFTCLIENTSDK
	UGameLiftCreateGameSession* Proxy = UGameLiftCreateGameSession::CreateGameSession(GameSessionProperties, bIsUsingGameLiftLocal);
	Proxy->GameLiftClient = GameLiftClient;
	return Proxy;
#endif
	return nullptr;
}

UGameLiftDescribeGameSessionDetails * UGameLiftClientObject::DescribeGameSessionDetails(FString GameSessionID)
{
#if WITH_GAMELIFTCLIENTSDK
	UGameLiftDescribeGameSessionDetails* Proxy = UGameLiftDescribeGameSessionDetails::DescribeGameSessionDetails(GameSessionID);
	Proxy->GameLiftClient = GameLiftClient;
	return Proxy;
#endif
	return nullptr;
}

UGameLiftCreatePlayerSession * UGameLiftClientObject::CreatePlayerSession(FString GameSessionID, FString UniquePlayerID, FString PlayerData)
{
#if WITH_GAMELIFTCLIENTSDK
	UGameLiftCreatePlayerSession* Proxy = UGameLiftCreatePlayerSession::CreatePlayerSession(GameSessionID, UniquePlayerID, PlayerData);
	Proxy->GameLiftClient = GameLiftClient;
	return Proxy;
#endif
	return nullptr;
}

UGameLiftDescribeGameSessionQueues * UGameLiftClientObject::DescribeGameSessionQueues(FString QueueName)
{
#if WITH_GAMELIFTCLIENTSDK
	UGameLiftDescribeGameSessionQueues* Proxy = UGameLiftDescribeGameSessionQueues::DescribeGameSessionQueues(QueueName);
	Proxy->GameLiftClient = GameLiftClient;
	return Proxy;
#endif
	return nullptr;
}

UGameLiftSearchGameSessions * UGameLiftClientObject::SearchGameSessions(FString FleetId, FString AliasId, FString FilterExpression, FString SortExpression)
{
#if WITH_GAMELIFTCLIENTSDK
	UGameLiftSearchGameSessions* Proxy = UGameLiftSearchGameSessions::SearchGameSessions(FleetId, AliasId, FilterExpression, SortExpression);
	Proxy->GameLiftClient = GameLiftClient;
	return Proxy;
#endif
	return nullptr;
}

UGameLiftStartGameSessionPlacement * UGameLiftClientObject::StartGameSessionPlacement(FString QueueName, int MaxPlayerCount, FString PlacementId)
{
#if WITH_GAMELIFTCLIENTSDK
	UGameLiftStartGameSessionPlacement* Proxy = UGameLiftStartGameSessionPlacement::StartGameSessionPlacement(QueueName, MaxPlayerCount, PlacementId);
	Proxy->GameLiftClient = GameLiftClient;
	return Proxy;
#endif
	return nullptr;
}

UGameLiftDescribeGameSessionPlacement * UGameLiftClientObject::DescribeGameSessionPlacement(FString GameSessionPlacementId)
{
#if WITH_GAMELIFTCLIENTSDK
	UGameLiftDescribeGameSessionPlacement* Proxy = UGameLiftDescribeGameSessionPlacement::DescribeGameSessionPlacement(GameSessionPlacementId);
	Proxy->GameLiftClient = GameLiftClient;
	return Proxy;
#endif
	return nullptr;
}

