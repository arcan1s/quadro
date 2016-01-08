/***************************************************************************
 *   This file is part of quadro                                           *
 *                                                                         *
 *   quadro is free software: you can redistribute it and/or               *
 *   modify it under the terms of the GNU General Public License as        *
 *   published by the Free Software Foundation, either version 3 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   quadro is distributed in the hope that it will be useful,             *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with quadro. If not, see http://www.gnu.org/licenses/           *
 ***************************************************************************/


#include "quadromainwindow.h"
#include "ui_quadromainwindow.h"

#include <QDBusConnection>
#include <QDBusMessage>
#include <QLibraryInfo>
#include <QTranslator>
#include <QStandardPaths>
#include <QUrl>
#include <QWindow>

#include <quadroui/quadroui.h>

#include "settingswindow.h"
#include "quadrouiadaptor.h"
#include "version.h"


QuadroMainWindow::QuadroMainWindow(QWidget *parent, const QVariantHash args)
    : QMainWindow(parent)
{
    qSetMessagePattern(LOG_FORMAT);
    qCDebug(LOG_UI) << __PRETTY_FUNCTION__;
        for (auto metadata : getBuildData())
            qCDebug(LOG_UI) << metadata;

    setWindowIcon(QIcon(":icon"));

    ui = new Ui::QuadroMainWindow;
    ui->setupUi(this);

    // apply window properties
    setWindowFlags(Qt::CustomizeWindowHint);
    setWindowState(Qt::WindowFullScreen);

    // update configuration
    updateConfiguration(args);
}


QuadroMainWindow::~QuadroMainWindow()
{
    qCDebug(LOG_UI) << __PRETTY_FUNCTION__;

    deleteObjects();
    delete ui;
}


void QuadroMainWindow::closeEvent(QCloseEvent *event)
{
//     if ((QSystemTrayIcon::isSystemTrayAvailable()) && (configuration[QString("SYSTRAY")] == QString("true"))) {
//         hide();
//         event->ignore();
//     } else
//         closeMainWindow();
}


void QuadroMainWindow::changeTab(const int index)
{
    qCDebug(LOG_UI) << "Index" << index;
    if ((index == -1) || (index >= ui->stackedWidget->count())) return;

    return ui->stackedWidget->setCurrentIndex(index);
}


void QuadroMainWindow::closeMainWindow()
{
    qApp->quit();
}


void QuadroMainWindow::showSettingsWindow()
{
    settingsWindow->showWindow();
}


void QuadroMainWindow::updateConfiguration(const QVariantHash args)
{
    deleteObjects();
    // update configuration
    m_configPath = QStandardPaths::locate(QStandardPaths::ConfigLocation,
                                          QString("quadro.conf"));
    qCInfo(LOG_UI) << "Load configuration from" << m_configPath;
    settingsWindow = new SettingsWindow(this, m_configPath);
    if (args[QString("default")].toBool())
        settingsWindow->setDefault();
    m_configuration = settingsWindow->getSettings();
    delete settingsWindow;
    settingsWindow = nullptr;

    createObjects();
    createActions();

    show();
}


void QuadroMainWindow::createContainer(const QStringList exec, const QString name)
{
    qCDebug(LOG_UI) << "Executable" << exec;
    qCDebug(LOG_UI) << "Name" << name;

    StandaloneAppWidget *app = new StandaloneAppWidget(this, exec, ui->stackedWidget->count());
    connect(app, SIGNAL(destroyWindow(const int)), this, SLOT(removeContainer(const int)));

    ui->stackedWidget->addWidget(app);
    tabActions.append(ui->toolBar->addAction(name));

    return ui->stackedWidget->setCurrentWidget(app);
}


void QuadroMainWindow::createWebContainer(const QString url, const bool showOpen)
{
    qCDebug(LOG_UI) << "Create URL" << url << "with show open button" << showOpen;

    WebAppWidget *app = new WebAppWidget(this, ui->stackedWidget->count(),
                                         showOpen);
    connect(app, SIGNAL(destroyWindow(const int)), this, SLOT(removeContainer(const int)));
    connect(app, &WebAppWidget::updateTitle, [this](const int index, const QString title) {
        tabActions[index]->setText(title);
    });

    ui->stackedWidget->addWidget(app);
    tabActions.append(ui->toolBar->addAction(QString("about:blank")));
    app->loadUrl(QUrl::fromUserInput(url));

    return ui->stackedWidget->setCurrentWidget(app);
}


