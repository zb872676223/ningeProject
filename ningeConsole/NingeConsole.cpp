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

#include <QtCore/QVariant>
#include <QtCore/QtPlugin>

#include "NingeConsole.h"
#include "Console.h"

NingeConsole::NingeConsole()
  : m_pConsole(new Console(this))
{

}

void NingeConsole::init()
{
  m_pConsole->init();
}

QString NingeConsole::pluginName()
{
  return m_pConsole->pluginName();
}

QString NingeConsole::pluginVersion()
{
  return m_pConsole->pluginVersion();
}

QString NingeConsole::pluginDetail()
{
  return m_pConsole->pluginDetail();
}

QWidget *NingeConsole::pluginSettingWidget()
{
  return m_pConsole->pluginSettingWidget();
}

QWidget *NingeConsole::pluginMainWidget()
{
  return m_pConsole->pluginMainWidget();
}

QObject *NingeConsole::pluginInnerObject(const QString &name)
{
  return m_pConsole->pluginInnerObject(name);
}

QVariant NingeConsole::exec(const QString &command, const QList<QVariant> &arguments)
{
  return m_pConsole->exec(command, arguments);
}

void NingeConsole::aboutToQuit()
{
  m_pConsole->aboutToQuit();
}

Q_EXPORT_PLUGIN2 ( ningeConsole, NingeConsole )
