// Fill out your copyright notice in the Description page of Project Settings.
#include "AdvancedSteamFriendsLibrary.h"
#include "OnlineSubSystemHeader.h"
#include "OnlineSubsystemTypes.h"
#include "Engine/Texture.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "PixelFormat.h"

//General Log
DEFINE_LOG_CATEGORY(AdvancedSteamFriendsLog);


// Clan functions, add in soon
/*int32 UAdvancedSteamFriendsLibrary::GetFriendSteamLevel(const FBPUniqueNetId UniqueNetId)
{

#if (PLATFORM_WINDOWS || PLATFORM_MAC || PLATFORM_LINUX) && STEAM_SDK_INSTALLED
	if (!UniqueNetId.IsValid() || !UniqueNetId.UniqueNetId->IsValid() || UniqueNetId.UniqueNetId->GetType() != STEAM_SUBSYSTEM)
	{
		UE_LOG(AdvancedSteamFriendsLog, Warning, TEXT("IsAFriend Had a bad UniqueNetId!"));
		return 0;
	}

	if (SteamAPI_Init())
	{
		uint64 id = *((uint64*)UniqueNetId.UniqueNetId->GetBytes());


		// clan (group) iteration and access functions
		//virtual int GetClanCount() = 0;
		//virtual CSteamID GetClanByIndex(int iClan) = 0;
		//virtual const char *GetClanName(CSteamID steamIDClan) = 0;
		//virtual const char *GetClanTag(CSteamID steamIDClan) = 0;
		// returns the most recent information we have about what's happening in a clan
		//virtual bool GetClanActivityCounts(CSteamID steamIDClan, int *pnOnline, int *pnInGame, int *pnChatting) = 0;
		// for clans a user is a member of, they will have reasonably up-to-date information, but for others you'll have to download the info to have the latest
		//virtual SteamAPICall_t DownloadClanActivityCounts(ARRAY_COUNT(cClansToRequest) CSteamID *psteamIDClans, int cClansToRequest) = 0;

		// requests information about a clan officer list
		// when complete, data is returned in ClanOfficerListResponse_t call result
		// this makes available the calls below
		// you can only ask about clans that a user is a member of
		// note that this won't download avatars automatically; if you get an officer,
		// and no avatar image is available, call RequestUserInformation( steamID, false ) to download the avatar
		//virtual SteamAPICall_t RequestClanOfficerList(CSteamID steamIDClan) = 0;


		// returns the steamID of the clan owner
		//virtual CSteamID GetClanOwner(CSteamID steamIDClan) = 0;
		// returns the number of officers in a clan (including the owner)
		//virtual int GetClanOfficerCount(CSteamID steamIDClan) = 0;
		// returns the steamID of a clan officer, by index, of range [0,GetClanOfficerCount)
		//virtual CSteamID GetClanOfficerByIndex(CSteamID steamIDClan, int iOfficer) = 0;


		return SteamFriends()->GetFriendSteamLevel(id);
	}
#endif

	return 0;
}*/

void UAdvancedSteamFriendsLibrary::GetSteamGroups(TArray<FBPSteamGroupInfo> & SteamGroups)
{
	
#if (PLATFORM_WINDOWS || PLATFORM_MAC || PLATFORM_LINUX) && STEAM_SDK_INSTALLED

	if (SteamAPI_Init())
	{
		int numClans = SteamFriends()->GetClanCount();

		for (int i = 0; i < numClans; i++)
		{
			CSteamID SteamGroupID = SteamFriends()->GetClanByIndex(i);

			if(!SteamGroupID.IsValid())
				continue;

			FBPSteamGroupInfo GroupInfo;

			TSharedPtr<const FUniqueNetId> ValueID(new const FUniqueNetIdSteam2(SteamGroupID));
			GroupInfo.GroupID.SetUniqueNetId(ValueID);
			SteamFriends()->GetClanActivityCounts(SteamGroupID, &GroupInfo.numOnline, &GroupInfo.numInGame, &GroupInfo.numChatting);
			GroupInfo.GroupName = FString(UTF8_TO_TCHAR(SteamFriends()->GetClanName(SteamGroupID)));
			GroupInfo.GroupTag = FString(UTF8_TO_TCHAR(SteamFriends()->GetClanTag(SteamGroupID)));

			SteamGroups.Add(GroupInfo);
		}
	}
#endif

}

