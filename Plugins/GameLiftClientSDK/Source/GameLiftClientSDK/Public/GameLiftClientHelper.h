// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameLiftClientTypes.h"
#if WITH_GAMELIFTCLIENTSDK
#include "aws/core/utils/DateTime.h"
#include "aws/gamelift/GameLiftClient.h"
#endif
#include "GameLiftClientHelper.generated.h"

UCLASS()
class GAMELIFTCLIENTSDK_API UGameLiftClientHelper : public UObject
{
	GENERATED_BODY()

public:
	static FDateTime ParseGameLiftTime(const Aws::Utils::DateTime Time);

	static FGameLiftGameSession ParseGameSession(Aws::GameLift::Model::GameSession* GameSession);

	static EGameLiftPlayerSessionCreationPolicy ParsePlayerSessionPolicy(Aws::GameLift::Model::PlayerSessionCreationPolicy Policy);

	static EGameLiftGameSessionStatus ParseGameSessionStatus(Aws::GameLift::Model::GameSessionStatus Status);

	static EGameLiftGameSessionStatusReason ParseGameSessionStatusReason(Aws::GameLift::Model::GameSessionStatusReason Reason);

	static FGameLiftGameSessionQueue ParseGameSessionQueue(Aws::GameLift::Model::GameSessionQueue* AwsGameSessionQueue, FGameLiftGameSessionQueue& GameSessionQueue);

	static FGameLiftPlayerSession ParsePlayerSession(Aws::GameLift::Model::PlayerSession* PlayerSession);

	static FGameLiftGameSessionPlacement ParseGameSessionPlacement(Aws::GameLift::Model::GameSessionPlacement * GameSessionPlacement);

	static FGameLiftPlayer ParsePlayer(Aws::GameLift::Model::Player * Player);

	static FGameLiftGameSessionConnectionInfo ParseGameSessionConnectionInfo(Aws::GameLift::Model::GameSessionConnectionInfo * Info);

	static FGameLiftMatchmakingTicket ParseMatchmakingTicket(Aws::GameLift::Model::MatchmakingTicket * Ticket);

	static FGameLiftMatchmakingConfiguration ParseMatchmakingConfiguration(Aws::GameLift::Model::MatchmakingConfiguration * Configuration);

	static FGameLiftMatchmakingRuleSet ParseMatchmakingRuleSet(Aws::GameLift::Model::MatchmakingRuleSet * RuleSet);
};