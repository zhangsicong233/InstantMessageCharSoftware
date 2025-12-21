#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>

#include "global.h"

namespace Ui {
class RegisterDialog;
}

class RegisterDialog : public QDialog {
  Q_OBJECT

 public:
  explicit RegisterDialog(QWidget* parent = nullptr);

  ~RegisterDialog();

 private slots:
  void on_get_code_clicked();
  void slot_res_mod_finish(ReqId id, QString res, ErrorCodes err);
  void on_sure_btn_clicked();
  void on_return_btn_clicked();
  void on_cancel_btn_clicked();

 private:
  Ui::RegisterDialog* ui;
  QMap<ReqId, std::function<void(const QJsonObject&)>> _handlers;
  QMap<TipErr, QString> _tip_errs;
  QTimer* _countdown_timer;
  int _countdown;

  void initHttpHandlers();
  bool checkUserValid();
  bool checkEmailValid();
  bool checkPassValid();
  bool checkVerifyValid();
  bool checkConfirmValid();
  void showTip(QString str, bool b_Ok);
  void AddTipErr(TipErr te, QString tips);
  void DelTipErr(TipErr te);
  void ChangeTipPage();

 signals:
  void sigSwitchLogin();
};

#endif  // REGISTERDIALOG_H