void UAdvancedSteamFriendsLibrary::GetSteamFriendGamePlayed(const FBPUniqueNetId UniqueNetId, EBlueprintResultSwitch &Result/*, FString & GameName*/, int32 & AppID)
{

#if (PLATFORM_WINDOWS || PLATFORM_MAC || PLATFORM_LINUX) && STEAM_SDK_INSTALLED
	if (!UniqueNetId.IsValid() || !UniqueNetId.UniqueNetId->IsValid() || UniqueNetId.UniqueNetId->GetType() != STEAM_SUBSYSTEM)
	{
		UE_LOG(AdvancedSteamFriendsLog, Warning, TEXT("GetSteamFriendGamePlayed Had a bad UniqueNetId!"));
		Result = EBlueprintResultSwitch::OnFailure;
		return;
	}

	if (SteamAPI_Init())
	{
		uint64 id = *((uint64*)UniqueNetId.UniqueNetId->GetBytes());

		FriendGameInfo_t GameInfo;
		bool bIsInGame = SteamFriends()->GetFriendGamePlayed(id, &GameInfo);

		if (bIsInGame && GameInfo.m_gameID.IsValid())
		{
			AppID = GameInfo.m_gameID.AppID();

			// Forgot this test and left it in, it is incorrect, you would need restricted access
			// And it would only find games in the local library anyway
			/*char NameBuffer[512];
			int Len = SteamAppList()->GetAppName(GameInfo.m_gameID.AppID(), NameBuffer, 512);

			if (Len != -1) // Invalid
			{
				GameName = FString(UTF8_TO_TCHAR(NameBuffer));
			}*/

			Result = EBlueprintResultSwitch::OnSuccess;
			return;
		}

	}
#endif

	Result = EBlueprintResultSwitch::OnFailure;
}

int32 UAdvancedSteamFriendsLibrary::GetFriendSteamLevel(const FBPUniqueNetId UniqueNetId)
{

#if (PLATFORM_WINDOWS || PLATFORM_MAC || PLATFORM_LINUX) && STEAM_SDK_INSTALLED
	if (!UniqueNetId.IsValid() || !UniqueNetId.UniqueNetId->IsValid() || UniqueNetId.UniqueNetId->GetType() != STEAM_SUBSYSTEM)
	{
		UE_LOG(AdvancedSteamFriendsLog, Warning, TEXT("IsAFriend Had a bad UniqueNetId!"));
		return 0;
	}

	if (SteamAPI_Init())
	{
		uint64 id = *((uint64*)UniqueNetId.UniqueNetId->GetBytes());

		return SteamFriends()->GetFriendSteamLevel(id);
	}
#endif

	return 0;
}

FString UAdvancedSteamFriendsLibrary::GetSteamPersonaName(const FBPUniqueNetId UniqueNetId)
{

#if (PLATFORM_WINDOWS || PLATFORM_MAC || PLATFORM_LINUX) && STEAM_SDK_INSTALLED
	if (!UniqueNetId.IsValid() || !UniqueNetId.UniqueNetId->IsValid() || UniqueNetId.UniqueNetId->GetType() != STEAM_SUBSYSTEM)
	{
		UE_LOG(AdvancedSteamFriendsLog, Warning, TEXT("GetSteamPersonaName Had a bad UniqueNetId!"));
		return FString(TEXT(""));
	}

	if (SteamAPI_Init())
	{
		uint64 id = *((uint64*)UniqueNetId.UniqueNetId->GetBytes());
		const char* PersonaName = SteamFriends()->GetFriendPersonaName(id);
		return FString(UTF8_TO_TCHAR(PersonaName));
	}
#endif

	return FString(TEXT(""));
}


