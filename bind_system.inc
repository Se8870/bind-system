/*
 * bind_system.inc
 * By: Kirima2nd
 * 
 * I'm making this for trying to make `sampctl package release` works, lmao.
 *
 */

#if defined _INC_bind_system
    #endinput
#endif

#define _INC_bind_system

#define string: 
#define void: 

#if !defined MAX_BIND
    #define MAX_BIND                100
#endif

#if !defined MAX_BIND_MESSAGE
    #define MAX_BIND_MESSAGE        144
#endif

#define INVALID_BIND_ID PlayerBind:-1

// Use this to make your own custom modification etc etc.
forward Bind_OnCustomExecuted(playerid, PlayerBind:slot, const string:action[]);

enum E_BIND_TYPE
{
    BIND_TYPE_NONE,
    BIND_TYPE_COMMAND,
    BIND_TYPE_CHAT,
    BIND_TYPE_CUSTOM
}

new
    bool:g_sBindOccupied[MAX_PLAYERS][PlayerBind:MAX_BIND],
    E_BIND_TYPE:g_sBindType[MAX_PLAYERS][PlayerBind:MAX_BIND],
    g_sBindAction[MAX_PLAYERS][PlayerBind:MAX_BIND][MAX_BIND_MESSAGE]
;

stock void:Bind_GetSlots(playerid, PlayerBind:output[MAX_BIND])
{
    for (new PlayerBind:i = PlayerBind:0; i < PlayerBind:MAX_BIND; i ++)
    {
        if (g_sBindOccupied[playerid][i])
            output[_:i] = i;
    }
}

stock bool:Bind_GetAction(playerid, PlayerBind:slot, string:output[], len = sizeof(output))
{
    if (!g_sBindOccupied[playerid][slot])
        return false;

    strcopy(output, g_sBindAction[playerid][slot], len);
    return true;
}

stock E_BIND_TYPE:Bind_GetType(playerid, PlayerBind:slot)
{
    if (!g_sBindOccupied[playerid][slot])
        return BIND_TYPE_NONE;

    return g_sBindType[playerid][slot];
}

stock bool:Bind_SetType(playerid, PlaerBind:slot, E_BIND_TYPE:type)
{
    g_sBindType[playerid][slot] = type;
    return 1;
}

stock bool:Bind_SetAction(playerid, PlayerBind:slot, const string:action[])
{
    g_sBindAction[playerid][slot][0] = EOS;
    strcopy(g_sBindAction[playerid][slot], action, MAX_BIND_MESSAGE);
    return true;
}

stock PlayerBind:Bind_Create(playerid, E_BIND_TYPE:type, const string:action[])
{
    new PlayerBind:slot = PlayerBind:_Bind_FindFreeSlot(playerid);

    if (slot == INVALID_BIND_ID)
        return INVALID_BIND_ID;
    
    g_sBindOccupied[playerid][slot] = true;
    g_sBindType[playerid][slot] = type;

    strcopy(g_sBindAction[playerid][slot], action, MAX_BIND_MESSAGE);

    // Just to make sure if some scripter/player forgot to put `/`
    if (type == BIND_TYPE_COMMAND && action[0] != '/')
        strins(g_sBindAction[playerid][slot], "/", 0, MAX_BIND_MESSAGE);

    return slot;
}

stock bool:Bind_Execute(playerid, PlayerBind:slot)
{
    if (!g_sBindOccupied[playerid][slot])
        return false;

    _Bind_ReProcess(playerid, PlayerBind:slot);
    return true;
}

stock bool:Bind_Remove(playerid, PlayerBind:slot)
{
    g_sBindOccupied[playerid][slot] = false;
    g_sBindType[playerid][slot] = BIND_TYPE_NONE;
    g_sBindAction[playerid][slot][0] = EOS;

    return true;
}

static stock _Bind_FindFreeSlot(playerid)
{
    for (new PlayerBind:i = PlayerBind:0; i < PlayerBind:MAX_BIND; i ++)
    {
        if (!g_sBindOccupied[playerid][i]) return _:i;
    }

    return _:INVALID_BIND_ID;
}

static stock void:_Bind_ReProcess(playerid, PlayerBind:slot)
{
    switch (g_sBindType[playerid][slot])
    {
        case BIND_TYPE_COMMAND:
        {
            // using remote function to trigger command text.
            CallLocalFunction(#OnPlayerCommandText, "is", playerid, g_sBindAction[playerid][slot]);
        }
        case BIND_TYPE_CHAT:
        {
            // Since we cannot mimick the player, we just going to call the callback/event.
            CallLocalFunction(#OnPlayerText, "is", playerid, g_sBindAction[playerid][slot]);
        }
        case BIND_TYPE_CUSTOM:
        {
            CallLocalFunction(#Bind_OnCustomExecuted, "iis", playerid, _:slot, g_sBindAction[playerid][slot]);
        }
    }
}
