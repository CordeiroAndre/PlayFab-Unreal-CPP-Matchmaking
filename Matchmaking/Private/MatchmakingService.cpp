// Fill out your copyright notice in the Description page of Project Settings.


#include "Matchmaking/Public/MatchmakingService.h"

#include "PlayFab.h"
#include "PlayFabClientAPI.h"
#include "PlayFabMultiplayerAPI.h"

UMatchmakingService* UMatchmakingService::Matchmake(UObject* WorldContextObject, const FString& OwnerID, const FString& QueueName)
{
	UMatchmakingService* MatchmakingService = NewObject<UMatchmakingService>();
	MatchmakingService->_queueName = QueueName;
	MatchmakingService->_worldContextObject = WorldContextObject;
	
	PlayFab::MultiplayerModels::FEntityKey CreatorEntityKey;
	CreatorEntityKey.Id = OwnerID;
	CreatorEntityKey.Type = FString("title_player_account");
	PlayFab::MultiplayerModels::FMatchmakingPlayer Creator;
	Creator.Entity = CreatorEntityKey;
	PlayFab::MultiplayerModels::FCreateMatchmakingTicketRequest Request;
	Request.Creator = Creator;
	Request.QueueName = QueueName;
	Request.GiveUpAfterSeconds = 300;
	Request.MembersToMatchWith = TArray<PlayFab::MultiplayerModels::FEntityKey>();

	PlayFab::UPlayFabMultiplayerAPI::FCreateMatchmakingTicketDelegate OnSuccess;
	OnSuccess.BindUObject(MatchmakingService, &UMatchmakingService::CreatedMatchmakingTicket);
	PlayFab::FPlayFabErrorDelegate OnError;
	OnError.BindUObject(MatchmakingService, &UMatchmakingService::CreateMatchmakingTicketError);

	
	PlayFab::UPlayFabMultiplayerAPI().CreateMatchmakingTicket(Request, OnSuccess, OnError);
	
	return MatchmakingService;
}

void UMatchmakingService::CreatedMatchmakingTicket(const PlayFab::MultiplayerModels::FCreateMatchmakingTicketResult& Result)
{

	_ticketID = Result.TicketId;
	
	_worldContextObject->GetWorld()->GetTimerManager().SetTimer(_checkMatchmakingTicketStatusHandle, [this]()
	{
		CheckMatchmakingTicketStatus();
	}, 8, true);
}

void UMatchmakingService::CreateMatchmakingTicketError(const PlayFab::FPlayFabCppError& Result)
{
	OnResponse.Broadcast(EMatchmakingResponseStatus::FAIL_CREATING_TICKET);
}

void UMatchmakingService::CheckMatchmakingTicketStatus()
{
	PlayFab::MultiplayerModels::FGetMatchmakingTicketRequest Request;
	Request.QueueName = _queueName;
	Request.TicketId = _ticketID;

	PlayFab::UPlayFabMultiplayerAPI::FGetMatchmakingTicketDelegate OnSuccess;
	OnSuccess.BindUObject(this, &UMatchmakingService::CheckedMatchmakingTicketStatus);
	PlayFab::FPlayFabErrorDelegate OnError; 
	OnError.BindUObject(this, &UMatchmakingService::CheckMatchmakingTicketStatusError);
	PlayFab::UPlayFabMultiplayerAPI().GetMatchmakingTicket(Request, OnSuccess, OnError);
}

void UMatchmakingService::CheckedMatchmakingTicketStatus(const PlayFab::MultiplayerModels::FGetMatchmakingTicketResult& Result)
{
	if(Result.Status.Equals("Matched", ESearchCase::IgnoreCase))
	{
		_checkMatchmakingTicketStatusHandle.Invalidate();
		
		PlayFab::MultiplayerModels::FGetMatchRequest Request;
		Request.MatchId = Result.MatchId;
		Request.QueueName = _queueName;

		PlayFab::UPlayFabMultiplayerAPI::FGetMatchDelegate OnSuccess;
		OnSuccess.BindUObject(this, &UMatchmakingService::GotMatch);
		PlayFab::FPlayFabErrorDelegate OnError;
		OnError.BindUObject(this, &UMatchmakingService::GetMatchError);
		PlayFab::UPlayFabMultiplayerAPI().GetMatch(Request,OnSuccess, OnError);
	}
}

void UMatchmakingService::CheckMatchmakingTicketStatusError(const PlayFab::FPlayFabCppError& Result)
{
	OnResponse.Broadcast(EMatchmakingResponseStatus::FAIL_GETTING_TICKET);
}

void UMatchmakingService::GotMatch(const PlayFab::MultiplayerModels::FGetMatchResult& Result)
{
	OnResponse.Broadcast(EMatchmakingResponseStatus::SUCCESS);
}

void UMatchmakingService::GetMatchError(const PlayFab::FPlayFabCppError& Error)
{
	OnResponse.Broadcast(EMatchmakingResponseStatus::FAIL_GETTING_MATCH);
}

