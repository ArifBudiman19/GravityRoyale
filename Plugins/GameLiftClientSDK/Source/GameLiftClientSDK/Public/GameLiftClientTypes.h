#pragma once

#include "aws/core/client/ClientConfiguration.h"
#include "GameLiftClientTypes.generated.h"

UENUM(BlueprintType)
enum class EGameLiftGameSessionStatus : uint8
{
	STATUS_Activating		UMETA(DisplayName = "Activating"),
	STATUS_Active			UMETA(DisplayName = "Active"),
	STATUS_Error			UMETA(DisplayName = "Error"),
	STATUS_NotSet			UMETA(DisplayName = "Not Set"),
	STATUS_Terminating		UMETA(DisplayName = "Terminating"),
	STATUS_Terminated		UMETA(DisplayName = "Terminated"),
	STATUS_NoStatus			UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EGameLiftPlayerSessionCreationPolicy : uint8
{
	NOT_SET				UMETA(DisplayName = "Not Set"),
	ACCEPT_ALL			UMETA(DisplayName = "Accept All"),
	DENY_ALL			UMETA(DisplayName = "Deny All")
};

UENUM(BlueprintType)
enum class EGameLiftGameSessionStatusReason : uint8
{
	NOT_SET			UMETA(DisplayName = "Not Set"),
	INTERRUPTED		UMETA(DisplayName = "Interupted")
};

UENUM(BlueprintType)
enum class EGameLiftPlayerSessionStatus : uint8
{
	NOT_SET		UMETA(DisplayName = "Not Set"),
	RESERVED	UMETA(DisplayName = "Reserved"),
	ACTIVE		UMETA(DisplayName = "Active"),
	COMPLETED	UMETA(DisplayName = "Completed"),
	TIMEDOUT	UMETA(DisplayName = "Timeout")
};

UENUM(BlueprintType)
enum class EGameLiftProtectionPolicy : uint8
{
	NOT_SET			UMETA(DisplayName = "Not Set"),
	NoProtection	UMETA(DisplayName = "No Protection"),
	FullProtection	UMETA(DisplayName = "Full Protection"),
};

USTRUCT(Blueprintable, BlueprintType)
struct FGameLiftMap
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString Key;

	UPROPERTY(BlueprintReadWrite)
	FString Value;
};

USTRUCT(BlueprintType)
struct FGameLiftPlayerSession
{
	GENERATED_BODY()
		
	UPROPERTY(BlueprintReadOnly)
	FString IpAddress;

	UPROPERTY(BlueprintReadOnly)
	int Port;

	UPROPERTY(BlueprintReadOnly)
	FString PlayerSessionId;

	UPROPERTY(BlueprintReadOnly)
	EGameLiftPlayerSessionStatus Status;

	UPROPERTY(BlueprintReadOnly)
	FString PlayerId;

	UPROPERTY(BlueprintReadOnly)
	FString PlayerData;

	UPROPERTY(BlueprintReadOnly)
	FString GameSessionId;

	UPROPERTY(BlueprintReadOnly)
	FDateTime CreationTime;

	UPROPERTY(BlueprintReadOnly)
	FString DnsName;

	UPROPERTY(BlueprintReadOnly)
	FString FleetId;

	UPROPERTY(BlueprintReadOnly)
	FDateTime TerimationTime;	
};

USTRUCT(BlueprintType)
struct FGameLiftGameSession
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FString GameSessionId;

	UPROPERTY(BlueprintReadOnly)
	int CurrentPlayerSessionCount;

	UPROPERTY(BlueprintReadOnly)
	int MaximumPlayerSessionCount;

	UPROPERTY(BlueprintReadOnly)
	FString IpAddress;

	UPROPERTY(BlueprintReadOnly)
	int Port;

	UPROPERTY(BlueprintReadOnly)
	FString Name;

	UPROPERTY(BlueprintReadOnly)
	FDateTime CreationTime;

	UPROPERTY(BlueprintReadOnly)
	FString CreatorId;

	UPROPERTY(BlueprintReadOnly)
	FString DnsName;

	UPROPERTY(BlueprintReadOnly)
	FString FleetId;

	UPROPERTY(BlueprintReadOnly)
	TArray<FGameLiftMap> GameProperties;

	UPROPERTY(BlueprintReadOnly)
	FString GameSessionData;

	UPROPERTY(BlueprintReadOnly)
	FString MatchmakerData;

	UPROPERTY(BlueprintReadOnly)
	EGameLiftGameSessionStatus Status;

	UPROPERTY(BlueprintReadOnly)
	EGameLiftGameSessionStatusReason StatusReason;

	UPROPERTY(BlueprintReadOnly)
	EGameLiftPlayerSessionCreationPolicy PlayerSessionCreationPolicy;

	UPROPERTY(BlueprintReadOnly)
	FDateTime TerminationTime;
};

