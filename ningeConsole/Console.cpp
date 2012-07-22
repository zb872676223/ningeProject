/*************************************************************************
 *
 * Copyright (C) 2012 Ninsun, All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *************************************************************************/

#include "Console.h"
#include "ui_Console.h"
#include "NingeConsole.h"

#include <QtGui/QAction>

Console::Console(NingeConsole *console, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Console),
    m_pConsole(console)
{
    ui->setupUi(this);
    if(QSystemTrayIcon::isSystemTrayAvailable())
    {
        m_pSystemTrayIcon = new QSystemTrayIcon(QIcon(":/ningeConsole/resource/image/ninge-logo-32.png"));
        connect(m_pSystemTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
                this, SLOT(activated(QSystemTrayIcon::ActivationReason)));
        m_pSystemTrayIcon->show();

        QAction *_action = m_menu.addAction(QIcon(":/ningeConsole/resource/image/close.png"), "Exit", this, SLOT(exitNinge()));
        _action->setIconVisibleInMenu(true);
        m_pSystemTrayIcon->setContextMenu(&m_menu);
    }
}

Console::~Console()
{
    delete ui;
    delete m_pSystemTrayIcon;
}

void Console::init()
{
//    show();
}

QString Console::pluginName()
{
    return "ningeConsole";
}

QString Console::pluginVersion()
{
    return "1.0";
}

QString Console::pluginDetail()
{
    return "A console plugin. Post commands to other plugins.";
}

QWidget *Console::pluginSettingWidget()
{
    return NULL;
}

QWidget *Console::pluginMainWidget()
{
    return this;
}

QObject *Console::pluginInnerObject(const QString &/*name*/)
{
    return NULL;
}

QVariant Console::exec(const QString &/*command*/, const QList<QVariant> &/*arguments*/)
{
    return "command not found in ningeConsole";
}

void Console::aboutToQuit()
{
    m_pSystemTrayIcon->hide();
}

void Console::on_post_clicked()
{
    QString _plugin = ui->plugin->text();
    QString _command = ui->command->text();
    QList<QVariant> _arguments;
    QStringListIterator _argIt(ui->arguments->toPlainText().split("\n"));
    while(_argIt.hasNext())
    {
        _arguments << _argIt.next();
    }
    m_pConsole->postCommand(_plugin, _command, _arguments);
    on_clear_clicked();
}

void Console::on_clear_clicked()
{
    ui->plugin->clear();
    ui->command->clear();
    ui->arguments->clear();
    ui->plugin->setFocus();
}

void Console::activated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
    case QSystemTrayIcon::MiddleClick:
        show();
        break;
    default:
        ;
    }
}

void Console::exitNinge()
{
    m_pConsole->postCommand("pluginManager", "exit");
}

void Console::closeEvent(QCloseEvent *event)
{
    hide();
    event->ignore();
}
