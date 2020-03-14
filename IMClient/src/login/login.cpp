#include "login.h"
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
}

CLogin::~CLogin()
{
    delete this;
}

bool CLogin::Login()
{
    QString username = m_pLineEditName->text();
    QString password = m_pLineEditPwd->text();
    printf("%s login success\n", username.toStdString().data());
    return true;
}
