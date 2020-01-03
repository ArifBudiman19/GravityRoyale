// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "GameLiftClientTypes.h"
#if WITH_GAMELIFTCLIENTSDK
#include "aws/gamelift/GameLiftClient.h"
#endif

#include "GameLiftClientMatchmakingSetting.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCreateMatchmakingConfigurationAsync, const FGameLiftMatchmakingConfiguration&, Configuration, const FString&, ErrorMessage);
UCLASS()
class GAMELIFTCLIENTSDK_API UGameLiftCreateMatchmakingConfigurationAsync : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

		friend class UGameLiftClientObject;

public:
	UPROPERTY(BlueprintAssignable)
		FOnCreateMatchmakingConfigurationAsync Success;

	UPROPERTY(BlueprintAssignable)
		FOnCreateMatchmakingConfigurationAsync Failed;

private:
	Aws::GameLift::GameLiftClient* GameLiftClient;
	bool AcceptanceRequired;
	TArray<FString> GameSessionQueueArns;
	FString Name;
	int RequestTimeoutSeconds;
	FString RuleSetName;
	int AcceptanceTimeoutSeconds;
	int AdditionalPlayerCount;
	EGameLiftBackfillMode BackfillMode;
	FString CustomEventData;
	FString Description;
	TArray<FGameLiftMap> GameProperties;
	FString GameSessionData;
	FString NotificationTarget;

public:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "Game Lift Async|Matchmaking")
		static UGameLiftCreateMatchmakingConfigurationAsync* CreateMatchmakingConfiguration(const UObject* WorldContextObject, const UGameLiftClientObject* GameLiftClientObject, bool AcceptanceRequired,	TArray<FString> GameSessionQueueArns, FString Name, int RequestTimeoutSeconds, FString RuleSetName, int AcceptanceTimeoutSeconds, int AdditionalPlayerCount, EGameLiftBackfillMode BackfillMode, FString CustomEventData, FString Description, TArray<FGameLiftMap> GameProperties, FString GameSessionData, FString NotificationTarget);

	virtual void Activate() override;

private:
	const UObject* WorldContextObject;

	void OnCreateMatchmakingConfigurationState(const Aws::GameLift::GameLiftClient* Client, const Aws::GameLift::Model::CreateMatchmakingConfigurationRequest& Request, const Aws::GameLift::Model::CreateMatchmakingConfigurationOutcome& Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context);
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDescribeMatchmakingConfigurationsAsync, const TArray<FGameLiftMatchmakingConfiguration>&, Configurations, const FString&, ErrorMessage);
UCLASS()
class GAMELIFTCLIENTSDK_API UGameLiftDescribeMatchmakingConfigurationsAsync : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

		friend class UGameLiftClientObject;

public:
	UPROPERTY(BlueprintAssignable)
		FOnDescribeMatchmakingConfigurationsAsync Success;

	UPROPERTY(BlueprintAssignable)
		FOnDescribeMatchmakingConfigurationsAsync Failed;

private:
	Aws::GameLift::GameLiftClient* GameLiftClient;
	int Limit;
	TArray<FString> Names;
	FString NextToken;
	FString RuleSetName;

public:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "Game Lift Async|Matchmaking")
		static UGameLiftDescribeMatchmakingConfigurationsAsync* DescribeMatchmakingConfigurations(const UObject* WorldContextObject, const UGameLiftClientObject* GameLiftClientObject, int Limit, TArray<FString> Names, FString NextToken, FString RuleSetName);

	virtual void Activate() override;

private:
	const UObject* WorldContextObject;

	void OnDescribeMatchmakingConfigurationsState(const Aws::GameLift::GameLiftClient* Client, const Aws::GameLift::Model::DescribeMatchmakingConfigurationsRequest& Request, const Aws::GameLift::Model::DescribeMatchmakingConfigurationsOutcome& Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context);
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpdateMatchmakingConfigurationAsync, const FGameLiftMatchmakingConfiguration&, Configuration, const FString&, ErrorMessage);
UCLASS()
class GAMELIFTCLIENTSDK_API UGameLiftUpdateMatchmakingConfigurationAsync : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

		friend class UGameLiftClientObject;

