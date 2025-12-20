#ifndef TIMERBTN_H
#define TIMERBTN_H

#include <QPushButton>
#include <QTimer>

class TimerBtn : public QPushButton {
 public:
  TimerBtn(QWidget* parent = nullptr);

  // 重写mouseReleaseEvent
  virtual void mouseReleaseEvent(QMouseEvent* e) override;

  ~TimerBtn();

 private:
  QTimer* _timer;
  int _counter;
};

#endif  // TIMERBTN_H
