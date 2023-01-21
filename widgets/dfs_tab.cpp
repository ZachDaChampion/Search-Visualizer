#include "dfs_tab.h"

DFSTab::DFSTab(QWidget* parent)
    : QWidget(parent)
{
  // Create the simulation parameters.
  simParams = new SimParams(GlobalState::SimType::DFS, this);

  // Create the layout.
  QVBoxLayout* layout = new QVBoxLayout(this);
  layout->addWidget(simParams);
  layout->addStretch();
  setLayout(layout);
}

DFSTab::~DFSTab() { }