// Fill out your copyright notice in the Description page of Project Settings.


#include "GameLiftClientHelper.h"
#include "GameLiftClientTypes.h"
#if WITH_GAMELIFTCLIENTSDK
#include "aws/core/utils/DateTime.h"
#include "aws/gamelift/GameLiftClient.h"
#endif

FDateTime UGameLiftClientHelper::ParseGameLiftTime(const Aws::Utils::DateTime Time) {
	FString AwsDateTime = FString(Time.ToGmtString(Aws::Utils::DateFormat::ISO_8601).c_str());
	FDateTime DateTime;
	FDateTime::ParseIso8601(*AwsDateTime, DateTime);
	return DateTime;
}

FGameLiftGameSession UGameLiftClientHelper::ParseGameSession(Aws::GameLift::Model::GameSession * GameSession)
{
	FGameLiftGameSession Session;

	Session.CreationTime = ParseGameLiftTime(GameSession->GetCreationTime());

	Session.CreatorId = FString(GameSession->GetCreatorId().c_str());
	Session.CurrentPlayerSessionCount = GameSession->GetCurrentPlayerSessionCount();
	Session.DnsName = FString(GameSession->GetDnsName().c_str());
	Session.FleetId = FString(GameSession->GetFleetId().c_str());
	Session.GameSessionData = FString(GameSession->GetGameSessionData().c_str());
	Session.GameSessionId = FString(GameSession->GetGameSessionId().c_str());
	Session.IpAddress = FString(GameSession->GetIpAddress().c_str());
	Session.MatchmakerData = FString(GameSession->GetMatchmakerData().c_str());
	Session.MaximumPlayerSessionCount = GameSession->GetMaximumPlayerSessionCount();
	Session.Name = FString(GameSession->GetName().c_str());
	Session.Port = GameSession->GetPort();

	GameSession->GetStatusReason();

	Session.Status = ParseGameSessionStatus(GameSession->GetStatus());
	Session.PlayerSessionCreationPolicy = ParsePlayerSessionPolicy(GameSession->GetPlayerSessionCreationPolicy());
	Session.StatusReason = ParseGameSessionStatusReason(GameSession->GetStatusReason());
	Session.TerminationTime = ParseGameLiftTime(GameSession->GetTerminationTime());

	for (auto It = GameSession->GetGameProperties().begin(); It != GameSession->GetGameProperties().end(); ++It) {
		FGameLiftMap Item;
		Item.Key = It->GetKey().c_str();
		Item.Value = It->GetValue().c_str();
		Session.GameProperties.Add(Item);
	}

	return Session;
}

EGameLiftPlayerSessionCreationPolicy UGameLiftClientHelper::ParsePlayerSessionPolicy(Aws::GameLift::Model::PlayerSessionCreationPolicy Policy)
{
#if WITH_GAMELIFTCLIENTSDK
	switch (Policy)
	{
	case Aws::GameLift::Model::PlayerSessionCreationPolicy::ACCEPT_ALL: return EGameLiftPlayerSessionCreationPolicy::ACCEPT_ALL;
	case Aws::GameLift::Model::PlayerSessionCreationPolicy::DENY_ALL: return EGameLiftPlayerSessionCreationPolicy::DENY_ALL;
	default: return EGameLiftPlayerSessionCreationPolicy::NOT_SET;
	}
	checkNoEntry();
#endif
	return EGameLiftPlayerSessionCreationPolicy::NOT_SET;
}

EGameLiftGameSessionStatus UGameLiftClientHelper::ParseGameSessionStatus(Aws::GameLift::Model::GameSessionStatus Status)
{
#if WITH_GAMELIFTCLIENTSDK
	switch (Status)
	{
	case Aws::GameLift::Model::GameSessionStatus::NOT_SET:
		return EGameLiftGameSessionStatus::STATUS_NotSet;
	case Aws::GameLift::Model::GameSessionStatus::ACTIVE:
		return EGameLiftGameSessionStatus::STATUS_Active;
	case Aws::GameLift::Model::GameSessionStatus::ACTIVATING:
		return EGameLiftGameSessionStatus::STATUS_Activating;
	case Aws::GameLift::Model::GameSessionStatus::TERMINATED:
		return EGameLiftGameSessionStatus::STATUS_Terminated;
	case Aws::GameLift::Model::GameSessionStatus::TERMINATING:
		return EGameLiftGameSessionStatus::STATUS_Terminating;
	case Aws::GameLift::Model::GameSessionStatus::ERROR_:
		return EGameLiftGameSessionStatus::STATUS_Error;
	default:
		return EGameLiftGameSessionStatus::STATUS_NoStatus;
	}

	checkNoEntry();
#endif
	return EGameLiftGameSessionStatus::STATUS_NoStatus;
}