void UAdvancedSteamFriendsLibrary::ForceRestartThroughSteam(int32 SteamAppId)
{

#if (PLATFORM_WINDOWS || PLATFORM_MAC || PLATFORM_LINUX) && STEAM_SDK_INSTALLED

	// Sanity check
	if (SteamAppId <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("ForceRestartThroughSteam: Invalid Steam AppId (%d)."), SteamAppId);
		return;
	}



	// Check if Steam environment variables are already present.
	// If they are, we are probably already running under Steam.
	const FString SteamAppIdEnv = FPlatformMisc::GetEnvironmentVariable(TEXT("SteamAppId"));
	const FString SteamGameIdEnv = FPlatformMisc::GetEnvironmentVariable(TEXT("SteamGameId"));
	const bool bLaunchedBySteam = !SteamAppIdEnv.IsEmpty() || !SteamGameIdEnv.IsEmpty();

	UE_LOG(LogTemp, Log, TEXT("ForceRestartThroughSteam: LaunchedBySteam=%s (SteamAppIdEnv='%s', SteamGameIdEnv='%s')"),
		bLaunchedBySteam ? TEXT("true") : TEXT("false"),
		*SteamAppIdEnv,
		*SteamGameIdEnv);

	// Call Valve's recommended function. If it returns true, Steam will
	// restart us and we MUST exit this process as soon as possible.
	const bool bShouldRestart = SteamAPI_RestartAppIfNecessary((uint32)SteamAppId);

	if (bShouldRestart)
	{
		UE_LOG(LogTemp, Log, TEXT("ForceRestartThroughSteam: Requesting restart via Steam (AppId=%d). Exiting now."), SteamAppId);

		// This tells Unreal to shut down cleanly. Steam will spawn a new
		// process under the correct context (steam://run/AppId).
		FPlatformMisc::RequestExit(false);

		// Do not do anything else after this point.
		return;
	}

	// If we get here, either:
	// - We were already launched via Steam, or
	// - Steam couldn't/shouldn't restart the app (e.g. wrong AppId).
	UE_LOG(LogTemp, Log, TEXT("ForceRestartThroughSteam: No restart needed (already under Steam or restart not required)."));
#endif
}

FBPUniqueNetId UAdvancedSteamFriendsLibrary::CreateSteamIDFromString(const FString SteamID64)
{
	FBPUniqueNetId netId;
	
#if (PLATFORM_WINDOWS || PLATFORM_MAC || PLATFORM_LINUX) && STEAM_SDK_INSTALLED
	if (!(SteamID64.Len() > 0))
	{
		UE_LOG(AdvancedSteamFriendsLog, Warning, TEXT("CreateSteamIDFromString Had a bad UniqueNetId!"));
		return netId;
	}

	if (SteamAPI_Init())
	{
		// Already does the conversion
		TSharedPtr<const FUniqueNetId> ValueID(new const FUniqueNetIdSteam2(SteamID64));
		//FCString::Atoi64(*SteamID64));
		
		netId.SetUniqueNetId(ValueID);
		return netId;
	}
#endif

	return netId;
}

FBPUniqueNetId UAdvancedSteamFriendsLibrary::GetLocalSteamIDFromSteam()
{
	FBPUniqueNetId netId;

#if (PLATFORM_WINDOWS || PLATFORM_MAC || PLATFORM_LINUX) && STEAM_SDK_INSTALLED
	if (SteamAPI_Init())
	{
		TSharedPtr<const FUniqueNetId> SteamID(new const FUniqueNetIdSteam2(SteamUser()->GetSteamID()));
		netId.SetUniqueNetId(SteamID);
	}
#endif

	return netId;
}

bool UAdvancedSteamFriendsLibrary::RequestSteamFriendInfo(const FBPUniqueNetId UniqueNetId, bool bRequireNameOnly)
{
#if (PLATFORM_WINDOWS || PLATFORM_MAC || PLATFORM_LINUX) && STEAM_SDK_INSTALLED
	if (!UniqueNetId.IsValid() || !UniqueNetId.UniqueNetId->IsValid() || UniqueNetId.UniqueNetId->GetType() != STEAM_SUBSYSTEM)
	{
		UE_LOG(AdvancedSteamFriendsLog, Warning, TEXT("RequestSteamFriendInfo Had a bad UniqueNetId!"));
		return false;
	}

	if (SteamAPI_Init())
	{
		uint64 id = *((uint64*)UniqueNetId.UniqueNetId->GetBytes());

		return !SteamFriends()->RequestUserInformation(id, bRequireNameOnly);
	}
#endif

	UE_LOG(AdvancedSteamFriendsLog, Warning, TEXT("RequestSteamFriendInfo Couldn't init steamAPI!"));
	return false;
}

