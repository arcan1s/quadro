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
 * @file pluginadaptor.h
 * Header of quadro library
 * @author Evgeniy Alekseev
 * @copyright GPLv3
 * @bug https://github.com/arcan1s/quadro-core/issues
 */


#ifndef PLUGINADAPTOR_H
#define PLUGINADAPTOR_H

#include <QDBusAbstractAdaptor>
#include <QObject>

#include "config.h"


class PluginInterface;

/**
 * @brief The PluginAdaptor class provides plugin DBus adaptor
 */
class PluginAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", DBUS_PLUGIN_INTERFACE)

public:
    /**
     * @brief PluginAdaptor class constructor
     * @param parent         pointer to parent object
     * @param plugin         pointer to plugin item
     */
    explicit PluginAdaptor(QObject *parent, PluginInterface *plugin);
    /**
     * @brief PluginAdaptor class destructor
     */
    virtual ~PluginAdaptor();

public slots:
    /**
     * @brief ping interface
     * @return true if interface is active
     */
    bool Ping() const;
    // public method interface
    /**
     * @brief plugin background
     * @return background
     */
    QString Background() const;
    /**
     * @brief plugin data in text
     * @return data
     */
    QString Data() const;
    /**
     * @brief plugin name
     * @return name
     */
    QString Name() const;
    /**
     * @brief called if plugin has been clicked
     */
    void Action();
    /**
     * @brief init plugin
     */
    void Init();
    /**
     * @brief get plugin minimal size
     * @return list of minimal size in format [width, height]
     */
    QStringList MinimalSize() const;
    /**
     * @brief close the plugin
     * @param configPath     full path to settings file
     */
    void Quit(const QString configPath);
    /**
     * @brief read plugin settings from configuration file
     * @param configPath     full path to settings file
     */
    void ReadSettings(const QString configPath);
    /**
     * @brief save plugin settings to configuration file
     * @param configPath     full path to settings file
     * @return true if settings has been saved successfully
     * @return false if there was an error while settings sync
     */
    bool SaveSettings(const QString configPath);
    /**
     * @brief update data. May be called to force update
     */
    void Update();
    /**
     * @brief plugin update interval
     * @return update interval in milliseconds
     */
    int UpdateInterval() const;

private:
    // properties
    /**
     * @brief pointer to the plugin
     */
    PluginInterface *m_plugin = nullptr;
};


#endif /* PLUGINADAPTOR_H */
