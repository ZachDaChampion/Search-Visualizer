#include "game_loop.h"

#include <iostream>

#include "algorithms/dijkstra.h"

using SimState = GlobalState::SimState;

GameLoop& GameLoop::singleton()
{
  static GameLoop instance;
  return instance;
}

GameLoop::GameLoop()
{
  // Get a reference to the global state.
  GlobalState& globalState = GlobalState::singleton();

  // Init timer.
  timer_.setInterval(1000.0 / globalState.simSpeed());

  // Connect the timer to the tick signal.
  connect(&timer_, &QTimer::timeout, this, &GameLoop::localTick);

  // Connect to global state signals.
  connect(
      &globalState, &GlobalState::simStateChanged, this, &GameLoop::onSimStateChanged);
  connect(&globalState, &GlobalState::simSpeedChanged, this, &GameLoop::setLoopSpeed);
}

/*
 * Public slots.
 */

void GameLoop::onSimStateChanged(SimState state)
{
  switch (state) {
  case GlobalState::SimState::RUNNING:
    timer_.start();
    break;
  case GlobalState::SimState::PAUSED:
    timer_.stop();
    break;
  case GlobalState::SimState::STOPPED:
    timer_.stop();
    break;
  }

  std::cout << "Game loop updated." << std::endl;
}

void GameLoop::setSim(
    std::shared_ptr<SearchAlgorithms::SearchAlgorithm> searchAlgorithm)
{
  searchAlgorithm_ = std::move(searchAlgorithm);
}

void GameLoop::setLoopSpeed(int speed)
{
  timer_.setInterval(1000.0 / speed);
  std::cout << "Game loop updated." << std::endl;
}

/*
 * Private slots.
 */

void GameLoop::localTick()
{
  if (searchAlgorithm_ == nullptr) {
    return;
  }
  try {
    auto cells = searchAlgorithm_->step();
    emit tick();
    emit updateGraphics(cells);
    if (cells.empty()) {
      timer_.stop();
      GlobalState::singleton().setSimState(GlobalState::SimState::PAUSED);
      emit updateGraphics(searchAlgorithm_->path());
    }
  } catch (SearchAlgorithms::NoPathFoundException& e) {
    std::cout << e.what() << std::endl;
    timer_.stop();
    GlobalState::singleton().setSimState(GlobalState::SimState::PAUSED);
  }
}