EGameLiftGameSessionStatusReason UGameLiftClientHelper::ParseGameSessionStatusReason(Aws::GameLift::Model::GameSessionStatusReason Reason)
{
#if WITH_GAMELIFTCLIENTSDK
	switch (Reason)
	{
	case Aws::GameLift::Model::GameSessionStatusReason::INTERRUPTED:
		return EGameLiftGameSessionStatusReason::INTERRUPTED;
	default:
		return EGameLiftGameSessionStatusReason::NOT_SET;
	}
	checkNoEntry();
#endif
	return EGameLiftGameSessionStatusReason::NOT_SET;
}

FGameLiftGameSessionQueue UGameLiftClientHelper::ParseGameSessionQueue(Aws::GameLift::Model::GameSessionQueue * AwsGameSessionQueue, FGameLiftGameSessionQueue& GameSessionQueue)
{
	GameSessionQueue.Name = AwsGameSessionQueue->GetName().c_str();
	GameSessionQueue.TimeoutInSeconds = AwsGameSessionQueue->GetTimeoutInSeconds();

	for (auto it = AwsGameSessionQueue->GetDestinations().begin(); it != AwsGameSessionQueue->GetDestinations().end(); ++it) {
		FGameLiftDestination Dest;
		Dest.DestinationArn = it->GetDestinationArn().c_str();
		GameSessionQueue.Destinations.Add(Dest);
	}

	for (Aws::GameLift::Model::PlayerLatencyPolicy p : AwsGameSessionQueue->GetPlayerLatencyPolicies()) {
		FGameLiftPlayerLatencyPolicy Policy;
		Policy.MaximumIndividualPlayerLatencyMilliseconds = p.GetMaximumIndividualPlayerLatencyMilliseconds();
		Policy.PolicyDurationSeconds = p.GetPolicyDurationSeconds();
		GameSessionQueue.PlayerLatencyPolicies.Add(Policy);
	}

	return GameSessionQueue;
}

FGameLiftPlayerSession UGameLiftClientHelper::ParsePlayerSession(Aws::GameLift::Model::PlayerSession * PlayerSession)
{
	FGameLiftPlayerSession Session;
	Session.CreationTime = ParseGameLiftTime(PlayerSession->GetCreationTime());
	Session.TerimationTime = ParseGameLiftTime(PlayerSession->GetTerminationTime());

	Session.PlayerId = PlayerSession->GetPlayerId().c_str();
	Session.PlayerSessionId = PlayerSession->GetPlayerSessionId().c_str();
	Session.PlayerData = PlayerSession->GetPlayerData().c_str();
	Session.DnsName = PlayerSession->GetDnsName().c_str();
	Session.FleetId = PlayerSession->GetFleetId().c_str();
	Session.GameSessionId = PlayerSession->GetGameSessionId().c_str();
	Session.IpAddress = PlayerSession->GetIpAddress().c_str();
	Session.Port = PlayerSession->GetPort();
	
	switch (PlayerSession->GetStatus())
	{
	case Aws::GameLift::Model::PlayerSessionStatus::NOT_SET:
		Session.Status = EGameLiftPlayerSessionStatus::NOT_SET;
		break;
	case Aws::GameLift::Model::PlayerSessionStatus::RESERVED:
		Session.Status = EGameLiftPlayerSessionStatus::RESERVED;
		break;
	case Aws::GameLift::Model::PlayerSessionStatus::ACTIVE:
		Session.Status = EGameLiftPlayerSessionStatus::ACTIVE;
		break;
	case Aws::GameLift::Model::PlayerSessionStatus::COMPLETED:
		Session.Status = EGameLiftPlayerSessionStatus::COMPLETED;
		break;
	case Aws::GameLift::Model::PlayerSessionStatus::TIMEDOUT:
		Session.Status = EGameLiftPlayerSessionStatus::TIMEDOUT;
		break;
	default:
		Session.Status = EGameLiftPlayerSessionStatus::NOT_SET;
		break;
	}

	return Session;
}

