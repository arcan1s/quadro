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
 * @file x11adaptor.h
 * Header of quadro library
 * @author Evgeniy Alekseev
 * @copyright GPLv3
 * @bug https://github.com/arcan1s/quadro-core/issues
 */


#ifndef X11ADAPTOR_H
#define X11ADAPTOR_H

#include <QHash>
#include <QObject>
#include <QX11Info>

#ifndef MAX_PROPERTY_VALUE_LEN
#define MAX_PROPERTY_VALUE_LEN 4096
#endif /* MAX_PROPERTY_VALUE_LEN */

typedef unsigned long Window;
typedef unsigned long Atom;


/**
 * @brief The X11Adaptor class provides adaptor to the common X11 calls
 * @remark this class is adapted from wmctrl https://sites.google.com/site/tstyblo/wmctrl
 */
class X11Adaptor : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief X11Adaptor class constructor
     * @param parent         pointer to parent item
     */
    explicit X11Adaptor(QObject *parent);
    /**
     * @brief X11Adaptor class destructor
     */
    virtual ~X11Adaptor();
    /**
     * @brief get WIds by given PID
     * @param _pid           PID
     * @return list of WIds
     */
    static QList<unsigned long long> getWindowByPid(const long long _pid);
    /**
     * @brief get active windows
     * @return map of windows keys of which are Q_PIDs
     */
    QHash<long long, unsigned long long> getWindowsList();

public slots:

private:
    /**
     * @brief get client list
     * @param size           pointer to size, will be filled in the method
     * @return client list
     */
    Window *getClientList(unsigned long *size) const;
    /**
     * @brief get property by window
     * @param _win           window to which need to get property
     * @param _xaPropType    required property type
     * @param _property      property name
     * @param size           pointer to size, will be filled in the method
     * @return property
     */
    char *getPropery(const Window _win, const Atom _xaPropType,
                     const char *_property, unsigned long *size) const;
};


#endif /* X11ADAPTOR_H */
