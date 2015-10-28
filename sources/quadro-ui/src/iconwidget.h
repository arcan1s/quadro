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


#ifndef ICONWIDGET_H
#define ICONWIDGET_H

#include <QSize>
#include <QWidget>

#include "quadro/quadro.h"


class QKeyEvent;
class QMouseEvent;

class IconWidget : public QWidget
{
    Q_OBJECT

public:
    explicit IconWidget(ApplicationItem *appItem, const QSize size,
                        QWidget *parent);
    virtual ~IconWidget();
    inline QSize convertSize(const QSize size);
    ApplicationItem *associatedItem();

signals:
    void widgetPressed();

protected:
    void keyPressEvent(QKeyEvent *pressedKey);
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    ApplicationItem *item = nullptr;
};


#endif /* ICONWIDGET_H */
