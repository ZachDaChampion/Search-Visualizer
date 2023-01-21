#include "bfs_tab.h"

BFSTab::BFSTab(QWidget* parent)
    : QWidget(parent)
{
  // Create the simulation parameters.
  simParams = new SimParams(GlobalState::SimType::BFS, this);

  // Create the layout.
  QVBoxLayout* layout = new QVBoxLayout(this);
  layout->addWidget(simParams);
  layout->addStretch();
  setLayout(layout);
}

BFSTab::~BFSTab() { }