void UAdvancedSteamFriendsLibrary::OpenSteamURL(UObject* WorldContextObject, FString URL)
{
#if (PLATFORM_WINDOWS || PLATFORM_MAC || PLATFORM_LINUX) && STEAM_SDK_INSTALLED
	if (SteamAPI_Init())
	{
		FTCHARToUTF8 Conv(*URL);
		const char* Ptr = Conv.Get();
		SteamFriends()->ActivateGameOverlayToWebPage(Ptr, EActivateGameOverlayToWebPageMode::k_EActivateGameOverlayToWebPageMode_Default);
	}
#endif
}

void UAdvancedSteamFriendsLibrary::ForceSteamVerify(UObject* WorldContextObject, bool missingOnly = false)
{
#if (PLATFORM_WINDOWS || PLATFORM_MAC || PLATFORM_LINUX) && STEAM_SDK_INSTALLED
	if (SteamAPI_Init())
	{
		SteamApps()->MarkContentCorrupt(missingOnly);
	}
#endif
}


bool UAdvancedSteamFriendsLibrary::OpenSteamUserOverlay(UObject* WorldContextObject,const FBPUniqueNetId UniqueNetId, ESteamUserOverlayType DialogType)
{
#if (PLATFORM_WINDOWS || PLATFORM_MAC || PLATFORM_LINUX) && STEAM_SDK_INSTALLED
	if (!UniqueNetId.IsValid() || !UniqueNetId.UniqueNetId->IsValid() || UniqueNetId.UniqueNetId->GetType() != STEAM_SUBSYSTEM)
	{
		UE_LOG(AdvancedSteamFriendsLog, Warning, TEXT("OpenSteamUserOverlay Had a bad UniqueNetId!"));
		return false;
	}

	if (SteamAPI_Init())
	{
		if (DialogType == ESteamUserOverlayType::invitetolobby)
		{
			UWorld* const World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
			IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(World);
			if (SessionInterface.IsValid())
			{
				FNamedOnlineSession* CurrentSession = SessionInterface->GetNamedSession(NAME_GameSession);

				if (CurrentSession && CurrentSession->SessionInfo->GetSessionId().IsValid())
				{			
					uint64 id = *((uint64*)CurrentSession->SessionInfo->GetSessionId().GetBytes());
					SteamFriends()->ActivateGameOverlayInviteDialog(id);
				}
			}
		}
		else
		{
			uint64 id = *((uint64*)UniqueNetId.UniqueNetId->GetBytes());
			FString DialogName = EnumToString("ESteamUserOverlayType", (uint8)DialogType);
			SteamFriends()->ActivateGameOverlayToUser(TCHAR_TO_ANSI(*DialogName), id);
		}
		return true;
	}
#endif

	UE_LOG(AdvancedSteamFriendsLog, Warning, TEXT("OpenSteamUserOverlay Couldn't init steamAPI!"));
	return false;
}

bool UAdvancedSteamFriendsLibrary::IsOverlayEnabled()
{
#if (PLATFORM_WINDOWS || PLATFORM_MAC || PLATFORM_LINUX) && STEAM_SDK_INSTALLED
	if (SteamAPI_Init())
	{
		return SteamUtils()->IsOverlayEnabled();
	}
#endif

	UE_LOG(AdvancedSteamFriendsLog, Warning, TEXT("OpenSteamUserOverlay Couldn't init steamAPI!"));
	return false;
}

