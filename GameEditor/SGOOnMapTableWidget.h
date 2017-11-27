#pragma once

#include <QWidget>
#include <qtableview.h>
#include <qboxlayout.h>
#include <memory>
#include "ui_SGOOnMapTableWidget.h"
#include "QtGEPaginator.h"
#include "SGOOnMapTM.h"
#include "SGOOnMapToolBox.h"
#include "AddOrEditSGOOnMapDialog.h"
#include "MapEditor.h"

class SGOOnMapTableWidget : public QWidget, public Ui::SGOOnMapTableWidget
{
    Q_OBJECT
private:
  std::unique_ptr<SGOOnMapToolBox> m_toolBox;
  std::unique_ptr<QTableView> m_table;
  std::unique_ptr<SGOOnMapTM> m_tableModel;
  std::unique_ptr<QtGEPaginator> m_paginator;
  MapEditor* m_mapEditor;
protected slots:
  void editBtnsStateConfigure();
  void TableRowSelected(const QItemSelection& selected, const QItemSelection& deselected);
  void PaginatorPageChanged(int pageNumber, int onPage);
  void HeaderSectionClicked(int sectionIndex);
  void UpdateTable();
  
  void DeleteBtnClicked();
  void EditBtnClicked();
  void CloneBtnClicked();
  void FreezeAllBtnClicked();
  void UnfreezeAllBtnClicked();

  void SGODeleted(int sgoId) { UpdateTable(); }
protected:
  void configureTable();
  void configureUI();
  void configurePaginator();
  std::vector<int> GetSelectedIds();
public:
  SGOOnMapTableWidget(QWidget *parent = Q_NULLPTR);
  ~SGOOnMapTableWidget();
  void AddSGOToMap(StaticGameObjectDbInfo& sgo);
  void SetMapEditor(MapEditor* mapEditor);
  SGOOnMapTM* GetTableModel() { return m_tableModel.get(); }
  void Clone(std::vector<int> selectedIds);
  void Delete(std::vector<int> selectedIds);
  void Edit(int id);
  void ClearSelection() { m_table->selectionModel()->clearSelection(); }
};
