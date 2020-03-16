#ifndef GAME_H
#define GAME_H

#include "entityManager.h"

class EntityManager;

class Game
{
public:
    Game();
    Game(const Game& other);
    ~Game();

    void Start();
    void Stop();
    bool isPlay() const;
    void SetEntityManager(EntityManager* entityManager);
private:
    EntityManager* entityManager;

    bool m_isPlay = false;
};

#endif
