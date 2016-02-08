#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <functional>
#include <QSystemTrayIcon>
#include <QDebug>
#include <QDir>
#include <QMessageBox>
#include <QJsonArray>
#include <QJsonObject>
#include <QUrl>
#include <QNetworkReply>
#include <QNetworkRequest>

#include "qxtglobalshortcut.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);

    QDir path(QCoreApplication::applicationDirPath());
    m_config.setFilename(path.absoluteFilePath(QStringLiteral("config.json")));

    createActions();
    createTrayIcon();

    loadConfig();
}

MainWindow::~MainWindow()
{
    delete m_ui;
}

void MainWindow::sendMessage(const QString& message, const QString& title)
{
    m_trayIcon->showMessage(title, message);
}

void MainWindow::reloadConfig()
{
    loadConfig();
}

bool MainWindow::loadConfig()
{
    if(!m_config.load()) {
        sendMessage("Cannot load config file");
        return false;
    }

    QJsonObject& data = m_config.getData();

    // clear all the shortcuts!
    for(auto&& it: m_shortcuts) {
        QxtGlobalShortcut *shortcut = it.second;
        shortcut->disconnect();
        delete shortcut;
    }

    m_shortcuts.clear();

    // create shortcut handlers
    for(auto&& color_def: data["colors"].toArray()) {
        QJsonObject cdef = color_def.toObject();
        QString hotkey = cdef["hotkey"].toString();
        QxtGlobalShortcut *shortcut = new QxtGlobalShortcut(this);
        bool register_result = shortcut->setShortcut(QKeySequence(hotkey));
        if (register_result) {
            m_shortcuts[hotkey] = shortcut;
            connect(shortcut, &QxtGlobalShortcut::activated, this, std::bind(&MainWindow::onHotkey, this, hotkey, cdef["color"].toObject()));
            qDebug() << "Hotkey" << hotkey << "successfully registered.";
        } else {
            sendMessage(QString("Failed to register hotkey: %1").arg(hotkey));
            delete shortcut;
        }
    }

    return true;
}

void MainWindow::createTrayIcon()
{
    m_trayIconMenu = new QMenu(this);

    m_trayIconMenu->addAction(m_reloadConfigAction);
    m_trayIconMenu->addAction(m_aboutAction);
    m_trayIconMenu->addSeparator();
    m_trayIconMenu->addAction(m_quitAction);

    m_trayIcon = new QSystemTrayIcon(this);
    m_trayIcon->setIcon(QIcon(":/light-bulb.ico"));
    m_trayIcon->setToolTip("Nemo");
    m_trayIcon->setContextMenu(m_trayIconMenu);
    m_trayIcon->setVisible(true);
}

void MainWindow::about()
{
    QMessageBox msgBox;
    msgBox.setText("Nemo: a tiny desktop lamp sw");
    msgBox.setInformativeText("TODO");
    msgBox.exec();
}

void MainWindow::createActions()
{
    m_quitAction = new QAction(tr("&Exit"), this);
    connect(m_quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    m_reloadConfigAction = new QAction(tr("&Reload config"), this);
    connect(m_reloadConfigAction, SIGNAL(triggered()), this, SLOT(reloadConfig()));

    m_aboutAction = new QAction(tr("&About"), this);
    connect(m_aboutAction, SIGNAL(triggered()), this, SLOT(about()));
}

void MainWindow::onHotkey(const QString &hotkey, const QJsonObject &color)
{
    qDebug() << "Hotkey" << hotkey << "pressed. Color" << color << "send to device-server";

    QJsonObject& device_server = m_config.getData()["device-server"].toObject();

    QJsonObject& outputs = device_server["outputs"].toObject();

    QByteArray encoded_data = QString("led[%1]=%2&led[%3]=%4&led[%5]=%6").arg(
        QString::number(outputs["red"].toInt()), QString::number(color["red"].toInt()),
        QString::number(outputs["green"].toInt()), QString::number(color["green"].toInt()),
        QString::number(outputs["blue"].toInt()), QString::number(color["blue"].toInt())
    ).toUtf8();

    QString url = QString("http://%1:%2/devices/%3/led").arg(
            device_server["host"].toString(),
            QString::number(device_server["port"].toInt()),
            device_server["name"].toString());

    qDebug() << "Send data" << encoded_data << "to" << url;

    QNetworkRequest request(QUrl(url.toUtf8()));

    m_network_manager.put(request, encoded_data);
}
