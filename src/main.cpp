#include <swiftly/swiftly.h>
#include <swiftly/server.h>
#include <swiftly/database.h>
#include <swiftly/commands.h>
#include <swiftly/configuration.h>
#include <swiftly/logger.h>
#include <swiftly/timers.h>

Server *server = nullptr;
PlayerManager *g_playerManager = nullptr;
Database *db = nullptr;
Commands *commands = nullptr;
Configuration *config = nullptr;
Logger *logger = nullptr;
Timers *timers = nullptr;

void OnProgramLoad(const char *pluginName, const char *mainFilePath)
{
    Swiftly_Setup(pluginName, mainFilePath);

    server = new Server();
    g_playerManager = new PlayerManager();
    commands = new Commands(pluginName);
    config = new Configuration();
    logger = new Logger(mainFilePath, pluginName);
    timers = new Timers();
}

void OnPluginStart()
{
    db = new Database("default_connection");
    if(db->IsConnected())
    {
        print("Database is connected.\n");
    }
    else
    {
        print("Database is not connected.\n");
        return;
    }
}

void OnPlayerConnected(Player* player)
{
    db->Query("CREATE TABLE IF NOT EXISTS s5_test.usrs (steamid BIGINT PRIMARY KEY)");
    db->Query("INSERT IGNORE INTO s5_test.usrs (steamid) VALUES ('%llu')", player->GetSteamID());
}

void OnClientDisconnect(Player* player)
{
    print("%s has played %d seconds on the server.\n", player->GetName(), player->GetConnectedTime());
}

void OnPluginStop()
{
    timers->UnregisterTimers();
}

const char *GetPluginAuthor() // ts
{
    return "";
}

const char *GetPluginVersion()
{
    return "1.0.0";
}

const char *GetPluginName()
{
    return "timeplayed";
}

const char *GetPluginWebsite()
{
    return "";
}