UTexture2D * UAdvancedSteamFriendsLibrary::GetSteamFriendAvatar(const FBPUniqueNetId UniqueNetId, EBlueprintAsyncResultSwitch &Result, SteamAvatarSize AvatarSize)
{
#if (PLATFORM_WINDOWS || PLATFORM_MAC || PLATFORM_LINUX) && STEAM_SDK_INSTALLED
	if (!UniqueNetId.IsValid() || !UniqueNetId.UniqueNetId->IsValid() || UniqueNetId.UniqueNetId->GetType() != STEAM_SUBSYSTEM)
	{
		UE_LOG(AdvancedSteamFriendsLog, Warning, TEXT("GetSteamFriendAvatar Had a bad UniqueNetId!"));
		Result = EBlueprintAsyncResultSwitch::OnFailure;
		return nullptr;
	}

	uint32 Width = 0;
	uint32 Height = 0;

	if (SteamAPI_Init())
	{
		//Getting the PictureID from the SteamAPI and getting the Size with the ID
		//virtual bool RequestUserInformation( CSteamID steamIDUser, bool bRequireNameOnly ) = 0;

		
		uint64 id = *((uint64*)UniqueNetId.UniqueNetId->GetBytes());
		int Picture = 0;
		
		switch(AvatarSize)
		{
		case SteamAvatarSize::SteamAvatar_Small: Picture = SteamFriends()->GetSmallFriendAvatar(id); break;
		case SteamAvatarSize::SteamAvatar_Medium: Picture = SteamFriends()->GetMediumFriendAvatar(id); break;
		case SteamAvatarSize::SteamAvatar_Large: Picture = SteamFriends()->GetLargeFriendAvatar(id); break;
		default: break;
		}

		if (Picture == -1)
		{
			Result = EBlueprintAsyncResultSwitch::AsyncLoading;
			return NULL;
		}

		SteamUtils()->GetImageSize(Picture, &Width, &Height);

		// STOLEN FROM ANSWERHUB :p, then fixed because answerhub wasn't releasing the memory O.o
		// Also fixed image pixel format and switched to a memcpy instead of manual iteration.
		// At some point I should probably reply to that answerhub post with these fixes to prevent people killing their games.....

		if (Width > 0 && Height > 0)
		{
			//Creating the buffer "oAvatarRGBA" and then filling it with the RGBA Stream from the Steam Avatar
			uint8 *oAvatarRGBA = new uint8[Width * Height * 4];


			//Filling the buffer with the RGBA Stream from the Steam Avatar and creating a UTextur2D to parse the RGBA Steam in
			SteamUtils()->GetImageRGBA(Picture, (uint8*)oAvatarRGBA, 4 * Height * Width * sizeof(char));


			// Removed as I changed the image bit code to be RGB, I think the original author was unaware that there were different pixel formats
			/*
			//Swap R and B channels because for some reason the games whack
			for (uint32 i = 0; i < (Width * Height * 4); i += 4)
			{
				uint8 Temp = oAvatarRGBA[i + 0];
				oAvatarRGBA[i + 0] = oAvatarRGBA[i + 2];
				oAvatarRGBA[i + 2] = Temp;
			}*/

			UTexture2D* Avatar = UTexture2D::CreateTransient(Width, Height, PF_R8G8B8A8);
			// Switched to a Memcpy instead of byte by byte transer

			if (FTexturePlatformData* PlatformData = Avatar->GetPlatformData())
			{
				uint8* MipData = (uint8*)PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
				FMemory::Memcpy(MipData, (void*)oAvatarRGBA, Height * Width * 4);
				PlatformData->Mips[0].BulkData.Unlock();

				//Setting some Parameters for the Texture and finally returning it
				PlatformData->SetNumSlices(1);
				Avatar->NeverStream = true;
				//Avatar->CompressionSettings = TC_EditorIcon;
			}

			// Free RGBA buffer regardless of whether it was used or not
			delete[] oAvatarRGBA;

			Avatar->UpdateResource();

			Result = EBlueprintAsyncResultSwitch::OnSuccess;
			return Avatar;
		}
		else
		{
			UE_LOG(AdvancedSteamFriendsLog, Warning, TEXT("Bad Height / Width with steam avatar!"));
		}

		Result = EBlueprintAsyncResultSwitch::OnFailure;
		return nullptr;
	}
#endif

	UE_LOG(AdvancedSteamFriendsLog, Warning, TEXT("STEAM Couldn't be verified as initialized"));
	Result = EBlueprintAsyncResultSwitch::OnFailure;
	return nullptr;
}

bool UAdvancedSteamFriendsLibrary::InitTextFiltering()
{
#if (PLATFORM_WINDOWS || PLATFORM_MAC || PLATFORM_LINUX) && STEAM_SDK_INSTALLED

	if (SteamAPI_Init())
	{
		return SteamUtils()->InitFilterText();
	}

#endif

	return false;
}

