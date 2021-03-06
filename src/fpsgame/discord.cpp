// discord RPC

#include "engine.h"
#include "cube.h"
#include "game.h"

namespace discord
{
	void dis_updatepresence(int gamestate, const char* modename, string playername, int playermodel)
	{
		char buffer[128];
		char gay[8];
		string partykey;
		DiscordRichPresence discordPresence;
		memset(&discordPresence, 0, sizeof(discordPresence));
		switch (gamestate)
		{
			case D_MENU:
				discordPresence.state = "In the menus";
				break;
			case D_PLAYING:
				discordPresence.state = "Playing";
				break;
			case D_SPECTATE:
				discordPresence.state = "Spectating";
				break;
			case D_QUITTING:
				discordPresence.state = "Quitting game";
				discordPresence.startTimestamp = 0;
				discordPresence.endTimestamp = 0;
				break;
			default:
				discordPresence.state = "SOMETHING BROKE";
				discordPresence.details = "Message Yellowberry#0483 about what you did!";
				discordPresence.largeImageKey = "logo-large";
				discordPresence.largeImageText = "Message Yellowberry#0483 about what you did!";
				discordPresence.smallImageKey = "turkey-test";
				discordPresence.smallImageText = "Message Yellowberry#0483 about what you did!";
				discordPresence.partyId = "";
				discordPresence.partySize = 0;
				discordPresence.partyMax = 0;
				discordPresence.joinSecret = "";
				discordPresence.spectateSecret = "";
				break;
		}
		if (gamestate != D_MENU) {
			const ENetAddress* address = connectedpeer();
			defformatstring(buffer, "Mode: %s", modename);
			discordPresence.details = buffer;
			if (game::maplimit >= 0) {
				time_t curtime;
				uint32_t endtimedelta = (game::maplimit - lastmillis) / 1000;
				time(&curtime);
				//conoutf("current time: %d", endtimedelta);
				//conoutf("time: %u, end match time: %u", (uint32_t)curtime, (uint32_t)curtime + (uint32_t)endtimedelta);
				discordPresence.startTimestamp = 0;
				discordPresence.endTimestamp = (uint32_t)curtime + (uint32_t)endtimedelta;
			}
			else {
				discordPresence.startTimestamp = 0;
				discordPresence.endTimestamp = 0;
			}
			//discordPresence.startTimestamp = 0;
			//discordPresence.endTimestamp = time(0) + (5 * 60);
			discordPresence.largeImageKey = game::getclientmap();
			discordPresence.largeImageText = game::getclientmap();
			defformatstring(gay, "player-%d", playermodel);
			//conoutf("shit: %s, balls: %d", gay, playermodel);
			discordPresence.smallImageKey = gay;
			discordPresence.smallImageText = playername;
			if (address) {
				if (enet_address_get_host_ip(address, partykey, sizeof(partykey)) >= 0)
				{
					//conoutf("%s:%d", partykey, address->port);
					defformatstring(newpartykey, "%s:%u", partykey, address->port);
					//conoutf(newpartykey);
					discordPresence.partyId = newpartykey;
					discordPresence.partySize = game::players.length();
					discordPresence.partyMax = game::players.length();
					//discordPresence.joinSecret = newpartykey;
					discordPresence.joinSecret = "gay";
				}
			}
			else {
				discordPresence.state = "Playing alone";
			}		
		} 
		else 
		{ 
			discordPresence.details = "";
			discordPresence.startTimestamp = starttime;
			discordPresence.endTimestamp = 0;
			discordPresence.largeImageKey = "logo-large";
			discordPresence.largeImageText = "";
			discordPresence.partyId = "";
			discordPresence.partySize = 0;
			discordPresence.partyMax = 0;
			discordPresence.joinSecret = "";
			discordPresence.spectateSecret = "";
		}
		Discord_UpdatePresence(&discordPresence);
	}

	void handleDiscordReady(const DiscordUser* connectedUser)
	{
		conoutf("Discord: connected to user %s#%s - %s",
			connectedUser->username,
			connectedUser->discriminator,
			connectedUser->userId);
		//SDL_Quit();
		//exit(EXIT_SUCCESS);
	}

	void handleDiscordDisconnected(int errcode, const char* message)
	{
		conoutf("Discord: disconnected (%d: %s)", errcode, message);
	}

	void handleDiscordError(int errcode, const char* message)
	{
		conoutf("Discord: error (%d: %s)", errcode, message);
	}

	void handleDiscordJoin(const char* secret)
	{
		// This code never runs???
		conoutf("Discord: join (%s)\n", secret);
		//disconnect();
	}

	static void handleDiscordSpectate(const char* secret)
	{
		conoutf("Discord: spectate (%s)\n", secret);
	}

	void handleDiscordJoinRequest(const DiscordUser* request)
	{
		// This code doesn't run either?
		int response = DISCORD_REPLY_YES;
		conoutf("Discord: join request from %s#%s - %s",
			request->username,
			request->discriminator,
			request->userId);
		Discord_Respond(request->userId, response);
	}
	void dis_initdiscord()
	{
		DiscordEventHandlers handlers;
		memset(&handlers, 0, sizeof(handlers));
		handlers.ready = handleDiscordReady;
		handlers.errored = handleDiscordError;
		handlers.disconnected = handleDiscordDisconnected;
		handlers.joinGame = handleDiscordJoin;
		handlers.joinRequest = handleDiscordJoinRequest;
		handlers.spectateGame = handleDiscordSpectate;

		// Discord_Initialize(const char* applicationId, DiscordEventHandlers* handlers, int autoRegister, const char* optionalSteamId)
		Discord_Initialize("436989367941070848", &handlers, 1, "0");
	}
}