USTRUCT(BlueprintType)
struct FGameLiftClientConfiguration {
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString Scheme = FString("HTTPS");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString Region = FString("ap-southeast-1");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int MaxConnection = 25;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int RequestTimeoutMs = 10000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int ConnectTimeoutMs = 10000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString EndpointOverride;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString ProxyScheme = FString("HTTP");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString ProxyHost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int ProxyPort;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString ProxyUserName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString ProxyPasword;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool VerifySSL = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString CaPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString CaFile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool FollowRedirects = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool DisableExpectHeader;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool EnableClockSkewAdjustment = true;

};

USTRUCT(BlueprintType)
struct FGameLiftCredentials {
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString AccessKeyId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString SecretKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString SessionToken;
};

USTRUCT(BlueprintType)
struct FGameLiftGameSessionConfig
{
	GENERATED_USTRUCT_BODY()

private:
	/* Maximum number of players that can be connected simultaneously to the game session. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 MaxPlayers;

	/*
	 * Unique identifier for an alias associated with the fleet to create a game session in.
	 * You can switch gameplay and players to a new fleet without changing your game client or other game components.
	 * For example, for games in production, using an alias allows you to seamlessly redirect your player base to a new game server update.
	 * @See http://docs.aws.amazon.com/gamelift/latest/developerguide/aliases-creating.html
	 * @See http://docs.aws.amazon.com/gamelift/latest/apireference/API_CreateAlias.html
	 * */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString AliasID;

	/*
	* Set of custom game session properties, formatted as a single string value. This data is passed to a
	* game server process in the GameSession (p. 316) object with a request to start a new game session.
	* Required: false
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString GameSessionData;

	/*
	* A descriptive label that is associated with a game session. Session names do not need to be unique.
	* Required: false
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString Name;

	/* Set of developer-defined properties for a game session, formatted as a set of type:value pairs.
	 * These properties are included in the GameSession object, which is passed to the game server with a request to start a new game session 
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<FGameLiftMap> GameSessionProperties;

public:

	void SetMaxPlayers(int32 NewMaxPlayers)
	{
		MaxPlayers = NewMaxPlayers;
	}

	void SetAliasID(FString NewAliasID)
	{
		AliasID = NewAliasID;
	}

	void SetGameSessionData(FString NewGameSessionData)
	{
		GameSessionData = NewGameSessionData;
	}

	void SetName(FString NewName) 
	{
		Name = NewName;
	}

	void SetGameSessionProperties(TArray<FGameLiftMap> NewProperties)
	{
		GameSessionProperties = NewProperties;
	}

	FORCEINLINE int32 GetMaxPlayers() const { return MaxPlayers; }
	FORCEINLINE FString GetAliasID() const { return AliasID; }
	FORCEINLINE FString GetGameSessionData() const { return GameSessionData; }
	FORCEINLINE FString GetName() const { return Name; }
	FORCEINLINE FString GetGameLiftLocalFleetID() const { return "fleet-1a2b3c4d-5e6f-7a8b-9c0d-1e2f3a4b5c6d"; }
	FORCEINLINE TArray<FGameLiftMap> GetGameSessionProperties() const { return GameSessionProperties; }

	FGameLiftGameSessionConfig()
	{
		MaxPlayers = 0;
		AliasID = "";
	}
};

/* Game Session Queue*/

USTRUCT(BlueprintType)
struct FGameLiftDestination {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString DestinationArn;
};

USTRUCT(BlueprintType)
struct FGameLiftPlayerLatencyPolicy {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	int MaximumIndividualPlayerLatencyMilliseconds;

