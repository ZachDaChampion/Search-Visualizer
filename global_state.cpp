#include "global_state.h"

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
}

void GlobalState::setSimType(SimType type)
{
  simType_ = type;
  emit simTypeChanged(type);
}

void GlobalState::setSimSpeed(int speed)
{
  simSpeed_ = speed;
  emit simSpeedChanged(speed);
}

void GlobalState::setDraw(bool draw)
{
  draw_ = draw;
  emit drawChanged(draw);
}