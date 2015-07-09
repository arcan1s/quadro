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


#ifndef APPLAUNCHERWIDGET_H
#define APPLAUNCHERWIDGET_H

#include <QPushButton>
#include <QWidget>

#include <quadro/quadro.h>


namespace Ui {
class AppLauncher;
}

class AppLauncher : public QWidget
{
    Q_OBJECT

public:
    explicit AppLauncher(QWidget *parent,
                         QVariantMap settings = QVariantMap(),
                         const bool debugCmd = false);
    ~AppLauncher();

public slots:
    void changeCategory(const int index);

private slots:
    void changeCategoryByButton();

private:
    // ui
    QList<QPushButton *> categoryButtons;
    QList<QWidget *> categoryWidgets;
    Ui::AppLauncher *ui = nullptr;
    // backend
    void createActions();
    void createObjects();
    void deleteObjects();
    void initCategory(const QString category, const int index);
    bool debug = false;
    LauncherCore *launcher = nullptr;
    // configuration
    QVariantMap configuration;
};


#endif /* APPLAUNCHERWIDGET_H */
