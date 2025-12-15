#include "registerdialog.h"
#include "ui_registerdialog.h"
#include "global.h"
#include "HttpMgr.h"

RegisterDialog::RegisterDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);
    ui->pass_Edit->setEchoMode(QLineEdit::Password);
    ui->verify_Edit->setEchoMode(QLineEdit::Password);
    ui->err_Tip->setProperty("state", "normal");

    repolish(ui->err_Tip);

    connect(HttpMgr::GetInstance().get(), &HttpMgr::sig_reg_mod_finish, this, &RegisterDialog::slot_res_mod_finish);

    initHttpHandlers();
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

void RegisterDialog::on_get_Code_clicked()
{
    auto email=  ui->email_Edit->text();
    QRegularExpression regex(R"((\w+)(\.|_)?(\w*)@(/w+)(\.(\w+))+)");

    bool match = regex.match(email).hasMatch();
    if (match) {
        // 发送 http 验证码
    } else {
        showTip(tr("邮箱地址不正确"), false);
    }
}

void RegisterDialog::slot_res_mod_finish(ReqId id, QString res, ErrorCodes err)
{
    if(err != ErrorCodes::SUCCESS) {
        showTip(tr("网络请求错误"), false);

        return ;
    }

    // 解析 JSON 字符串，res 转化为 QByteArray
    QJsonDocument jsonDoc = QJsonDocument::fromJson(res.toUtf8());
    if (jsonDoc.isNull()) {
        showTip(tr("json 解析失败"), false);

        return;
    }

    // json 解析错误
    if(!jsonDoc.isObject()) {
        showTip(tr("json 解析失败"), false);

        return;
    }

    _handlers[id](jsonDoc.object());

    return;
}

void RegisterDialog::initHttpHandlers()
{
    // 注册获取验证码回包的逻辑
    _handlers.insert(ReqId::ID_GET_VARIFY_CODE, [this](const QJsonObject& jsonObj){
        int error = jsonObj["error"].toInt();
        if (error != ErrorCodes::SUCCESS) {
            showTip(tr("参数错误"), false);

            return;
        }

        auto email = jsonObj["email"].toString();
        showTip(tr("验证码已经发送到邮箱，注意查收"), true);
        qDebug() << "email is " << email;
    });
}

void RegisterDialog::showTip(QString str, bool b_Ok)
{
    if (b_Ok) {
        ui->err_Tip->setProperty("state", "normal");
    } else {
        ui->err_Tip->setProperty("state", "err");
    }

    ui->err_Tip->setText(str);

    repolish(ui->err_Tip);
}

