// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayFabError.h"
#include "PlayFabMultiplayerDataModels.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "MatchmakingService.generated.h"

UENUM(BlueprintType)
enum class EMatchmakingResponseStatus : uint8
{
	SUCCESS,
	FAIL_CREATING_TICKET,
	FAIL_GETTING_TICKET,
	FAIL_GETTING_MATCH
};

/**
 * 
 */
UCLASS(Blueprintable)
class MATCHMAKING_API UMatchmakingService : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()


public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMatchmakingResponse, EMatchmakingResponseStatus, Status);
	UPROPERTY(BlueprintAssignable)
	FOnMatchmakingResponse OnResponse;
	
	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly = "true", WorldContext="WorldContextObject"), Category="Tutorial|Matchmaking")
	static UMatchmakingService* Matchmake(UObject* WorldContextObject, const FString& OwnerID, const FString& QueueName);

private:
	void CreatedMatchmakingTicket(const PlayFab::MultiplayerModels::FCreateMatchmakingTicketResult& Result);
	void CreateMatchmakingTicketError(const PlayFab::FPlayFabCppError& Result);
	void CheckedMatchmakingTicketStatus(const PlayFab::MultiplayerModels::FGetMatchmakingTicketResult& Result);
	void CheckMatchmakingTicketStatusError(const PlayFab::FPlayFabCppError& Result);
	void GotMatch(const PlayFab::MultiplayerModels::FGetMatchResult& Result);
	void GetMatchError(const PlayFab::FPlayFabCppError& Error);

	
	void CheckMatchmakingTicketStatus();

	FString _ticketID;
	FString _queueName;
	FTimerHandle _checkMatchmakingTicketStatusHandle;
	UPROPERTY()
	UObject* _worldContextObject;
};