	UPROPERTY(BlueprintReadWrite)
	int PolicyDurationSeconds;
};

USTRUCT(BlueprintType)
struct FGameLiftGameSessionQueue
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString GameSessionQueueArn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<FGameLiftDestination> Destinations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<FGameLiftPlayerLatencyPolicy> PlayerLatencyPolicies;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int TimeoutInSeconds;
};

/* Game Session Placement */
USTRUCT(BlueprintType)
struct FGameLiftPlacedPlayerSession {

	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString PlayerId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString PlayerSessionId;

};

USTRUCT(BlueprintType)
struct FGameLiftPlayerLatency {

	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float LatencyInMilliseconds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString PlayerId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString RegionIdentifier;

};

USTRUCT(BlueprintType)
struct FGameLiftDesiredPlayerSession {

	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString PlayerId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString PlayerData;

};

UENUM(BlueprintType)
enum class EGameLiftGameSessionPlacementState : uint8
{
	NOT_SET					UMETA(DisplayName = "Not Set"),
	PENDING					UMETA(DisplayName = "Pending"),
	FULFILLED				UMETA(DisplayName = "Fulfilled"),
	CANCELLED				UMETA(DisplayName = "Cancelled"),
	TIMED_OUT				UMETA(DisplayName = "Timed Out"),
	FAILED					UMETA(DisplayName = "Failed")
};


USTRUCT(BlueprintType)
struct FGameLiftGameSessionPlacement {

	GENERATED_BODY();

	UPROPERTY(BlueprintReadOnly)
	FString PlacementId;

	UPROPERTY(BlueprintReadOnly)
	TArray<FGameLiftPlacedPlayerSession> PlacedPlayerSessions;

	UPROPERTY(BlueprintReadOnly)
	int MaximumNumberPlayerSessionCount;

	UPROPERTY(BlueprintReadOnly)
	TArray<FGameLiftPlayerLatency> PlayerLatencies;
	
	UPROPERTY(BlueprintReadOnly)
	FString GameSessionArn;

	UPROPERTY(BlueprintReadOnly)
	FString GameSessionData;

	UPROPERTY(BlueprintReadOnly)
	FString GameSessionId;

	UPROPERTY(BlueprintReadOnly)
	FString GameSessionName;

	UPROPERTY(BlueprintReadOnly)
	FString GameSessionQueueName;

	UPROPERTY(BlueprintReadOnly)
	FString GameSessionRegion;

	UPROPERTY(BlueprintReadOnly)
	TArray<FGameLiftMap> GameProperties;

	UPROPERTY(BlueprintReadOnly)
	FString IpAddress;

	UPROPERTY(BlueprintReadOnly)
	int Port;

	UPROPERTY(BlueprintReadOnly)
	FString DnsName;

	UPROPERTY(BlueprintReadOnly)
	FString MatchmakerData;

	UPROPERTY(BlueprintReadOnly)
	FDateTime StartTime;

	UPROPERTY(BlueprintReadOnly)
	FDateTime EndTime;

	UPROPERTY(BlueprintReadOnly)
	EGameLiftGameSessionPlacementState Status;

};

/* Matchmaking */

UENUM(BlueprintType)
enum class EGameLiftMatchmakingConfigurationStatus: uint8
{
	NOT_SET					UMETA(DisplayName = "Not Set"),
	CANCELLED				UMETA(DisplayName = "Cancelled"),
	COMPLETED				UMETA(DisplayName = "Completed"),
	FAILED					UMETA(DisplayName = "Failed"),
	PLACING					UMETA(DisplayName = "Placing"),
	QUEUED					UMETA(DisplayName = "Queue"),
	REQUIRES_ACCEPTANCE		UMETA(DisplayName = "Requires Acceptance"),
	SEARCHING				UMETA(DisplayName = "Searching"),
	TIMED_OUT				UMETA(DisplayName = "Timed out")
};

