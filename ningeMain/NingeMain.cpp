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

#include "NingeMain.h"

NingeMain::NingeMain()
    :m_pMain(new Main(this))
{
}

void NingeMain::init()
{
    m_pMain->init();
}

QString NingeMain::pluginName()
{
    return m_pMain->pluginName();
}

QString NingeMain::pluginVersion()
{
    return m_pMain->pluginVersion();
}

QString NingeMain::pluginDetail()
{
    return m_pMain->pluginDetail();
}

QWidget *NingeMain::pluginSettingWidget()
{
    return m_pMain->pluginSettingWidget();
}

QWidget *NingeMain::pluginMainWidget()
{
    return m_pMain->pluginMainWidget();
}

QObject *NingeMain::pluginInnerObject(const QString &name)
{
    return m_pMain->pluginInnerObject(name);
}

QVariant NingeMain::exec(const QString &command, const QList<QVariant> &arguments)
{
    return m_pMain->exec(command, arguments);
}

void NingeMain::aboutToQuit()
{
    m_pMain->aboutToQuit();
}

Q_EXPORT_PLUGIN2 ( ningeMain, NingeMain )