FGameLiftGameSessionPlacement UGameLiftClientHelper::ParseGameSessionPlacement(Aws::GameLift::Model::GameSessionPlacement * GameSessionPlacement)
{
	FGameLiftGameSessionPlacement Session;


	Session.DnsName = GameSessionPlacement->GetDnsName().c_str();
	Session.EndTime = ParseGameLiftTime(GameSessionPlacement->GetEndTime());
	
	for (auto it = GameSessionPlacement->GetGameProperties().begin(); it != GameSessionPlacement->GetGameProperties().end(); ++it) {
		FGameLiftMap Item;
		Item.Key = it->GetKey().c_str();
		Item.Value = it->GetValue().c_str();
		Session.GameProperties.Add(Item);
	}

	Session.GameSessionArn = GameSessionPlacement->GetGameSessionArn().c_str();
	Session.GameSessionData = GameSessionPlacement->GetGameSessionData().c_str();
	Session.GameSessionId = GameSessionPlacement->GetGameSessionId().c_str();
	Session.GameSessionName = GameSessionPlacement->GetGameSessionName().c_str();
	Session.GameSessionRegion = GameSessionPlacement->GetGameSessionRegion().c_str();
	Session.IpAddress = GameSessionPlacement->GetIpAddress().c_str();
	Session.MatchmakerData = GameSessionPlacement->GetMatchmakerData().c_str();
	Session.MaximumNumberPlayerSessionCount = GameSessionPlacement->GetMaximumPlayerSessionCount();

	for (auto it = GameSessionPlacement->GetPlacedPlayerSessions().begin(); it != GameSessionPlacement->GetPlacedPlayerSessions().end(); ++it) {
		FGameLiftPlacedPlayerSession Item;
		Item.PlayerId = it->GetPlayerId().c_str();
		Item.PlayerSessionId = it->GetPlayerSessionId().c_str();
		Session.PlacedPlayerSessions.Add(Item);
	}

	Session.PlacementId = GameSessionPlacement->GetPlacementId().c_str();
	
	for (auto it = GameSessionPlacement->GetPlayerLatencies().begin(); it != GameSessionPlacement->GetPlayerLatencies().end(); ++it) {
		FGameLiftPlayerLatency Item;
		Item.LatencyInMilliseconds = it->GetLatencyInMilliseconds();
		Item.PlayerId = it->GetPlayerId().c_str();
		Item.RegionIdentifier = it->GetRegionIdentifier().c_str();
		Session.PlayerLatencies.Add(Item);
	}

	Session.Port = GameSessionPlacement->GetPort();
	Session.StartTime = ParseGameLiftTime(GameSessionPlacement->GetStartTime());
	switch (GameSessionPlacement->GetStatus())
	{
	case Aws::GameLift::Model::GameSessionPlacementState::NOT_SET:
		Session.Status = EGameLiftGameSessionPlacementState::NOT_SET;
		break;
	case Aws::GameLift::Model::GameSessionPlacementState::PENDING:
		Session.Status = EGameLiftGameSessionPlacementState::PENDING;
		break;
	case Aws::GameLift::Model::GameSessionPlacementState::FULFILLED:
		Session.Status = EGameLiftGameSessionPlacementState::FULFILLED;
		break;
	case Aws::GameLift::Model::GameSessionPlacementState::CANCELLED:
		Session.Status = EGameLiftGameSessionPlacementState::CANCELLED;
		break;
	case Aws::GameLift::Model::GameSessionPlacementState::TIMED_OUT:
		Session.Status = EGameLiftGameSessionPlacementState::TIMED_OUT;
		break;
	case Aws::GameLift::Model::GameSessionPlacementState::FAILED:
		Session.Status = EGameLiftGameSessionPlacementState::FAILED;
		break;
	default:
		Session.Status = EGameLiftGameSessionPlacementState::NOT_SET;
		break;
	}


	return Session;
}

