/*************************************************************************
 *
 * Copyright (C) 2011 Ninsun, All rights reserved.
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

#include "NingeGal.h"
#include "Gal.h"

NingeGal::NingeGal()
  : m_pGal(new Gal(this))
{
}

void NingeGal::init()
{
  m_pGal->init();
}

QString NingeGal::pluginName()
{
  return m_pGal->pluginName();
}

QString NingeGal::pluginVersion()
{
  return m_pGal->pluginVersion();
}

QString NingeGal::pluginDetail()
{
  return m_pGal->pluginDetail();
}

QWidget *NingeGal::pluginSettingWidget()
{
  return m_pGal->pluginSettingWidget();
}

QWidget *NingeGal::pluginMainWidget()
{
  return m_pGal->pluginMainWidget();
}

QObject *NingeGal::pluginInnerObject(const QString &name)
{
  return m_pGal->pluginInnerObject(name);
}

QVariant NingeGal::exec(const QString &command, const QList<QVariant> &arguments)
{
  return m_pGal->exec(command, arguments);
}

void NingeGal::aboutToQuit()
{
  m_pGal->aboutToQuit();
}

void NingeGal::postCommand(const QString &plugin, const QString &command, const QList<QVariant> &arguments)
{
  emit sendCommand(plugin, command, arguments);
}

Q_EXPORT_PLUGIN2 ( ningeGal, NingeGal )
