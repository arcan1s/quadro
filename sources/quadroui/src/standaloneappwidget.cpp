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
 * @file standaloneappwidget.cpp
 * Source code of quadro library
 * @author Evgeniy Alekseev
 * @copyright GPLv3
 * @bug https://github.com/arcan1s/quadro-core/issues
 */


#include "quadroui/quadroui.h"
#include "ui_standaloneappwidget.h"

#include <QMdiSubWindow>

#include <quadrocore/quadro.h>


/**
 * @class StandaloneAppWidget
 */
/**
 * @fn StandaloneAppWidget
 */
StandaloneAppWidget::StandaloneAppWidget(QWidget *parent, const QStringList exec,
                                         const int index, const QVariantHash settings)
    : QMainWindow(parent),
      m_configuration(settings),
      m_exec(exec),
      m_index(index)
{
    qCDebug(LOG_UI) << __PRETTY_FUNCTION__;

    ui = new Ui::StandaloneAppWidget;
    ui->setupUi(this);

    createObjects();

    connect(m_application, SIGNAL(ready()), this, SLOT(paintWidget()));
    m_application->startApplication();
}


/**
 * @fn ~StandaloneAppWidget
 */
StandaloneAppWidget::~StandaloneAppWidget()
{
    qCDebug(LOG_UI) << __PRETTY_FUNCTION__;

    delete ui;
}


/**
 * @fn paintWidget
 */
void StandaloneAppWidget::paintWidget()
{
    QList<QWidget *> widgets = m_application->widgets();
    foreach (QWidget *widget, widgets) {
        QMdiSubWindow *subWindow = ui->mdiArea->addSubWindow(widget);
        subWindow->setAttribute(Qt::WA_DeleteOnClose);
        connect(subWindow, SIGNAL(destroyed(QObject *)), this, SLOT(subWindowDestroyed()));
        subWindow->show();
    }

    ui->mdiArea->tileSubWindows();
}


/**
 * @fn subWindowDestroyed
 */
void StandaloneAppWidget::subWindowDestroyed()
{
    if (ui->mdiArea->subWindowList().isEmpty()) {
        qCInfo(LOG_UI) << "No windows remain, close tab";
        emit(destroyWindow(m_index));
    }
}


/**
 * @fn createObjects
 */
void StandaloneAppWidget::createObjects()
{
    m_application = new StandaloneApplicationItem(this, m_exec);
}