#pragma once

class Event{
public:
    Event() {}
    ~Event() {}

    enum Type {
        key_up, key_down, key_enter, changeScene, noEvent
    } type;

    struct SceneChanged {
        SceneChanged() = default;
        int nextScene;
    };

    union {
        SceneChanged changingScene;
    };
};

