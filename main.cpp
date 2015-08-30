#include <QApplication>
#include <QCommandLineParser>
#include <QQmlApplicationEngine>
#include <QtQml/QQmlContext>

#include <QDebug>

#include "TimeLogModel.h"
#include "ReverseProxyModel.h"
#include "TimeLogSingleton.h"
#include "DataImporter.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName("G-TimeTracker");
    app.setOrganizationDomain("g-timetracker.org");
    app.setApplicationName("G-TimeTracker");
    app.setApplicationVersion("0.1");

    QCommandLineParser parser;
    parser.setApplicationDescription("Global Time Tracker");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption importOption(QStringList() << "i" << "import", "Import a CSV file", "file");
    parser.addOption(importOption);

    parser.process(app);

    if (parser.isSet(importOption)) {
        DataImporter importer;
        return (importer.importFile(parser.value(importOption)) ? EXIT_SUCCESS : EXIT_FAILURE);
    }

    TimeLogSingleton *singleton = new TimeLogSingleton;
    qRegisterMetaType<TimeLogData>();
    TimeLogModel *model = new TimeLogModel;
    ReverseProxyModel *proxy = new ReverseProxyModel;
    proxy->setSourceModel(model);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("TimeLogModel", proxy);
    engine.rootContext()->setContextProperty("TimeLog", singleton);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
