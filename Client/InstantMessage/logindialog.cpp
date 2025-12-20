#include "logindialog.h"

#include <QDebug>

#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget* parent)
    : QDialog(parent), ui(new Ui::LoginDialog) {
  ui->setupUi(this);

  connect(ui->reg_Btn, &QPushButton::clicked, this,
          &LoginDialog::switchRegister);
}

LoginDialog::~LoginDialog() {
  qDebug() << "destruct LoginDlg";

  delete ui; }
