#include "game.h"

Game::Game() {}

Game::Game(const Game& other)
{
    entityManager = other.entityManager;
}

Game::~Game()
{

}

void Game::Start()
{
    m_isPlay = true;
}

void Game::Stop()
{
    entityManager->StopComponents();
    m_isPlay = false;
}

void Game::SetEntityManager(EntityManager* entityManager)
{
    this->entityManager = entityManager;
}

bool Game::isPlay() const
{
    return m_isPlay;
}
