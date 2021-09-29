#pragma once

struct Event{
public:
    Event() {}
    ~Event() {}

    enum Type {
        KEY_ENTERED, SCENE_CHANGED, NO_EVENT
    };

    struct SceneChanged {
        int nextScene;
    };

    struct KeyEntered {
        int key;
    };

    Type type;
    union {
        SceneChanged sceneChanged;
        KeyEntered keyEntered;
    };
};