FGameLiftPlayer UGameLiftClientHelper::ParsePlayer(Aws::GameLift::Model::Player * Player)
{
	FGameLiftPlayer NewPlayer;

	NewPlayer.PlayerId = Player->GetPlayerId().c_str();
	NewPlayer.Team = Player->GetTeam().c_str();

	for (auto It = Player->GetPlayerAttributes().begin(); It != Player->GetPlayerAttributes().end(); ++It) {
		FGameLiftAttributeValue Attribute;
		
		FString S = It->second.GetS().c_str();
		if (!S.IsEmpty()) {
			Attribute.S = S;
		}

		if (It->second.GetN()) {
			Attribute.N = It->second.GetN();
		}

		for (auto sdm = It->second.GetSDM().begin(); sdm != It->second.GetSDM().end(); ++sdm) {
			Attribute.SDM.Add(sdm->first.c_str(), sdm->second);
		}

		for (auto sl = It->second.GetSL().begin(); sl != It->second.GetSL().end(); ++sl) {
			Attribute.SL.Add(sl->c_str());
		}

		NewPlayer.PlayerAttributes.Add(It->first.c_str(), Attribute);
	}

	for (auto It = Player->GetLatencyInMs().begin(); It != Player->GetLatencyInMs().end(); ++It) {
		NewPlayer.LatencyInMs.Add(It->first.c_str(), It->second);
	}

	return NewPlayer;
}

FGameLiftGameSessionConnectionInfo UGameLiftClientHelper::ParseGameSessionConnectionInfo(Aws::GameLift::Model::GameSessionConnectionInfo * Info)
{
	FGameLiftGameSessionConnectionInfo ConInfo;
	ConInfo.DnsName = Info->GetDnsName().c_str();
	ConInfo.GameSessionArn = Info->GetGameSessionArn().c_str();
	ConInfo.IpAddress = Info->GetIpAddress().c_str();
	ConInfo.Port = Info->GetPort();

	for (auto It = Info->GetMatchedPlayerSessions().begin(); It != Info->GetMatchedPlayerSessions().end(); ++It) {
		FGameLiftPlacedPlayerSession pps;
		pps.PlayerId = It->GetPlayerId().c_str();
		pps.PlayerSessionId = It->GetPlayerSessionId().c_str();
	}

	return ConInfo;
}

FGameLiftMatchmakingTicket UGameLiftClientHelper::ParseMatchmakingTicket(Aws::GameLift::Model::MatchmakingTicket * Ticket)
{
	FGameLiftMatchmakingTicket MyTicket;

	MyTicket.ConfigurationName = Ticket->GetConfigurationName().c_str();
	MyTicket.TicketId = Ticket->GetTicketId().c_str();
	MyTicket.EstimatedWaitTime = Ticket->GetEstimatedWaitTime();
	
	switch (Ticket->GetStatus())
	{
	case Aws::GameLift::Model::MatchmakingConfigurationStatus::NOT_SET:
		MyTicket.Status = EGameLiftMatchmakingConfigurationStatus::NOT_SET;
		break;
	case Aws::GameLift::Model::MatchmakingConfigurationStatus::CANCELLED:
		MyTicket.Status = EGameLiftMatchmakingConfigurationStatus::CANCELLED;
		break;
	case Aws::GameLift::Model::MatchmakingConfigurationStatus::COMPLETED:
		MyTicket.Status = EGameLiftMatchmakingConfigurationStatus::COMPLETED;
		break;
	case Aws::GameLift::Model::MatchmakingConfigurationStatus::FAILED:
		MyTicket.Status = EGameLiftMatchmakingConfigurationStatus::FAILED;
		break;
	case Aws::GameLift::Model::MatchmakingConfigurationStatus::PLACING:
		MyTicket.Status = EGameLiftMatchmakingConfigurationStatus::PLACING;
		break;
	case Aws::GameLift::Model::MatchmakingConfigurationStatus::QUEUED:
		MyTicket.Status = EGameLiftMatchmakingConfigurationStatus::QUEUED;
		break;
	case Aws::GameLift::Model::MatchmakingConfigurationStatus::REQUIRES_ACCEPTANCE:
		MyTicket.Status = EGameLiftMatchmakingConfigurationStatus::REQUIRES_ACCEPTANCE;
		break;
	case Aws::GameLift::Model::MatchmakingConfigurationStatus::SEARCHING:
		MyTicket.Status = EGameLiftMatchmakingConfigurationStatus::SEARCHING;
		break;
	case Aws::GameLift::Model::MatchmakingConfigurationStatus::TIMED_OUT:
		MyTicket.Status = EGameLiftMatchmakingConfigurationStatus::TIMED_OUT;
		break;
	default:
		MyTicket.Status = EGameLiftMatchmakingConfigurationStatus::NOT_SET;
		break;
	}

	auto ConInfo = Ticket->GetGameSessionConnectionInfo();
	MyTicket.GameSessionConnectionInfo = ParseGameSessionConnectionInfo(&ConInfo);

	for (auto It = Ticket->GetPlayers().begin(); It != Ticket->GetPlayers().end(); ++It) {
		MyTicket.Players.Add(ParsePlayer(It._Ptr));
	}

	MyTicket.StartTime = ParseGameLiftTime(Ticket->GetStartTime());
	MyTicket.EndTime = ParseGameLiftTime(Ticket->GetEndTime());
	MyTicket.StatusMessage = Ticket->GetStatusMessage().c_str();
	MyTicket.StatusReason = Ticket->GetStatusReason().c_str();

	return MyTicket;
}

