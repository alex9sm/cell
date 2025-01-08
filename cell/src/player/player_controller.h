#pragma once
#include "camera.h"
#include "player.h"
#include "ui.h"

class PlayerController {
private:
    Camera& m_Camera;
    Player& m_Player;
    UI& m_UI;
    bool m_WasPlayerMode;

public:
    PlayerController(Camera& camera, Player& player, UI& ui)
        : m_Camera(camera)
        , m_Player(player)
        , m_UI(ui)
        , m_WasPlayerMode(false)
    {}

    void update(float deltaTime);
};
