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

#include "mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    defDir="";

            readSettings();

        procIsActive=false;
        guiActive=false;
        morotorySync=false;

        stateTimer=new QTimer();
        connect(stateTimer,SIGNAL(timeout()),this,SLOT(onStateTimer()));

        morotoryTimer=new QTimer();
        connect(morotoryTimer,SIGNAL(timeout()),this,SLOT(onMorotoryTimer()));


        if((RemoteSyncPeriod>0)&&(!ManualSync)&&(SyncDir!="")){
            syncTimer=new QTimer();
            syncTimer->start(RemoteSyncPeriod*1000);
            connect(syncTimer,SIGNAL(timeout()),this,SLOT(onSyncTimer()));
        }

        mainMenu = new QMenu(this);

        mainMenu->addAction("Start sync now",this,
                            SLOT(start()));

        mainMenu->addAction("Terminate sync",this,
                            SLOT(stop()));

        mainMenu->addSeparator();

        grive2Status=mainMenu->addAction("Grive2 is not active now");
        grive2Status->setEnabled(false);
        mainMenu->addSeparator();

        mainMenu->addAction("Manage sync filelist",this,
                            SLOT(mngFlist()));

        mainMenu->addAction("Manage Google account and profile",this,
                            SLOT(mngAcc()));

        mainMenu->addSeparator();

        mainMenu->addAction("Quit",this,
                            SLOT(quit()));


        trayIcon = new QSystemTrayIcon(this);
        trayIcon->setIcon(QIcon(":/grive-app-ind.svg"));
        trayIcon->setContextMenu(mainMenu);


        trayIcon->show();

        trayIcon->setToolTip("Grive2 - Google drive sync solution");
}

MainWindow::~MainWindow()
{

}

void MainWindow::readSettings(){

    // read config file and set form fields
     QSettings* sett=new QSettings(QDir::homePath()+"/.config/grive2gui.conf",QSettings::IniFormat);
     sett->setIniCodec("UTF-8");

      AccountEmail=sett->value("SyncAccount").toString();
      SyncDir=(sett->value("SyncDir").toString().toUtf8());
      RemoteSyncPeriod=sett->value("RemoteSyncPeriod").toInt();
      ManualSync=sett->value("ManualSync").toBool();
      NotifyState=sett->value("DisableNotifications").toBool();

      //procFinder("grive2");

}


void MainWindow::start(){

    if(morotorySync) return;

    bool r=this->procFinder("grive2");

    if((SyncDir!="")&&(AccountEmail!="")){
        if(!r){
            stateTimer->start(600);
            QProcess p;
            p.startDetached("grive2 -p "+SyncDir);
        }
        else{
            if(!NotifyState){
                QProcess p;
                p.startDetached("notify-send \"Another instance of Grive2 already running.\"");
            }
        }
    }
    else{
        if(!NotifyState){
            QProcess p;
            p.startDetached("notify-send \"No one account present.\nCreate account first.\"");
        }
    }
}



void MainWindow::stop(){

    bool r=this->procFinder("grive2");
    if(r){
        QProcess p;
        p.startDetached("killall grive2 -9");
    }
}



void MainWindow::runInTerm(){

    if(morotorySync) return;

    bool r=this->procFinder("grive2");

    if((SyncDir!="")&&(AccountEmail!="")){
        if(!r){
            stateTimer->start(600);
            QProcess p;
            p.startDetached("xterm grive2 -p "+SyncDir);
        }
        else{
            if(!NotifyState){
                QProcess p;
                p.startDetached("notify-send \"Another instance of Grive2 already running.\"");
            }
        }
    }
    else{
        if(!NotifyState){
            QProcess p;
            p.startDetached("notify-send \"No one account present.\nCreate account first.\"");
        }
    }

}


bool MainWindow::procFinder(QString proc){

    QDirIterator it("/proc", QDirIterator::Subdirectories);
    char* buff=(char*)malloc(1000);

    while (it.hasNext()){
        it.next();
        QFileInfo child = it.fileInfo();

        if (child.fileName() == ".." || child.fileName() == ".")
        continue;

        if(child.baseName()=="cmdline"){
            QFile f(child.absoluteFilePath());
            f.open(QIODevice::ReadOnly | QIODevice::Text);
            qint64 sz=f.read(buff,999);
            buff[sz]=0;
            QString name=buff;

            name=name.mid(name.lastIndexOf("/")+1);
            QString withParam=name;
            name=name.left(name.indexOf(" "));

            if(name==proc){
               // qDebug() << withParam;
               return true;
            }
        }

    }
    return false;
}


void MainWindow::quit(){
this->close();
}

void MainWindow::mngAcc(){
    morotoryTimer->start(1000);
    QProcess p;
    p.startDetached(defDir+"grive2-gui account");
}

void MainWindow::mngFlist(){
    morotoryTimer->start(600);
    QProcess p;
    p.startDetached(defDir+"grive2-gui filelist");
}

void MainWindow::onStateTimer(){

    bool r=this->procFinder("grive2");

    if(r&& !procIsActive){
        if(!procIsActive){
            trayIcon->setIcon(QIcon(":/grive-app-ind-active.png"));
            if(!NotifyState){
                QProcess p;
                p.startDetached("notify-send \"Grive2 are sync now\"");
            }
        }
        procIsActive=true;
    }
     if(!r&& procIsActive){
         if(procIsActive){
             trayIcon->setIcon(QIcon(":/grive-app-ind.svg"));
             if(!NotifyState){
                 QProcess p;
                 p.startDetached("notify-send \"Grive2 sync ended.\"");
             }
             stateTimer->stop();
         }
         procIsActive=false;
     }

}

void MainWindow::onSyncTimer(){

    if(morotorySync) return;

    bool r=this->procFinder("grive2");

    if((!r)&&(SyncDir!="")){
        stateTimer->start(600);
        QProcess p;
        p.startDetached("grive2 -p "+SyncDir);
    }



}

void MainWindow::onMorotoryTimer(){

    bool r=this->procFinder("grive2-gui");
    if(r){
        guiActive=true;
        morotorySync=true;
    }

     if(!r&& guiActive){
         morotoryTimer->stop();
         morotorySync=false;
         guiActive=false;
         this->readSettings();

         if(syncTimer!=0){
             disconnect(this,SLOT(onSyncTimer()));
             syncTimer->stop();
             delete(syncTimer);
         }

         if((RemoteSyncPeriod>0)&&(!ManualSync)&&(SyncDir!="")){
             syncTimer=new QTimer();
             syncTimer->start(RemoteSyncPeriod*1000);
             connect(syncTimer,SIGNAL(timeout()),this,SLOT(onSyncTimer()));
         }
     }
}



