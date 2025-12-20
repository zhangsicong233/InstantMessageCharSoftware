#include <QApplication>
#include <QFile>

#include "global.h"
#include "mainwindow.h"

int main(int argc, char* argv[]) {
  QApplication a(argc, argv);

  QFile qss(":/style/stylesheet.qss");
  if (qss.open(QFile::ReadOnly)) {
    qDebug("Open Success");
    QString style = QLatin1String(qss.readAll());
    a.setStyleSheet(style);
    qss.close();
  } else {
    qDebug("Open Failed");
  }

  QString app_path = QCoreApplication::applicationDirPath();

  QString fileName = "config.ini";
  QString config_path =
      QDir::toNativeSeparators(app_path + QDir::separator() + fileName);

  QSettings setttings(config_path, QSettings::IniFormat);
  QString gate_host = setttings.value("GateServer/host").toString();
  QString gate_port = setttings.value("GateServer/port").toString();
  gate_url_prefix = "http://" + gate_host + ":" + gate_port;

  MainWindow w;
  w.show();

  return a.exec();
}
