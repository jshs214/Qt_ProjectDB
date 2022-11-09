#ifndef LOGTHREAD_H
#define LOGTHREAD_H

#include <QThread>
#include <QList>

class QTreeWidgetItem;

/* 로그저장을 위한 클래스 */
class LogThread : public QThread
{
    Q_OBJECT
public:
    explicit LogThread(QObject *parent = nullptr);

private:
    void run();

    QList<QTreeWidgetItem*> logList;   /* 로그의 데이터 기록 */
    QString filename;

signals:
    void send(int data);

public slots:
    void appendData(QTreeWidgetItem*);  /* 로그 입력 */
    void saveData();    /* 로그 데이터 저장 */
};

#endif // LOGTHREAD_H
