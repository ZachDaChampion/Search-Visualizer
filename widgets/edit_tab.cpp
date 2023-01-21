#include "edit_tab.h"

EditTab::EditTab(QWidget* parent)
    : QWidget(parent)
{
  // Create the layout.
  layout = new QVBoxLayout(this);

  // Create the set cell section.
  setCellGroupBox = new QGroupBox("Set Cell", this);
  setCellLayout = new QVBoxLayout(setCellGroupBox);
  setEmptyButton = new QPushButton("Empty", setCellGroupBox);
  setWallButton = new QPushButton("Wall", setCellGroupBox);
  setStartButton = new QPushButton("Start", setCellGroupBox);
  setEndButton = new QPushButton("End", setCellGroupBox);
  setCellLayout->addWidget(setEmptyButton);
  setCellLayout->addWidget(setWallButton);
  setCellLayout->addWidget(setStartButton);
  setCellLayout->addWidget(setEndButton);
  layout->addWidget(setCellGroupBox);

  // Create the reset grid section.
  resetGridGroupBox = new QGroupBox("Reset Grid", this);
  resetGridLayout = new QVBoxLayout(resetGridGroupBox);
  resetGridSizeLayout = new QHBoxLayout();
  resetGridWidthSpinBox = new QSpinBox(resetGridGroupBox);
  resetGridWidthSpinBox->setMinimum(1);
  resetGridWidthSpinBox->setMaximum(100);
  resetGridSizeLabelX = new QLabel("x", resetGridGroupBox);
  resetGridHeightSpinBox = new QSpinBox(resetGridGroupBox);
  resetGridHeightSpinBox->setMinimum(1);
  resetGridHeightSpinBox->setMaximum(100);
  resetGridSizeLabel = new QLabel("cells", resetGridGroupBox);
  resetGridSizeLayout->addWidget(resetGridWidthSpinBox);
  resetGridSizeLayout->addWidget(resetGridSizeLabelX);
  resetGridSizeLayout->addWidget(resetGridHeightSpinBox);
  resetGridSizeLayout->addWidget(resetGridSizeLabel);
  resetGridLayout->addLayout(resetGridSizeLayout);
  resetGridButton = new QPushButton("Reset Grid", resetGridGroupBox);
  resetGridLayout->addWidget(resetGridButton);
  layout->addWidget(resetGridGroupBox);

  // Set the layout.
  setLayout(layout);
}

EditTab::~EditTab() { }
