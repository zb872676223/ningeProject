/**************************************************************************
 *
 * Copyright (C) 2011 Ninsun
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
#include <QtGui/QMenu>
#include "NingePlayer.h"

NingePlayer::NingePlayer()
  : m_pPlayer(new Player())
{
}

void NingePlayer::init()
{
  m_pPlayer->init();
  QList<QVariant> _args;

  _args.clear();
  _args << QVariant::fromValue<QObject *>(m_pPlayer->getMenu());
  emit sendCommand("ningeMain", "addMenu", _args);

  _args.clear();
  _args << QVariant::fromValue<QObject *>(m_pPlayer->pluginMainWidget()) << "center";
  emit sendCommand("ningeMain", "addWidget", _args);
}

QString NingePlayer::pluginName()
{
  return m_pPlayer->pluginName();
}

QString NingePlayer::pluginVersion()
{
  return m_pPlayer->pluginVersion();
}

QString NingePlayer::pluginDetail()
{
  return m_pPlayer->pluginDetail();
}

QWidget * NingePlayer::pluginSettingWidget()
{
  return m_pPlayer->pluginSettingWidget();
}

QWidget * NingePlayer::pluginMainWidget()
{
  return m_pPlayer->pluginMainWidget();
}

QObject * NingePlayer::pluginInnerObject(const QString &name)
{
  return m_pPlayer->pluginInnerObject(name);
}

QVariant NingePlayer::exec(const QString &command, const QList<QVariant> &arguments)
{
  return m_pPlayer->exec(command, arguments);
}

void NingePlayer::aboutToQuit()
{
  m_pPlayer->aboutToQuit();
}

Q_EXPORT_PLUGIN2 ( ningePlayer, NingePlayer )
