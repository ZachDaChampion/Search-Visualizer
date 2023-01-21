#include "astar_tab.h"

AStarTab::AStarTab(QWidget* parent)
    : QWidget(parent)
{
  // Create the simulation parameters.
  simParams = new SimParams(GlobalState::SimType::ASTAR, this);

  // Create the layout.
  QVBoxLayout* layout = new QVBoxLayout(this);
  layout->addWidget(simParams);
  layout->addStretch();
  setLayout(layout);
}

AStarTab::~AStarTab() { }