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

#include "GlobalSetting.h"
#include <QtCore/QSettings>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QCoreApplication>

GlobalSetting::GlobalSetting(const QString &fileName)
{
    QString _settingFileName = fileName;
    if(_settingFileName.isEmpty())
    {
        QFileInfo _fileInfo(QCoreApplication::applicationFilePath());
        QDir _dir(QCoreApplication::applicationDirPath());
        _settingFileName = _dir.absoluteFilePath(_fileInfo.completeBaseName()+".cfg");
    }
    m_pSetting = new QSettings(_settingFileName, QSettings::IniFormat);
}

GlobalSetting::~GlobalSetting()
{
    delete m_pSetting;
}

QVariant GlobalSetting::value(const QString &key, const QVariant &val)
{
    if (!m_pSetting->contains(key))
    {
        m_pSetting->setValue(key, val);
    }

    return m_pSetting->value(key);
}

void GlobalSetting::setValue(const QString &key, const QVariant &val)
{
    m_pSetting->setValue(key, val);
}
