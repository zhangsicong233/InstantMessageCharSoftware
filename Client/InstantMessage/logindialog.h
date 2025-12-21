#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

#include "global.h"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog {
  Q_OBJECT

 public:
  explicit LoginDialog(QWidget* parent = nullptr);

  ~LoginDialog();

 private:
  Ui::LoginDialog* ui;
      QMap<TipErr, QString> _tip_errs;

  void initHead();
  bool checkUserValid();
      bool checkPwdValid();
      bool enableBtn(bool enabled);
      void showTip(QString str,bool b_ok);
  void AddTipErr(TipErr te,QString tips);
  void DelTipErr(TipErr te);

 private slots:
  void slot_forget_pwd();
  void on_login_btn_clicked();

 signals:
  void switchRegister();
  void switchReset();
};

#endif  // LOGINDIALOG_H
