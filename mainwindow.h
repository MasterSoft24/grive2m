/*
    grive2m: System tray manager for grive2 and grive2-gui

    Copyright (C) 2014  Vladimir Kamensky

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation version 2
    of the License.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QStyle>
#include <QAction>
#include <QProcess>
#include <QEventLoop>
#include <QSettings>
#include <QDir>
#include <QFile>
#include <QDirIterator>
#include <QTimer>
#include <QDebug>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QMenu* mainMenu;
    QSystemTrayIcon* trayIcon;
    QAction* grive2Status;
    QTimer* stateTimer;
    QTimer* syncTimer;
    QTimer* morotoryTimer;

    bool procIsActive;
    bool guiActive;
    bool morotorySync;

    QString defDir; //need remove to production

    // settings
    QString AccountEmail;
    QString SyncDir;
    int RemoteSyncPeriod;
    bool ManualSync;
    bool NotifyState;


    bool procFinder(QString proc);
    void readSettings();

private slots:
    void start();
    void quit();
    void mngAcc();
    void mngFlist();
    void onStateTimer();
    void onSyncTimer();
    void onMorotoryTimer();

};

#endif // MAINWINDOW_H
