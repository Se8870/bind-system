# bind-system

[![sampctl](https://img.shields.io/badge/sampctl-bind--system-2f2f2f.svg?style=for-the-badge)](https://github.com/Kirima2nd/bind-system)

Simple bind system with several types, making it more dynamic and easy to use.

## Installation

Simply install to your project:

```bash
sampctl package install Kirima2nd/bind-system
```

Include in your code and begin using the library:

```pawn
#include <bind_system>
```

## Function Lists

Callbacks:
```pawn
// Called when you're using `BIND_TYPE_CUSTOM`
forward Bind_OnCustomExecuted(playerid, PlayerBind:slot, const string:action[]);
```

Functions:
```pawn
// Getting all slots and store it inside an array.
forward void:Bind_GetSlots(playerid, PlayerBind:output[MAX_BIND]);

// Get & Set
forward E_BIND_TYPE:Bind_GetType(playerid, PlayerBind:slot);
forward bool:Bind_SetType(playerid, PlaerBind:slot, E_BIND_TYPE:type);

forward bool:Bind_GetAction(playerid, PlayerBind:slot, string:output[], len = sizeof(output));
forward bool:Bind_SetAction(playerid, PlayerBind:slot, const string:action[])

// Core function
forward PlayerBind:Bind_Create(playerid, E_BIND_TYPE:type, const string:action[])
forward bool:Bind_Execute(playerid, PlayerBind:slot)
forward Bind_Remove(playerid, PlayerBind:slot)
```

## Usage

```pawn
#include <a_samp>
#include <bind_system>

CMD:createbind(playerid, params[])
{
    new 
        E_BIND_TYPE:type, action[32];

    if (sscanf(params, "is[32]", _:type, params))
    {
        // Usage: /createbind [type] [action]
        // Type: 1 =  type command | 2 = type chat | 3 = (type custom, you decided)
        return 0;
    }

    if (type == BIND_TYPE_NONE || type > BIND_TYPE_CUSTOM)
    {
        // Error: invalid type.
        return 0;
    }

    Bind_Create(playerid, type, action);
    // Success creating bind (ID: 1)
    return 1;
}

CMD:executebind(playerid, params[])
{
    new PlayerBind:slot;

    if (sscanf(params, "i", _:slot))
    {
        // Usage: /executebind [slot]
        return 0;
    }

    Bind_Execute(playerid, slot);
    return 1;
}

CMD:removebind(playerid, params[])
{
    new PlayerBind:slot;

    if (sscanf(params, "i", _:slot))
    {
        // Usage: /removebind [slot]
        return 0;
    }

    Bind_Remove(playerid, slot);
    return 1;
}
```

## Testing

To test, simply run the package:

```bash
sampctl package run
```
