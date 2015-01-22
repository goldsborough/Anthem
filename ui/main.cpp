#include "mainwindow.hpp"
#include <QApplication>
#include <QFile>
#include <QString>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QFile file(":/style/style.qss");
    file.open(QFile::ReadOnly);

    QString styleSheet = QString(file.readAll());
    app.setStyleSheet(styleSheet);

    MainWindow window;

    window.show();

    return app.exec();
}
