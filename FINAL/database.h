#ifndef DATABASE_H
#define DATABASE_H
#include <QObject>
#include <QtSql>


class Database : public QObject
{
    Q_OBJECT

public:
    explicit Database(QObject *parent = nullptr);

public slots:
    void TestConnection();
    QString checkLogin(QString username, QString password);
    QString checkSignUp(QString username, QString password);
    QString getQuizz();
    void setPoint(QString name, int point);
private slots:


private:
    QSqlDatabase db;
};

#endif // DATABASE_H