bool UAdvancedSteamFriendsLibrary::FilterText(FString TextToFilter, EBPTextFilteringContext Context, const FBPUniqueNetId TextSourceID, FString& FilteredText)
{
#if (PLATFORM_WINDOWS || PLATFORM_MAC || PLATFORM_LINUX) && STEAM_SDK_INSTALLED

	if (SteamAPI_Init())
	{
		uint32 BufferLen = TextToFilter.Len() + 10; // Docs say 1 byte excess min, going with 10
		char* OutText = new char[BufferLen];
		
		uint64 id = 0;

		if (TextSourceID.IsValid())
		{
			id = *((uint64*)TextSourceID.UniqueNetId->GetBytes());
		}
		
		int FilterCount = SteamUtils()->FilterText((ETextFilteringContext)Context, id, TCHAR_TO_ANSI(*TextToFilter), OutText, BufferLen);

		if (FilterCount > 0)
		{
			FilteredText = FString(UTF8_TO_TCHAR(OutText));
			delete[] OutText;
			return true;
		}

		delete[] OutText;
	}

#endif

	FilteredText = TextToFilter;
	return false;
}

bool UAdvancedSteamFriendsLibrary::IsSteamInBigPictureMode()
{
#if (PLATFORM_WINDOWS || PLATFORM_MAC || PLATFORM_LINUX) && STEAM_SDK_INSTALLED

	if (SteamAPI_Init())
	{
		return SteamUtils()->IsSteamInBigPictureMode();
	}

#endif

	return false;
}

bool UAdvancedSteamFriendsLibrary::IsSteamOnline()
{
#if (PLATFORM_WINDOWS || PLATFORM_MAC || PLATFORM_LINUX) && STEAM_SDK_INSTALLED

	if (!SteamAPI_Init())
	{
		return false;
	}

	ISteamUser* User = SteamUser();
	if (!User)
	{
		return false;
	}

	// Per Steamworks SDK: BLoggedOn() returns false if Steam is in offline mode, has no
	// network connection on the local machine, or the Steam servers are down/busy.
	if (!User->BLoggedOn())
	{
		return false;
	}

	// Defensive: a valid SteamID should always be present for a logged-on user. If it isn't,
	// Steam is in some transitional / not-fully-online state.
	if (!User->GetSteamID().IsValid())
	{
		return false;
	}

	// Defensive: GetConnectedUniverse() returns k_EUniverseInvalid when the Steam client
	// is not connected to any Steam universe (e.g. offline mode, no network).
	if (ISteamUtils* Utils = SteamUtils())
	{
		if (Utils->GetConnectedUniverse() == k_EUniverseInvalid)
		{
			return false;
		}
	}

	return true;

#else
	return false;
#endif
}

bool UAdvancedSteamFriendsLibrary::RequestSteamGoOnline(bool& bOutWasAlreadyOnline)
{
	bOutWasAlreadyOnline = false;

#if (PLATFORM_WINDOWS || PLATFORM_MAC || PLATFORM_LINUX) && STEAM_SDK_INSTALLED

	if (!SteamAPI_Init())
	{
		UE_LOG(AdvancedSteamFriendsLog, Warning, TEXT("RequestSteamGoOnline: Steam SDK not initialized."));
		return false;
	}

	ISteamUser* User = SteamUser();
	if (User && User->BLoggedOn())
	{
		bOutWasAlreadyOnline = true;
		return true;
	}

	// Steamworks has no API to programmatically switch from Offline to Online. The closest we can do
	// is bring the Steam client window to the foreground so the user can click Steam -> Go Online.
	FString LaunchError;
	FPlatformProcess::LaunchURL(TEXT("steam://open/main"), nullptr, &LaunchError);
	if (!LaunchError.IsEmpty())
	{
		UE_LOG(AdvancedSteamFriendsLog, Warning, TEXT("RequestSteamGoOnline: Failed to open Steam client: %s"), *LaunchError);
		return false;
	}

	UE_LOG(AdvancedSteamFriendsLog, Log, TEXT("RequestSteamGoOnline: Steam is offline. Opened Steam client so user can go online manually."));
	return true;

#endif

	return false;
}