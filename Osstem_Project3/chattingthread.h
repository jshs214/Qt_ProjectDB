#ifndef CHATTINGTHREAD_H
#define CHATTINGTHREAD_H

#include <QThread>

/* 클라이언트별 채팅로그저장을 위한 클래스 */
class ChattingThread : public QThread
{
    Q_OBJECT
public:
    explicit ChattingThread(int =0, QString name = "", QObject *parent = nullptr);

private:
    void run();

    QStringList clientChattingLog;   /* 로그의 데이터 기록 */
    QString filename;

public slots:
    void appendData(QString);  /* 로그 입력 */
    void saveData();    /* 로그 데이터 저장 */
};

#endif // CHATTINGTHREAD_H
