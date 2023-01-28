#include "global_state.h"

#include <iostream>

GlobalState::GlobalState() { }

GlobalState& GlobalState::singleton()
{
  static GlobalState instance;
  return instance;
}

/*
 * Getters.
 */

GlobalState::SimState GlobalState::simState() const { return simState_; }

GlobalState::SimType GlobalState::simType() const { return simType_; }

int GlobalState::simSpeed() const { return simSpeed_; }

bool GlobalState::draw() const { return draw_; }

/*
 * Setters.
 */

void GlobalState::setSimState(SimState state)
{
  simState_ = state;
  emit simStateChanged(state);

  switch (state) {
  case SimState::RUNNING:
    std::cout << "Sim state changed to RUNNING" << std::endl;
    break;
  case SimState::PAUSED:
    std::cout << "Sim state changed to PAUSED" << std::endl;
    break;
  case SimState::STOPPED:
    std::cout << "Sim state changed to STOPPED" << std::endl;
    break;
  }
}

void GlobalState::setSimType(SimType type)
{
  simType_ = type;
  emit simTypeChanged(type);

  switch (type) {
  case SimType::NONE:
    std::cout << "Sim type changed to NONE" << std::endl;
    break;
  case SimType::DIJKSTRA:
    std::cout << "Sim type changed to DIJKSTRA" << std::endl;
    break;
  case SimType::ASTAR:
    std::cout << "Sim type changed to ASTAR" << std::endl;
    break;
  }
}

void GlobalState::setSimSpeed(int speed)
{
  simSpeed_ = speed;
  emit simSpeedChanged(speed);
  std::cout << "Sim speed changed to " << speed << std::endl;
}

void GlobalState::setDraw(bool draw)
{
  draw_ = draw;
  emit drawChanged(draw);
  std::cout << "Draw changed to " << draw << std::endl;
}