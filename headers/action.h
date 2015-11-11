#ifndef ACTION_H
#define ACTION_H

#include "liver.h"
#include "vec2.h"

enum class Action {
    die,
    spawn,
    eat,
    move
};

struct ActionData {
    Liver* who;
    Action what;
    Vec2 where;

    ActionData(Liver* liver, Action action, Vec2 vec)
        : who(liver)
        , what(action)
        , where(vec)
    {}
};

#endif // ACTION_H