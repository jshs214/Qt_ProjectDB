#include "ordermanagerform.h"
#include "ui_ordermanagerform.h"

#include "ordermanagerform.h"

#include <QFile>
#include <QMenu>
#include <QMessageBox>

OrderManagerForm::OrderManagerForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OrderManagerForm)
{
    ui->setupUi(this);

    /* ContextMenu의 remove 액션, 리스트의 데이터 삭제 */
    QAction* removeAction = new QAction(tr("&Remove"));
    removeAction->setIcon(QIcon(":/images/eraser.png"));
    connect(removeAction, SIGNAL(triggered()), SLOT(removeItem()));
    menu = new QMenu;
    menu->addAction(removeAction);
    ui->orderTableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->orderTableView, SIGNAL(customContextMenuRequested(QPoint)), this,
            SLOT(showContextMenu(QPoint)));

    ui->stockLineEdit->setValidator(new QIntValidator(0, 9999, this) ); //수량에 0~9999 범위의 숫자만 받도록

    //connect(ui->searchLineEdit, SIGNAL(returnPressed()), this, SLOT(on_searchPushButton_clicked()));

    ui->clientTreeWidget->QTreeView::setColumnWidth(0,40);
    ui->clientTreeWidget->QTreeView::setColumnWidth(1,70);
    ui->clientTreeWidget->QTreeView::setColumnWidth(2,95);
    ui->productTreeWidget->QTreeView::setColumnWidth(0,40);
    ui->productTreeWidget->QTreeView::setColumnWidth(1,70);
    ui->productTreeWidget->QTreeView::setColumnWidth(2,95);

}



/* 소멸자에서 파일 출력 */
OrderManagerForm::~OrderManagerForm()
{
    delete ui;

}



/* ContextMenu 슬롯 */
void OrderManagerForm::showContextMenu(const QPoint &pos)
{
    //if(ui->orderTableView->currentItem() == nullptr)    return; //에외처리
    QPoint globalPos = ui->orderTableView->mapToGlobal(pos);
    menu->exec(globalPos);
}

/* 주문정보의 데이터(트리위젯)의 리스트 제거 슬롯 */
void OrderManagerForm::removeItem()
{

}


/* 버튼 클릭 시 입력 값 초기화 하는 슬롯 */
void OrderManagerForm::on_clearButton_clicked()
{
    ui->clientTreeWidget->clear();
    ui->productTreeWidget->clear();
    ui->cIdLineEdit->clear();
    ui->pIdLineEdit->clear();
    ui->stockLineEdit->clear();
    ui->addressLineEdit->clear();
    ui->priceLineEdit->clear();
    ui->searchLineEdit->clear();
    ui->clientLineEdit->clear();
    ui->productLineEdit->clear();
}

//고객 정보 추가 or loaddata
void OrderManagerForm::updateClient(int id,QString name, QString phoneNumber, QString address)
{
    QTreeWidgetItem* cItem = new QTreeWidgetItem(ui->clientTreeWidget);
    cItem->setText(0,QString::number(id));
    cItem->setText(1,name);
    cItem->setText(2,phoneNumber);
    cItem->setText(3,address);
}

// 고객 정보 제거 시 반영
void OrderManagerForm::delClient(int id)
{
    {
        auto item = ui->clientTreeWidget->findItems(QString::number(id), Qt::MatchFixedString, 0);
        foreach(auto i, item) {
            if(QString::number(id) == i->text(0))
                ui->clientTreeWidget->takeTopLevelItem(ui->clientTreeWidget->indexOfTopLevelItem(i));
            delete i;
        }
    }

}
// 고객 정보 변경 시 반영
void OrderManagerForm::modClient(int id, QString name, QString phoneNumber, QString address)
{
    auto item = ui->clientTreeWidget->findItems(QString::number(id), Qt::MatchFixedString, 0);        //키값비교, 0
    foreach(auto i, item) {
        if(QString::number(id) == i->text(0)){
        i->setText(0, QString::number(id));
        i->setText(1, name);
        i->setText(2, phoneNumber);
        i->setText(3, address);
        }
    }
}

//고객 검사
void OrderManagerForm::on_clientButton_clicked()
{
    QString str = ui->clientLineEdit->text();
    auto flag =  Qt::MatchCaseSensitive|Qt::MatchContains;

    for(int i =0; i < ui->clientTreeWidget->topLevelItemCount() ; i++)
    {
        ui->clientTreeWidget->topLevelItem(i)->setHidden(true);
    }

    auto cItem = ui->clientTreeWidget->findItems(str, Qt::MatchFixedString, 0); //id 는 완전일치
    foreach(auto i, cItem) {
        i->setHidden(false);
    }

    cItem = ui->clientTreeWidget->findItems(str, flag, 1);
    foreach(auto i, cItem) {
        i->setHidden(false);
    }
}

