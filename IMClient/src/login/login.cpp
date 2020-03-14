#include <QMessageBox>
#include "login.h"
#include "src/stdafx.h"
CLogin::CLogin(QMainWindow *parent)
{
    this->resize(QSize(400, 300));

    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap("../IMClient/resource/pictures/login_bg.jpg")));
    this->setPalette(palette);

    m_pLabelUserName = new QLabel(this);
    m_pLabelUserName->move(120, 80);
    m_pLabelUserName->setText(QStringLiteral("用户名："));
    m_pLineEditName = new QLineEdit(this);
    m_pLineEditName->move(190, 80);
    m_pLineEditName->setPlaceholderText(QStringLiteral("请输入用户名"));

    m_pLabelpassword = new QLabel(this);
    m_pLabelpassword->move(120, 130);
    m_pLabelpassword->setText(QStringLiteral("密码："));
    m_pLineEditPwd = new QLineEdit(this);
    m_pLineEditPwd->move(190, 130);
    m_pLineEditPwd->setPlaceholderText(QStringLiteral("请输入密码"));
    m_pLineEditPwd->setEchoMode(QLineEdit::Password);

    m_pButtonLogin = new QPushButton(this);
    m_pButtonLogin->move(150, 200);
    m_pButtonLogin->setText(QStringLiteral("登录"));

    connect(m_pButtonLogin, &QPushButton::clicked, this, &CLogin::Login);
    connect(g_GateServer, SIGNAL(GateServerRecvLoginAck(char*)),this,SLOT(RecvLoginAck(char*)),Qt::QueuedConnection);
    g_GateServer->start();
}

CLogin::~CLogin()
{
    delete this;
}

bool CLogin::Login()
{
    QString username = m_pLineEditName->text();
    QString password = m_pLineEditPwd->text();
    g_GateServer->SendLoginToServer(c2s_login, username.toStdString(), password.toStdString());
    printf("%s login success\n", username.toStdString().data());
    return true;
}

bool CLogin::RecvLoginAck(char* pPackage)
{
    printf("%s recv login success\n", m_pLineEditName->text().data());
    S2C_LOGIN_ACK _login_ack = *(S2C_LOGIN_ACK*)pPackage;
    if (_login_ack._result_code == success_login)
    {
       QMessageBox::information(this, QStringLiteral("成功"), QStringLiteral("密码正确"), QMessageBox::Yes);
       return true;
    }
    else
    {
        QMessageBox::warning(this, QStringLiteral("失败"), QStringLiteral("账号或密码错误"), QMessageBox::Yes);
        return true;
    }
    return false;
}
