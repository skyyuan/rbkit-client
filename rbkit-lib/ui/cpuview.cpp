#include "cpuview.h"
#include "ui_cpuview.h"
#include <QDebug>
#include "model/cpustorage.h"

CpuView::CpuView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CpuView)
{
    callGraphModel = new QStandardItemModel;
    flatGraphModel = new QStandardItemModel;

    ui->setupUi(this);

    connect(this,
            SIGNAL(fillCallGraph(QStandardItemModel*)),
            RBKit::CpuStorage::getStorage().data(),
            SLOT(fillCallGraphModel(QStandardItemModel*)));

    connect(this,
            SIGNAL(fillFlatProfile(QStandardItemModel*)),
            RBKit::CpuStorage::getStorage().data(),
            SLOT(fillFlatProfileModel(QStandardItemModel*)));

    // generating both the models
    emit fillCallGraph(callGraphModel);
    emit fillFlatProfile(flatGraphModel);

    // column headers for each model
    QStringList headers;
    headers.append("Methods");
    headers.append("Self");
    headers.append("Total");
    this->callGraphModel->setHorizontalHeaderLabels(headers);
    this->flatGraphModel->setHorizontalHeaderLabels(headers);

    // setting callgraph as default view
    updateCpuModel(callGraphModel);
}

CpuView::~CpuView()
{
    qDebug() << "++++++++++++++cpu view destroyed+++++++++++++++++";
    delete ui;
}

void CpuView::updateCpuModel(QStandardItemModel *model)
{
    qDebug() << "inside cpu tree signal";
    ui->cpuTree->setModel(model);
    ui->cpuTree->show();
}

void CpuView::on_callGraphRadio_clicked()
{
    updateCpuModel(callGraphModel);
}

void CpuView::on_flatRadio_clicked()
{
    updateCpuModel(flatGraphModel);
}