void QuadroMainWindow::removeContainer(const int index)
{
    qCDebug(LOG_UI) << "Remove tab" << index;

    ui->stackedWidget->setCurrentIndex(0);
    QWidget *widget = ui->stackedWidget->widget(index);
    ui->stackedWidget->removeWidget(widget);
    ui->toolBar->removeAction(tabActions[index]);
    tabActions.removeAt(index);

    widget->deleteLater();
}


void QuadroMainWindow::changeTabByAction(QAction *action)
{
    return changeTab(tabActions.indexOf(action));
}


void QuadroMainWindow::clearTabs()
{
    QStringList tabs = m_configuration[QString("Tabs")].toStringList();
    disconnect(ui->toolBar, SIGNAL(actionTriggered(QAction *)),
               this, SLOT(changeTabByAction(QAction *)));

    tabActions.clear();
    ui->toolBar->clear();
    while (ui->stackedWidget->count() > 0) {
        QWidget *widget = ui->stackedWidget->widget(0);
        ui->stackedWidget->removeWidget(widget);
    }
    for (auto plugin : m_plugins) {
        int index = plugin[QString("Index")].toInt();
        QString tab = plugin[QString("Plugin")].toString();
        QString config = m_core->plugin()->configurationPath(
            QString("%1.tab-%2.conf").arg(tab).arg(tabs.indexOf(tab)));
        m_core->plugin()->unloadPlugin(index, config);
    }
}


void QuadroMainWindow::initTabs()
{
    QStringList tabs = m_configuration[QString("Tabs")].toStringList();

    for (auto tab : tabs) {
        int index = m_core->plugin()->loadPlugin(tab);
        if (index == -1) {
            qCWarning(LOG_UI) << "Could not find tab" << tab;
        } else {
            m_core->plugin()->initPlugin(index,
                                         QString("%1.tab-%2.conf").arg(tab).arg(tabs.indexOf(tab)));
            auto item = m_core->plugin()->plugin<TabPluginInterface>(index);
            ui->stackedWidget->addWidget(item->widget());
            tabActions.append(ui->toolBar->addAction(item->name()));
            // generate metadata
            QVariantHash metadata;
            metadata[QString("Index")] = index;
            metadata[QString("Plugin")] = tab;
            m_plugins.append(metadata);
        }
    }

    connect(ui->toolBar, SIGNAL(actionTriggered(QAction *)),
            this, SLOT(changeTabByAction(QAction *)));
}


void QuadroMainWindow::createActions()
{
    connect(this, SIGNAL(needToBeConfigured()), this, SLOT(showSettingsWindow()));

    // menu
    connect(ui->actionSettings, SIGNAL(triggered()), this, SLOT(showSettingsWindow()));
//     connect(ui->actionDBus_API, SIGNAL(triggered()), this, SLOT(showApi()));
//     connect(ui->actionLibrary, SIGNAL(triggered()), this, SLOT(showLibrary()));
//     connect(ui->actionReport_a_bug, SIGNAL(triggered()), this, SLOT(reportABug()));
//     connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(showAboutWindow()));
    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(closeMainWindow()));
}


void QuadroMainWindow::createDBusSession()
{
    QDBusConnection bus = QDBusConnection::sessionBus();
    if (!bus.registerService(DBUS_SERVICE)) {
        qCWarning(LOG_UI) << "Could not register service";
        qCWarning(LOG_UI) << bus.lastError().message();
    }
    if (!bus.registerObject(DBUS_UI_OBJECT_PATH, new QuadroUiAdaptor(this),
                            QDBusConnection::ExportAllContents)) {
        qCWarning(LOG_UI) << "Could not register GUI object";
        qCWarning(LOG_UI) << bus.lastError().message();
    }
}


void QuadroMainWindow::createObjects()
{
    // backend
    m_core = new QuadroCore(this);
    createDBusSession();

    // frontend
    initTabs();
    ui->retranslateUi(this);
    settingsWindow = new SettingsWindow(this, m_configPath);
}


void QuadroMainWindow::deleteObjects()
{
    // frontend
    delete settingsWindow;
    settingsWindow = nullptr;
    clearTabs();

    // backend
    delete m_core;
    m_core = nullptr;

    QDBusConnection::sessionBus().unregisterObject(DBUS_UI_OBJECT_PATH);
    QDBusConnection::sessionBus().unregisterService(DBUS_SERVICE);
}
