#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <string>
#include <QTextCodec>

class CLogin : public QMainWindow
{
    Q_OBJECT
public:
    explicit CLogin(QMainWindow *parent = nullptr);
    ~CLogin();

public slots:
    bool Login();
    bool RecvLoginAck(char* pPackage);

private:
    QPushButton *m_pButtonLogin;
    QLabel *m_pLabelUserName;
    QLabel *m_pLabelpassword;
    QLineEdit *m_pLineEditName;
    QLineEdit *m_pLineEditPwd;
};
#endif // LOGIN_H