public:
	UPROPERTY(BlueprintAssignable)
		FOnUpdateMatchmakingConfigurationAsync Success;

	UPROPERTY(BlueprintAssignable)
		FOnUpdateMatchmakingConfigurationAsync Failed;

private:
	Aws::GameLift::GameLiftClient* GameLiftClient;
	bool AcceptanceRequired;
	bool SetAcceptanceRequired;
	TArray<FString> GameSessionQueueArns;
	FString Name;
	int RequestTimeoutSeconds;
	FString RuleSetName;
	int AcceptanceTimeoutSeconds;
	int AdditionalPlayerCount;
	EGameLiftBackfillMode BackfillMode;
	FString CustomEventData;
	FString Description;
	TArray<FGameLiftMap> GameProperties;
	FString GameSessionData;
	FString NotificationTarget;

public:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "Game Lift Async|Matchmaking")
		static UGameLiftUpdateMatchmakingConfigurationAsync* UpdateMatchmakingConfiguration(const UObject* WorldContextObject, const UGameLiftClientObject* GameLiftClientObject, FString Name, bool OverrideAcceptanceRequired, bool AcceptanceRequired, TArray<FString> GameSessionQueueArns, int RequestTimeoutSeconds, FString RuleSetName, int AcceptanceTimeoutSeconds, int AdditionalPlayerCount, EGameLiftBackfillMode BackfillMode, FString CustomEventData, FString Description, TArray<FGameLiftMap> GameProperties, FString GameSessionData, FString NotificationTarget);

	virtual void Activate() override;

private:
	const UObject* WorldContextObject;

	void OnUpdateMatchmakingConfigurationState(const Aws::GameLift::GameLiftClient* Client, const Aws::GameLift::Model::UpdateMatchmakingConfigurationRequest& Request, const Aws::GameLift::Model::UpdateMatchmakingConfigurationOutcome& Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context);
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeleteMatchmakingConfigurationAsync, const FString&, ErrorMessage);
UCLASS()
class GAMELIFTCLIENTSDK_API UGameLiftDeleteMatchmakingConfigurationAsync : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

		friend class UGameLiftClientObject;

public:
	UPROPERTY(BlueprintAssignable)
		FOnDeleteMatchmakingConfigurationAsync Success;

	UPROPERTY(BlueprintAssignable)
		FOnDeleteMatchmakingConfigurationAsync Failed;

private:
	Aws::GameLift::GameLiftClient* GameLiftClient;
	FString Name;

public:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "Game Lift Async|Matchmaking")
		static UGameLiftDeleteMatchmakingConfigurationAsync* DeleteMatchmakingConfiguration(const UObject* WorldContextObject, const UGameLiftClientObject* GameLiftClientObject, FString Name);

	virtual void Activate() override;

private:
	const UObject* WorldContextObject;

	void OnDeleteMatchmakingConfigurationState(const Aws::GameLift::GameLiftClient* Client, const Aws::GameLift::Model::DeleteMatchmakingConfigurationRequest& Request, const Aws::GameLift::Model::DeleteMatchmakingConfigurationOutcome& Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context);
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCreateMatchmakingRuleSetAsync, const FGameLiftMatchmakingRuleSet&, RuleSet, const FString&, ErrorMessage);
UCLASS()
class GAMELIFTCLIENTSDK_API UGameLiftCreateMatchmakingRuleSetAsync : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

		friend class UGameLiftClientObject;

public:
	UPROPERTY(BlueprintAssignable)
		FOnCreateMatchmakingRuleSetAsync Success;

	UPROPERTY(BlueprintAssignable)
		FOnCreateMatchmakingRuleSetAsync Failed;

private:
	Aws::GameLift::GameLiftClient* GameLiftClient;
	FString Name;
	FString RuleSetBody;

public:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "Game Lift Async|Matchmaking")
		static UGameLiftCreateMatchmakingRuleSetAsync* CreateMatchmakingRuleSet(const UObject* WorldContextObject, const UGameLiftClientObject* GameLiftClientObject, FString Name, FString RuleSetBody);

	virtual void Activate() override;