UENUM(BlueprintType)
enum class EGameLiftAcceptanceType: uint8
 {
	NOT_SET		UMETA(DisplayName = "Not Set"),
	ACCEPT		UMETA(DisplayName = "Accept"),
	REJECT		UMETA(DisplayName = "Reject")
 };

USTRUCT(BlueprintType)
struct FGameLiftGameSessionConnectionInfo {

	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString GameSessionArn;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString IpAddress;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int Port;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString DnsName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<FGameLiftPlacedPlayerSession> MatchedPlayerSessions;

};


USTRUCT(BlueprintType)
struct FGameLiftAttributeValue {

	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float N;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString S;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TMap<FString, float> SDM;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<FString> SL;

};


USTRUCT(BlueprintType)
struct FGameLiftPlayer {

	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString PlayerId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString Team;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TMap<FString, FGameLiftAttributeValue> PlayerAttributes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TMap<FString, int> LatencyInMs;

};

USTRUCT(BlueprintType)
struct FGameLiftMatchmakingTicket {

	GENERATED_BODY();

	UPROPERTY(BlueprintReadOnly)
	FString ConfigurationName;
	
	UPROPERTY(BlueprintReadOnly)
	FString TicketId;

	UPROPERTY(BlueprintReadOnly)
	int EstimatedWaitTime;

	UPROPERTY(BlueprintReadOnly)
	EGameLiftMatchmakingConfigurationStatus Status;

	UPROPERTY(BlueprintReadOnly)
	FGameLiftGameSessionConnectionInfo GameSessionConnectionInfo;

	UPROPERTY(BlueprintReadOnly)
	TArray<FGameLiftPlayer> Players;

	UPROPERTY(BlueprintReadOnly)
	FDateTime StartTime;

	UPROPERTY(BlueprintReadOnly)
	FDateTime EndTime;

	UPROPERTY(BlueprintReadOnly)
	FString StatusMessage;

	UPROPERTY(BlueprintReadOnly)
	FString StatusReason;
};

/* Matchmaking Setting */
UENUM(BlueprintType)
enum class EGameLiftBackfillMode: uint8
{
	NOT_SET		UMETA(DisplayName = "Not Set"),
	AUTOMATIC	UMETA(DisplayName = "Automatic"),
	MANUAL		UMETA(DisplayName = "Manual")
};

USTRUCT(BlueprintType)
struct FGameLiftMatchmakingConfiguration {

	GENERATED_BODY();

	UPROPERTY(BlueprintReadOnly)
	bool AcceptanceRequired;

	UPROPERTY(BlueprintReadOnly)
	TArray<FString> GameSessionQueueArns;

	UPROPERTY(BlueprintReadOnly)
	FString Name;

	UPROPERTY(BlueprintReadOnly)
	int RequestTimeoutSeconds;

	UPROPERTY(BlueprintReadOnly)
	FString RuleSetName;

	UPROPERTY(BlueprintReadOnly)
	int AcceptanceTimeoutSeconds;

	UPROPERTY(BlueprintReadOnly)
	int AdditionalPlayerCount;

	UPROPERTY(BlueprintReadOnly)
	EGameLiftBackfillMode BackFillMode;

	UPROPERTY(BlueprintReadOnly)
	FString CustomEventData;

	UPROPERTY(BlueprintReadOnly)
	FString Description;

	UPROPERTY(BlueprintReadOnly)
	TArray<FGameLiftMap> GameProperties;

	UPROPERTY(BlueprintReadOnly)
	FString GameSessionData;

	UPROPERTY(BlueprintReadOnly)
	FString NotificationTarget;

	UPROPERTY(BlueprintReadOnly)
	FDateTime CreationTime;
};


USTRUCT(BlueprintType)
struct FGameLiftMatchmakingRuleSet {

	GENERATED_BODY();

	UPROPERTY(BlueprintReadOnly)
	FString RuleSetName;

	UPROPERTY(BlueprintReadOnly)
	FString RuleSetBody;

	UPROPERTY(BlueprintReadOnly)
	FDateTime CreationTime;

};