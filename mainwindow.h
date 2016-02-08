#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <map>

#include "config.h"

namespace Ui {
    class MainWindow;
}

class QSystemTrayIcon;
class QxtGlobalShortcut;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

    private slots:
        void reloadConfig();
        void about();

    private:
        void sendMessage(const QString& message, const QString& title = "Nemo");
        void createTrayIcon();
        bool loadConfig();
        void createActions();
        void onHotkey(const QString& hotkey, const QJsonObject& color);

        QSystemTrayIcon *m_trayIcon;

        QMenu *m_trayIconMenu;

        QAction *m_quitAction;
        QAction *m_reloadConfigAction;
        QAction *m_aboutAction;

        Ui::MainWindow *m_ui;
        Config m_config;

        std::map<QString, QxtGlobalShortcut*> m_shortcuts;

        QNetworkAccessManager m_network_manager;
};

#endif // MAINWINDOW_H
