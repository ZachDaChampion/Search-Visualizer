#include "dijkstra_tab.h"

DijkstraTab::DijkstraTab(QWidget* parent)
    : QWidget(parent)
{
  // Create the simulation parameters.
  simParams = new SimParams(GlobalState::SimType::DIJKSTRA, this);

  // Create the layout.
  QVBoxLayout* layout = new QVBoxLayout(this);
  layout->addWidget(simParams);
  layout->addStretch();
  setLayout(layout);
}

DijkstraTab::~DijkstraTab() { }