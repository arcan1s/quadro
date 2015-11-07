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
/**
 * @file launchercore.cpp
 * Source code of quadro library
 * @author Evgeniy Alekseev
 * @copyright GPLv3
 * @bug https://github.com/arcan1s/quadro-core/issues
 */


#include "quadrocore/quadro.h"

#include <QDir>
#include <QProcessEnvironment>
#include <QStandardPaths>


/**
 * @class LauncherCore
 */
/**
 * @fn LauncherCore
 */
LauncherCore::LauncherCore(QObject *parent)
    : AbstractAppAggregator(parent)
{
    qCDebug(LOG_LIB) << __PRETTY_FUNCTION__;
}


/**
 * @fn ~LauncherCore
 */
LauncherCore::~LauncherCore()
{
    qCDebug(LOG_LIB) << __PRETTY_FUNCTION__;

    m_applicationsFromPaths.clear();
}


/**
 * @fn applicationsFromPaths
 */
QMap<QString, ApplicationItem *> LauncherCore::applicationsFromPaths() const
{
    return m_applicationsFromPaths;
}


/**
 * @fn applicationsBySubstr
 */
QMap<QString, ApplicationItem *> LauncherCore::applicationsBySubstr(const QString _substr) const
{
    qCDebug(LOG_LIB) << "Substring" << _substr;

    QMap<QString, ApplicationItem *> apps = AbstractAppAggregator::applicationsBySubstr(_substr);
    if (m_applicationsFromPaths.contains(_substr))
        apps[_substr] = m_applicationsFromPaths[_substr];

    return apps;
}


/**
 * @fn initApplications
 */
void LauncherCore::initApplications()
{
    // start cleanup
    dropApplications();
    m_applicationsFromPaths.clear();

    QMap<QString, ApplicationItem *> desktops = getApplicationsFromDesktops();
    foreach (const QString item, desktops.keys()) {
        if (!desktops[item]->shouldBeShown()) continue;
        addApplication(desktops[item]);
    }
    m_applicationsFromPaths = getApplicationsFromPaths();

    // cleanup
    desktops.clear();
}


/**
 * @fn getApplicationsFromDesktops
 */
QMap<QString, ApplicationItem *> LauncherCore::getApplicationsFromDesktops()
{
    QStringList filter("*.desktop");
    QStringList desktopPaths = QStandardPaths::standardLocations(QStandardPaths::ApplicationsLocation);
    // append from subdirectories
    for (int i=desktopPaths.count()-1; i>=0; i--) {
        QStringList entries = QDir(desktopPaths.at(i)).entryList(QDir::Dirs | QDir::NoDotAndDotDot);
        foreach (const QString entry, entries)
            desktopPaths.append(QString("%1/%2").arg(desktopPaths.at(i)).arg(entry));
    }
    // show
    qCInfo(LOG_LIB) << "Paths" << desktopPaths;
    QMap<QString, ApplicationItem *> items;

    for (int i=desktopPaths.count()-1; i>=0; i--) {
        QStringList entries = QDir(desktopPaths.at(i)).entryList(filter, QDir::Files);
        foreach (const QString entry, entries) {
            QString desktop = QFileInfo(QDir(desktopPaths.at(i)), entry).filePath();
            qCInfo(LOG_LIB) << "Desktop" << desktop;
            ApplicationItem *item = ApplicationItem::fromDesktop(desktop, this);
            items[item->name()] = item;
        }
    }

    return items;
}


/**
 * @fn getApplicationsFromPaths
 */
QMap<QString, ApplicationItem *> LauncherCore::getApplicationsFromPaths()
{
    QProcessEnvironment environment = QProcessEnvironment::systemEnvironment();
    QString pathVariable = environment.value(QString("PATH"));
    QStringList paths = pathVariable.split(QChar(':'));
    qCInfo(LOG_LIB) << "Paths" << paths;
    QMap<QString, ApplicationItem *> items;

    foreach (const QString path, paths) {
        QStringList entries = QDir(path).entryList(QDir::Files);
        foreach (const QString entry, entries) {
            QString executable = QFileInfo(QDir(path), entry).filePath();
            if (!QFileInfo(executable).isExecutable()) continue;
            qCInfo(LOG_LIB) << "Executable" << executable;
            ApplicationItem *item = new ApplicationItem(this, QFileInfo(executable).fileName());
            item->setExec(executable);
            items[item->name()] = item;
        }
    }

    return items;
}
