#include "player_controller.h"

void PlayerController::update(float deltaTime) {
    bool isPlayerMode = m_UI.isPlayerMode();

    if (isPlayerMode != m_WasPlayerMode) {
        if (isPlayerMode) {
            m_Camera.attachToPlayer(&m_Player);
        }
        else {
            m_Camera.detachFromPlayer();
        }
        m_WasPlayerMode = isPlayerMode;
    }

    if (isPlayerMode) {
        m_Player.update(deltaTime);
    }
}