private:
	const UObject* WorldContextObject;

	void OnCreateMatchmakingRuleSetState(const Aws::GameLift::GameLiftClient* Client, const Aws::GameLift::Model::CreateMatchmakingRuleSetRequest& Request, const Aws::GameLift::Model::CreateMatchmakingRuleSetOutcome& Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context);
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDescribeMatchmakingRuleSetsAsync, const TArray<FGameLiftMatchmakingRuleSet>&, RuleSets, const FString&, ErrorMessage);
UCLASS()
class GAMELIFTCLIENTSDK_API UGameLiftDescribeMatchmakingRuleSetsAsync : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

		friend class UGameLiftClientObject;

public:
	UPROPERTY(BlueprintAssignable)
		FOnDescribeMatchmakingRuleSetsAsync Success;

	UPROPERTY(BlueprintAssignable)
		FOnDescribeMatchmakingRuleSetsAsync Failed;

private:
	Aws::GameLift::GameLiftClient* GameLiftClient;
	int Limit;
	TArray<FString> Names;
	FString NextToken;

public:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "Game Lift Async|Matchmaking")
		static UGameLiftDescribeMatchmakingRuleSetsAsync* DescribeMatchmakingRuleSets(const UObject* WorldContextObject, const UGameLiftClientObject* GameLiftClientObject, int Limit, TArray<FString> Names, FString NextToken);

	virtual void Activate() override;

private:
	const UObject* WorldContextObject;

	void OnDescribeMatchmakingRuleSetsState(const Aws::GameLift::GameLiftClient* Client, const Aws::GameLift::Model::DescribeMatchmakingRuleSetsRequest& Request, const Aws::GameLift::Model::DescribeMatchmakingRuleSetsOutcome& Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context);
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnValidateMatchmakingRuleSetAsync, const bool, Valid, const FString&, ErrorMessage);
UCLASS()
class GAMELIFTCLIENTSDK_API UGameLiftValidateMatchmakingRuleSetAsync : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

		friend class UGameLiftClientObject;

public:
	UPROPERTY(BlueprintAssignable)
		FOnValidateMatchmakingRuleSetAsync Success;

	UPROPERTY(BlueprintAssignable)
		FOnValidateMatchmakingRuleSetAsync Failed;

private:
	Aws::GameLift::GameLiftClient* GameLiftClient;
	FString RuleSetBody;

public:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "Game Lift Async|Matchmaking")
		static UGameLiftValidateMatchmakingRuleSetAsync* ValidateMatchmakingRuleSet(const UObject* WorldContextObject, const UGameLiftClientObject* GameLiftClientObject, FString RuleSetBody);

	virtual void Activate() override;

private:
	const UObject* WorldContextObject;

	void OnValidateMatchmakingRuleSetState(const Aws::GameLift::GameLiftClient* Client, const Aws::GameLift::Model::ValidateMatchmakingRuleSetRequest& Request, const Aws::GameLift::Model::ValidateMatchmakingRuleSetOutcome& Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context);
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeleteMatchmakingRuleSetAsync, const FString&, ErrorMessage);
UCLASS()
class GAMELIFTCLIENTSDK_API UGameLiftDeleteMatchmakingRuleSetAsync : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

		friend class UGameLiftClientObject;

public:
	UPROPERTY(BlueprintAssignable)
		FOnDeleteMatchmakingRuleSetAsync Success;

	UPROPERTY(BlueprintAssignable)
		FOnDeleteMatchmakingRuleSetAsync Failed;

private:
	Aws::GameLift::GameLiftClient* GameLiftClient;
	FString Name;

public:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "Game Lift Async|Matchmaking")
		static UGameLiftDeleteMatchmakingRuleSetAsync* DeleteMatchmakingRuleSet(const UObject* WorldContextObject, const UGameLiftClientObject* GameLiftClientObject, FString Name);

	virtual void Activate() override;

private:
	const UObject* WorldContextObject;

	void OnDeleteMatchmakingRuleSetState(const Aws::GameLift::GameLiftClient* Client, const Aws::GameLift::Model::DeleteMatchmakingRuleSetRequest& Request, const Aws::GameLift::Model::DeleteMatchmakingRuleSetOutcome& Outcome, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& Context);
};