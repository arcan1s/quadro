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
 * @file tabpluginitem.cpp
 * Source code of quadro library
 * @author Evgeniy Alekseev
 * @copyright GPLv3
 * @bug https://github.com/arcan1s/quadro-core/issues
 */


#include "quadro/quadro.h"

#include <QSettings>


/**
 * @class TabPluginItem
 */
/**
 * @fn TabPluginItem
 */
TabPluginItem::TabPluginItem(QObject *parent)
    : QObject(parent)
{
    qCDebug(LOG_PL) << __PRETTY_FUNCTION__;

    init();
}


/**
 * @fn ~TabPluginItem
 */
TabPluginItem::~TabPluginItem()
{
    qCDebug(LOG_PL) << __PRETTY_FUNCTION__;
}


/**
 * @fn api
 */
int TabPluginItem::api()
{
    return m_api;
}


/**
 * @fn background
 */
QString TabPluginItem::background()
{
    return m_background;
}


/**
 * @fn comment
 */
QString TabPluginItem::comment()
{
    return m_comment;
}


/**
 * @fn configuration
 */
QVariantHash TabPluginItem::configuration()
{
    QVariantHash pluginSettings;
    pluginSettings[QString("Comment")] = comment();

    return pluginSettings;
}


/**
 * @fn htmlImage
 */
QString TabPluginItem::htmlImage()
{
    // TODO
//     PluginItem::ImageType type = defineImageType(background());
    QImage pluginImage;
    QString image;

//     switch (type) {
//     case ImageType::COLOR:
//         pluginImage.fill(background());
//         image = convertImage(pluginImage);
//         break;
//     case ImageType::PATH:
//         image = convertImage(pluginImage);
//         break;
//     case ImageType::HASH:
//         image = background();
//         break;
//     case ImageType::NONE:
//     default:
//         break;
//     }

    return image;
}


/**
 * @fn name
 */
QString TabPluginItem::name()
{
    return m_name;
}


/**
 * @fn ui
 */
QWidget *TabPluginItem::ui()
{
    return m_ui;
}


/**
 * @fn setComment
 */
void TabPluginItem::setComment(const QString _comment)
{
    qCDebug(LOG_PL) << "Comment" << _comment;

    m_comment = _comment;
}


/**
 * @fn readDesktop
 */
void TabPluginItem::readDesktop(const QString _desktopPath)
{
    qCDebug(LOG_PL) << "Path" << _desktopPath;

    QSettings settings(_desktopPath, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");

    settings.beginGroup(QString("Quadro tabplugin"));
    setApi(settings.value(QString("API"), m_api).toInt());
    setComment(settings.value(QString("Comment"), m_comment).toString());
    setName(settings.value(QString("Name"), m_name).toString());
    settings.endGroup();
}


/**
 * @fn readSettings
 */
void TabPluginItem::readSettings(const QString _desktopPath)
{
    qCDebug(LOG_PL) << "Path" << _desktopPath;

    QSettings settings(_desktopPath, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");

    settings.beginGroup(name());

    settings.beginGroup(QString("Core"));
    setComment(settings.value(QString("Comment"), m_comment).toString());
    settings.endGroup();

    settings.beginGroup(QString("UI"));
    settings.endGroup();

    settings.endGroup();
}


/**
 * @fn saveSettings
 */
bool TabPluginItem::saveSettings(const QString _desktopPath)
{
    qCDebug(LOG_PL) << "Path" << _desktopPath;

    QVariantHash config = configuration();
    QSettings settings(_desktopPath, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");

    settings.beginGroup(name());

    settings.beginGroup(QString("Core"));
    settings.setValue(QString("Comment"), config[QString("Comment")]);
    settings.endGroup();

    settings.beginGroup(QString("UI"));
    settings.endGroup();

    settings.endGroup();

    settings.sync();

    return settings.status() == QSettings::NoError;
}


/**
 * @fn setApi
 */
void TabPluginItem::setApi(int _api)
{
    qCDebug(LOG_PL) << "API version" << _api;
    if (_api < 1) _api = 1;
    if (_api > PLUGIN_API) _api = PLUGIN_API;

    m_api = _api;
}


/**
 * @fn setBackground
 */
void TabPluginItem::setBackground(QString _background)
{
    qCDebug(LOG_PL) << "Background" << _background;
    if (_background.isEmpty()) _background = QString("#ffffffff");

    m_background = _background;
}


/**
 * @fn setName
 */
void TabPluginItem::setName(const QString _name)
{
    qCDebug(LOG_PL) << "Name" << _name;

    m_name = _name;
}
