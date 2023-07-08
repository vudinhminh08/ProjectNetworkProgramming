#include "database.h"
#include <QDebug>

Database::Database(QObject *parent) : QObject(parent){
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("UserInfor");
    db.setUserName("root");
    db.setPassword("MacPhu12");
    db.open();
}
void Database::TestConnection(){
    QSqlQuery *query = new QSqlQuery(db);
    QString queryString = "SELECT username, password FROM UserInfor.userinfor";
    if (query->exec(queryString)) {
        // Query executed successfully
        while (query->next()) {
            // Access the retrieved data
            QString username = query->value(0).toString();
            QString password = query->value(1).toString();

            // Print the retrieved data
            qDebug() << "Username: " << username;
            qDebug() << "Password: " << password;
        }
    } else {
        // Query execution failed
        qDebug() << "Query execution failed: " << query->lastError().text();
    }
}

QString Database::checkLogin(QString username, QString password){
    QSqlQuery *query = new QSqlQuery(db);
    QString queryString = "SELECT * FROM UserInfor.userinfor WHERE username= '" +username+"';";
    query->exec(queryString);
    if(query->next()){
        if(query->value(2).toString() != password){
            int input_time = query->value(5).toInt() + 1;
            if(input_time == 4){
                query->exec("UPDATE `UserInfor`.`userinfor` SET `status` = '1' WHERE (`userId` = '"+query->value(0).toString()+"')");
                return "Wrong password three times. Your account is blocked!";
            }
            query->exec("UPDATE `UserInfor`.`userinfor` SET `input_time` = '"+QString::number(input_time)+"' WHERE (`userId` = '"+query->value(0).toString()+"')");
            return "Wrong password";
        }
    }
    else{
        return "Account is not existed!";
    }

    queryString = "SELECT * FROM UserInfor.userinfor WHERE username= '" +username+"'AND password ='"+password+"';";
    if (query->exec(queryString)) {
        // Query executed successfully
        if(query->next()){
            if(query->value(6).toInt()==1){
                return "Account is blocked";
            }
            else if(query->value(7).toInt()==1){
                return "Account is online";
            }
            else{
                //query->exec("UPDATE `UserInfor`.`userinfor` SET `online` = '1' ,`input_time` = '0' WHERE (`userId` = '"+query->value(0).toString()+"')");
                QString mes = "Logged in "+query->value(4).toString();
                return mes;
            }
        }
        else{
            return "Account is not existed!";
        }
    } else {
        // Query execution failed
        return  "Query execution failed: ";
    }
}

QString Database::checkSignUp(QString username, QString password){
    QSqlQuery *query = new QSqlQuery(db);
    QString queryString = "SELECT username FROM UserInfor.userinfor WHERE username= '" +username+"';";
    if (query->exec(queryString)) {
        // Query executed successfully
        if(query->next()){
            return "Account existed";
        }
        else{
            QString sqlQuery = "INSERT INTO UserInfor.userinfor (username, password) VALUES (:value1, :value2)";
            query->prepare(sqlQuery);

            // Bind the values to the query placeholders
            query->bindValue(":value1", username);
            query->bindValue(":value2", password);

            // Execute the query
            if (query->exec())
            {
                return "ok";
            }
            else
            {
                return "fail to access information";
            }
        }
    } else {
        // Query execution failed
        return  "fail to access database";
    }
}

QString Database::getQuizz(){
    QString Quizz ="";
    QSqlQuery *query = new QSqlQuery(db);
    QString queryString = "SELECT * FROM UserInfor.quizz WHERE idquizz= '1';";
    query->exec(queryString);
    if(query->next()){
        Quizz = query->value(1).toString();
    }
    return Quizz;
}
void Database :: setPoint(QString name, int point){
    QSqlQuery *query = new QSqlQuery(db);
    QString queryString = "SELECT u.point FROM UserInfor.userinfor as u where u.username ='"+name+"';";
    int curPoint = 0;
    if (query->exec(queryString)) {
            if(query->next()){
            int prePoint = query->value(0).toInt();
            curPoint = prePoint + point;
            if (curPoint < 0) curPoint = 0;
            //qDebug()<<prePoint;
        }
            else return;
    } else {

        return ;
    }
    queryString = "UPDATE `UserInfor`.`userinfor` SET `point` = '" + QString::number(curPoint) +"' WHERE (`username` = '"+name+"');";
    query->exec(queryString);
    if(curPoint <50) queryString ="UPDATE `UserInfor`.`userinfor` SET `rank` = '0' WHERE (`username` = '"+name+"');";
    if(curPoint >= 50 && curPoint <100) queryString ="UPDATE `UserInfor`.`userinfor` SET `rank` = '1' WHERE (`username` = '"+name+"');";
    if(curPoint >= 100 && curPoint <150) queryString ="UPDATE `UserInfor`.`userinfor` SET `rank` = '2' WHERE (`username` = '"+name+"');";
    if(curPoint >= 150 && curPoint <200) queryString ="UPDATE `UserInfor`.`userinfor` SET `rank` = '3' WHERE (`username` = '"+name+"');";
    if(curPoint >= 200 && curPoint <250) queryString ="UPDATE `UserInfor`.`userinfor` SET `rank` = '4' WHERE (`username` = '"+name+"');";
    if(curPoint >= 250 && curPoint <300) queryString ="UPDATE `UserInfor`.`userinfor` SET `rank` = '5' WHERE (`username` = '"+name+"');";
    if(curPoint >= 300 && curPoint <350) queryString ="UPDATE `UserInfor`.`userinfor` SET `rank` = '6' WHERE (`username` = '"+name+"');";
    if(curPoint >= 350 && curPoint <400) queryString ="UPDATE `UserInfor`.`userinfor` SET `rank` = '7' WHERE (`username` = '"+name+"');";
    if(curPoint >= 400) queryString ="UPDATE `UserInfor`.`userinfor` SET `rank` = '8' WHERE (`username` = '"+name+"');";
    query->exec(queryString);
}






