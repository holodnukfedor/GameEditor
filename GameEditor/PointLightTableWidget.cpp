#include "PointLightTableWidget.h"

PointLightTableWidget::PointLightTableWidget(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this);
    configureUI();
}

PointLightTableWidget::~PointLightTableWidget()
{
}

void PointLightTableWidget::configureUI()
{
  m_paginator = std::unique_ptr<QtGEPaginator>(new QtGEPaginator);
  m_toolBox = std::unique_ptr<PointLightWidgetToolBox>(new PointLightWidgetToolBox);

  configureTable();
  configurePaginator();
  QVBoxLayout* vertToolBoxLayout = new QVBoxLayout;
  vertToolBoxLayout->addWidget(m_toolBox.get());
  vertToolBoxLayout->addSpacerItem(new QSpacerItem(20, 1, QSizePolicy::Minimum, QSizePolicy::Expanding));

  QHBoxLayout* horizontalLayout = new QHBoxLayout;
  horizontalLayout->addLayout(vertToolBoxLayout);

  QVBoxLayout* verticalTableLayout = new QVBoxLayout;
  verticalTableLayout->addWidget(m_table.get());
  verticalTableLayout->addWidget(m_paginator.get());

  horizontalLayout->addLayout(verticalTableLayout);
  this->setLayout(horizontalLayout);

  connect(m_toolBox.get(), SIGNAL(FilterChanged()), this, SLOT(UpdateTable()));
}

void PointLightTableWidget::configureTable()
{
  m_table = std::unique_ptr<QTableView>(new QTableView);
  m_tableModel = std::unique_ptr<PointLightTM>(new PointLightTM(10));

  m_table->setModel(m_tableModel.get());

  m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
  m_table->setSelectionMode(QAbstractItemView::SingleSelection);
  m_table->resizeColumnsToContents();
  m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
  m_table->horizontalHeader()->setStretchLastSection(true);

  m_table->horizontalHeader()->setSortIndicatorShown(true);
  m_table->horizontalHeader()->setSortIndicator(1, Qt::AscendingOrder);

  connect(m_toolBox->addBtn, SIGNAL(clicked()), this, SLOT(on_addPointLightBtn_clicked()));
  connect(m_toolBox->editBtn, SIGNAL(clicked()), this, SLOT(on_editPointLightBtn_clicked()));
  connect(m_toolBox->deleteBtn, SIGNAL(clicked()), this, SLOT(on_deletePointLightBtn_clicked()));
  //connect(m_toolBox->previewBtn, SIGNAL(clicked()), this, SLOT(on_previewSGOBtn_clicked()));
  //connect(m_toolBox->ui.addToMapBtn, SIGNAL(clicked()), this, SLOT(on_addToMapBtn_clicked()));

  connect(m_table->selectionModel(),
    SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
    SLOT(RowSelected(const QItemSelection &, const QItemSelection &)));

  connect(m_tableModel.get(), SIGNAL(TableDataChanged()), this, SLOT(editBtnsStateConfigure()));
  connect(m_table->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(HeaderSectionClicked(int)));
}

void PointLightTableWidget::configurePaginator()
{
  connect(m_tableModel.get(), SIGNAL(PagingInfoChanged(PagingInfo)), m_paginator.get(), SLOT(UpdatePagingInfo(PagingInfo)));
  m_paginator->UpdatePagingInfo(m_tableModel->GetPagingInfo());

  connect(m_paginator.get(), SIGNAL(PageChanged(int, int)), this, SLOT(PaginatorPageChanged(int, int)));
}

void PointLightTableWidget::UpdateTable()
{
  m_tableModel->UpdateTable(m_paginator->GetPageNumber(),
    m_paginator->GetOnPageCount(),
    m_table->horizontalHeader()->sortIndicatorSection(),
    m_table->horizontalHeader()->sortIndicatorOrder(),
    m_toolBox->GetNameFilter(),
    m_toolBox->GetSgoNameFilter());
}

void PointLightTableWidget::HeaderSectionClicked(int sectionIndex)
{
  UpdateTable();
}

void PointLightTableWidget::PaginatorPageChanged(int pageNumber, int onPage)
{
  UpdateTable();
}

void PointLightTableWidget::RowSelected(const QItemSelection& selected, const QItemSelection& deselected)
{
  editBtnsStateConfigure();
}

void PointLightTableWidget::editBtnsStateConfigure()
{
  bool hasSelection = m_table->selectionModel()->hasSelection();
  m_toolBox->editBtn->setEnabled(hasSelection);
  m_toolBox->deleteBtn->setEnabled(hasSelection);
  m_toolBox->previewBtn->setEnabled(hasSelection);
  m_toolBox->addToMapBtn->setEnabled(hasSelection);
}

void PointLightTableWidget::on_addPointLightBtn_clicked()
{
  AddOrEditPointLightDialog dialog(this);
  if (dialog.exec() == QDialog::Accepted) {
    m_tableModel->append(dialog.GetPointLight());
  }
}

void PointLightTableWidget::on_editPointLightBtn_clicked()
{
  int selectedRow = m_table->selectionModel()->currentIndex().row();
  PointLightDbInfo pointLight = m_tableModel->GetEntity(selectedRow);

  AddOrEditPointLightDialog dialog(this);
  dialog.SetPointLight(pointLight);

  if (dialog.exec() == QDialog::Accepted) {
    m_tableModel->edit(dialog.GetPointLight());
  }
}

void PointLightTableWidget::on_deletePointLightBtn_clicked()
{
  int selectedRow = m_table->selectionModel()->currentIndex().row();
  int id = m_tableModel->index(selectedRow, 0).data().toInt();
  m_tableModel->remove(id);
}