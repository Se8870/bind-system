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

/*
 *    NOTE:
 *      These configuration below can be changed from your script.
 *      Add it before including bind_system.inc.
 *
 *      Please do not edit it manually here, because you're not supposed to do that
 *      and also these code below it's just default setting if you are not defining it.
 */

#if !defined MAX_BIND
    #define MAX_BIND                100
#endif

#if !defined MAX_BIND_MESSAGE
    #define MAX_BIND_MESSAGE        144
#endif

#define INVALID_BIND_ID PlayerBind:-1

// Some configuration for older compiler.
#define string: 
#define void: 

// Prepare strcopy just in case someone still using old include.
#if !defined strcopy
    stock strcopy(dest[], const string:source[], maxlength=sizeof dest)
	    return strcat((dest[0] = EOS, dest), source, maxlength);
#endif

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
    g_sBindKeys[MAX_PLAYERS][PlayerBind:MAX_BIND],
    g_sBindName[MAX_PLAYERS][PlayerBind:MAX_BIND][MAX_BIND_MESSAGE],
    g_sBindAction[MAX_PLAYERS][PlayerBind:MAX_BIND][MAX_BIND_MESSAGE]
;

stock PlayerBind:Bind_Create(playerid, const string:name[], E_BIND_TYPE:type, const string:action[])
{
    new PlayerBind:slot = PlayerBind:_Bind_FindFreeSlot(playerid);

    if (slot == INVALID_BIND_ID)
        return INVALID_BIND_ID;
    
    g_sBindOccupied[playerid][slot] = true;
    g_sBindType[playerid][slot] = type;

    strcopy(g_sBindName[playerid][slot], action, MAX_BIND_MESSAGE);
    strcopy(g_sBindAction[playerid][slot], action, MAX_BIND_MESSAGE);

    // Just to make sure if some scripter/player forgot to put `/`
    if (type == BIND_TYPE_COMMAND && action[0] != '/')
        strins(g_sBindAction[playerid][slot], "/", 0, MAX_BIND_MESSAGE);

    return slot;
}

stock void:Bind_GetSlots(playerid, PlayerBind:output[MAX_BIND])
{
    new slotCount = 0;
    for (new PlayerBind:i = PlayerBind:0; i < PlayerBind:MAX_BIND; i ++)
    {
        if (g_sBindOccupied[playerid][i])
            output[slotCount++] = i; // Store at the previous count, then update the count.
    }
}

stock bool:Bind_GetName(playerid, PlayerBind:slot, string:output[], len = sizeof(output))
{
    if (!g_sBindOccupied[playerid][slot])
        return false;

    strcopy(output, g_sBindName[playerid][slot], len);
    return true;
}

stock bool:Bind_SetName(playerid, PlayerBind:slot, const string:name[])
{
    strcopy(g_sBindName[playerid][slot], action, MAX_BIND_MESSAGE);
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

stock bool:Bind_GetAction(playerid, PlayerBind:slot, string:output[], len = sizeof(output))
{
    if (!g_sBindOccupied[playerid][slot])
        return false;

    strcopy(output, g_sBindAction[playerid][slot], len);
    return true;
}

stock bool:Bind_SetAction(playerid, PlayerBind:slot, const string:action[])
{
    strcopy(g_sBindAction[playerid][slot], action, MAX_BIND_MESSAGE);
    return true;
}

stock bool:Bind_SetKeys(playerid, PlayerBind:slot, keys)
{
    g_sBindKeys[playerid][slot] = keys;
    return true;
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

    g_sBindKeys[playerid][slot] = 0;
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
            // i think i need to just directly detects and call without using this, but maybe in another day.
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

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
    for (new PlayerBind:i; i < PlayerBind:MAX_BIND; i ++)
    {
        new bindKeys = g_sBindKeys[playerid][i];
        if ((newkeys & bindKeys) && !(oldkeys & bindKeys))
        {
            CallLocalFunction(#Bind_OnPressed, "iis", playerid, _:i, g_sBindAction[playerid][i]);
            Bind_Execute(playerid, i);
            break;
        }
    }

    #if defined Ks_OnPlayerKeyStateChange
        return Ks_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
    #else
        return 1;
    #endif
}


#if defined _ALS_OnPlayerKeyStateChange
    #undef OnPlayerKeyStateChange
#else
    #define _ALS_OnPlayerKeyStateChange
#endif

#define OnPlayerKeyStateChange Ks_OnPlayerKeyStateChange
#if defined Ks_OnPlayerKeyStateChange
    forward Ks_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
#endif