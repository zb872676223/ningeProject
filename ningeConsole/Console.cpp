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

Console::Console(NingeConsole *console, QWidget *parent) :
  QWidget(parent),
  ui(new Ui::Console),
  m_pConsole(console)
{
  ui->setupUi(this);
}

Console::~Console()
{
  delete ui;
}

void Console::init()
{
  show();
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

QObject *Console::pluginInnerObject(const QString &name)
{
  return NULL;
}

QVariant Console::exec(const QString &command, const QList<QVariant> &arguments)
{
  return "command not found in ningeConsole";
}

void Console::aboutToQuit()
{
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
}
