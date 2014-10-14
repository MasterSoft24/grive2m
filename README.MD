Grive2 Manager v0.1 (grive2m)

8 October 2014

Grive2 manager is an userspace utility to control grive2 and  its  settings management.
It placed in system tray and allow you easy and quick manage grive2 settings and as well 
as to control the launch and stopping sync.

This program uses Qt5

grive2m uses qmake to build:

1. Download source code archive
2. Unpack
3. cd to unpacking directory
4. mkdir build
5. cd build
6. qmake ../
7. make

Resulting binary file you can copy to destination folder e.g. /usr/bin

Notice for Ubuntu users:
	If application tray icon shows on top left corner instead system tray area you need make follow actions:
	
	sudo apt-add-repository ppa:gurqn/systray-trusty
	sudo apt-get update
	sudo apt-get upgrade
	
	and reboot...
	
This is known bugs. See 
http://stoflru.org/out.php?link=https://bugreports.qt-project.org/browse/QTBUG-31762?page=com.atlassian.jira.plugin.system.issuetabpanels:all-tabpanel
for details.
This problem was been reproduced at least on Ubuntu 14.04


Enjoy!

Version History:

	0.1:
		:Manual start and stop sync process.
		:Raise gui preferences dialogs for tune 
		 sync parameters and list of files for sync.
		
