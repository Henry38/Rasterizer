#include <iostream>

// Qt
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QUrl>

#include "ImageModel.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    QQmlApplicationEngine applicationEngine;

    // qml
    applicationEngine.addImportPath("qrc:/");

    // register qml specific class
    const int versionMajor = 1;
    const int versionMinor = 0;

    qmlRegisterType<ImageModel>("ImageModel", versionMajor, versionMinor, "ImageModel");

    applicationEngine.load(QUrl("qrc:/Main.qml"));

    return app.exec();
}