FGameLiftMatchmakingConfiguration UGameLiftClientHelper::ParseMatchmakingConfiguration(Aws::GameLift::Model::MatchmakingConfiguration * Configuration)
{
	FGameLiftMatchmakingConfiguration NewConfiguration;
	NewConfiguration.AcceptanceRequired = Configuration->GetAcceptanceRequired();
	NewConfiguration.AcceptanceTimeoutSeconds = Configuration->GetAcceptanceTimeoutSeconds();
	NewConfiguration.AdditionalPlayerCount = Configuration->GetAdditionalPlayerCount();
	NewConfiguration.CustomEventData = Configuration->GetCustomEventData().c_str();
	NewConfiguration.Description = Configuration->GetDescription().c_str();
	NewConfiguration.GameSessionData = Configuration->GetGameSessionData().c_str();
	NewConfiguration.Name = Configuration->GetName().c_str();
	NewConfiguration.NotificationTarget = Configuration->GetNotificationTarget().c_str();
	NewConfiguration.RequestTimeoutSeconds = Configuration->GetRequestTimeoutSeconds();
	NewConfiguration.RuleSetName = Configuration->GetRuleSetName().c_str();
	NewConfiguration.CreationTime = ParseGameLiftTime(Configuration->GetCreationTime());
	
	for (auto It = Configuration->GetGameSessionQueueArns().begin(); It != Configuration->GetGameSessionQueueArns().end(); ++It) {
		NewConfiguration.GameSessionQueueArns.Add(It->c_str());
	}

	for (auto It = Configuration->GetGameProperties().begin(); It != Configuration->GetGameProperties().end(); ++It) {
		FGameLiftMap Item;
		Item.Key = It->GetKey().c_str();
		Item.Value = It->GetValue().c_str();
		NewConfiguration.GameProperties.Add(Item);
	}
	
	switch (Configuration->GetBackfillMode())
	{
	case Aws::GameLift::Model::BackfillMode::NOT_SET:
		NewConfiguration.BackFillMode = EGameLiftBackfillMode::NOT_SET;
		break;
	case Aws::GameLift::Model::BackfillMode::AUTOMATIC:
		NewConfiguration.BackFillMode = EGameLiftBackfillMode::AUTOMATIC;
		break;
	case Aws::GameLift::Model::BackfillMode::MANUAL:
		NewConfiguration.BackFillMode = EGameLiftBackfillMode::MANUAL;
		break;
	default:
		break;
	}
	
	return NewConfiguration;
}

FGameLiftMatchmakingRuleSet UGameLiftClientHelper::ParseMatchmakingRuleSet(Aws::GameLift::Model::MatchmakingRuleSet * RuleSet)
{
	FGameLiftMatchmakingRuleSet NewRuleSet;
	NewRuleSet.CreationTime = ParseGameLiftTime(RuleSet->GetCreationTime());
	NewRuleSet.RuleSetName = RuleSet->GetRuleSetName().c_str();
	NewRuleSet.RuleSetBody = RuleSet->GetRuleSetBody().c_str();
	return NewRuleSet;
}
