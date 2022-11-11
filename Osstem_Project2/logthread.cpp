#include "logthread.h"

#include <QTreeWidgetItem>
#include <QFile>
#include <QDateTime>

LogThread::LogThread(QObject *parent)
    : QThread{parent}
{
    QString format = "yyyyMMdd_hhmmss"; //파일 포맷
    filename = QString("log_%1.txt").arg(QDateTime::currentDateTime().toString(format));
}

void LogThread::run()
{
    Q_FOREVER {
        saveData();
        sleep(60);      // 1분마다 저장
    }
}

/* 로그 입력 */
void LogThread::appendData(QTreeWidgetItem* item)
{
    logList.append(item);
}

/* 로그 데이터 저장 */
void LogThread::saveData()
{
    if(logList.count() > 0) {
        QFile file("../data/log/"+filename);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return;
        QTextStream out(&file);

        foreach(auto item, logList) {
            out << item->text(0) << ", ";
            out << item->text(1) << ", ";
            out << item->text(2) << ", ";
            out << item->text(3) << ", ";
            out << item->text(4) << ", ";
            out << item->text(5) << "\n";
        qDebug()<< item->text(0) << item->text(1) <<item->text(2) << item->text(3)<< item->text(4)<< item->text(5);
        }
        file.close();
    }
}
