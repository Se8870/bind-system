#include <a_samp>

#include "bind_system.inc"

new 
    PlayerBind:mySlots[MAX_PLAYERS];

main()
{
    new playerid = 12;
    new PlayerBind:slots_1 = Bind_Create(playerid, BIND_TYPE_COMMAND, "/hello");
    new PlayerBind:slots_2 = Bind_Create(playerid, BIND_TYPE_CHAT, "hello world");
    
    new PlayerBind:slots_3 = mySlots[playerid] = Bind_Create(playerid, BIND_TYPE_CUSTOM, "hello %s %d");

    print("Trying to make bind system.");

    new success;
    success = Bind_Execute(playerid, slots_1);
    printf("Execute success #1? %s", success ? "Yes" : "No");

    success = Bind_Execute(playerid, slots_2);
    printf("Execute success #2? %s", success ? "Yes" : "No");

    success = Bind_Execute(playerid, slots_3);
    printf("Execute success #3? %s", success ? "Yes" : "No");

    success = Bind_Execute(playerid, PlayerBind:69);
    printf("Execute success #4? %s", success ? "Yes" : "No");

}

public OnPlayerText(playerid, text[])
{
    printf("Player says: %s", text);
    return 0;
}

public OnPlayerCommandText(playerid, cmdtext[])
{
    if (!strcmp(cmdtext, "/hello", true))
    {
        print("Hello Two!");
        return 1;
    }

    return 0;
}

public Bind_OnCustomExecuted(playerid, PlayerBind:slot, const string:action[])
{
    if (slot == mySlots[playerid])
    {
        printf(action, "World", 12);
        return 1;
    }
    return 0;
}