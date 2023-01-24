#ifndef EDIT_TAB_H
#define EDIT_TAB_H

#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QWidget>

class EditTab : public QWidget {
  Q_OBJECT

  private slots:

  /**
   * "Set cell to empty" button clicked.
   */
  void setEmptyButtonClicked();

  /**
   * "Set cell to wall" button clicked.
   */
  void setWallButtonClicked();

  /**
   * "Set cell to start" button clicked.
   */
  void setStartButtonClicked();

  /**
   * "Set cell to goal" button clicked.
   */
  void setGoalButtonClicked();

  public:
  /**
   * Construct a new Edit Tab.
   *
   * \param parent The parent widget.
   */
  EditTab(QWidget* parent = nullptr);

  /**
   * Destroy an Edit Tab.
   */
  ~EditTab();

  signals:

  /**
   * Signal that the selected cells should have their cost set.
   */
  void setCostSelectedCells(int cost);

  /**
   * Signal that the selected cells should be set as the start cell.
   */
  void setStartCellSelected();

  /**
   * Signal that the selected cells should be set as the goal cell.
   */
  void setGoalCellSelected();

  private:

  /*
   * Widgets in the edit tab.
   */

  QVBoxLayout* layout; // The layout of the tab.

  /*
   * Widgets for set cell section.
   */

  QSpinBox* setCellCostSpinBox; // The spin box for the cost of the cell.
  QPushButton* setEmptyButton; // The button for setting an empty cell.
  QPushButton* setWallButton; // The button for setting a wall cell.
  QPushButton* setStartButton; // The button for setting the start cell.
  QPushButton* setGoalButton; // The button for setting the goal cell.

  /*
   * Widgets for grid reset section.
   */

  // General grid reset section.
  QGroupBox* resetGridGroupBox; // The group box for the reset grid section.
  QVBoxLayout* resetGridLayout; // The layout of the reset grid section.

  // Resize grid section.
  QHBoxLayout* resetGridSizeLayout; // The layout of the set grid size section.
  QSpinBox* resetGridWidthSpinBox; // The spin box for the width of the grid.
  QLabel* resetGridSizeLabelX; // The label for the "x" in the reset grid size section.
  QSpinBox* resetGridHeightSpinBox; // The spin box for the height of the grid.
  QLabel* resetGridSizeLabel; // The label for the "cells" in the set grid size section.

  // Reset grid button.
  QPushButton* resetGridButton; // The button for resetting the grid.
};

#endif // EDIT_TAB_H