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

#include "NMLReader.h"
#include "PluginManager.h"

#include <QtCore/QFile>
#include <QtCore/QDebug>

using namespace ninge;

NMLReader * NMLReader::m_pInstance = NULL;

NMLReader * NMLReader::instance()
{
  if (m_pInstance)
  {
    return m_pInstance;
  }
  else
  {
    m_pInstance = new NMLReader();
    return m_pInstance;
  }
}

NMLReader::NMLReader()
{
}

void NMLReader::loadNMLFile(const QString &NMLFile)
{
  m_document.clear();
  QFile _file(NMLFile);
  if(!_file.open(QFile::ReadOnly))
  {
    qWarning() << tr("File open error.");
    return;
  }
  if(m_document.setContent(&_file, &m_strErrorMsg, &m_iErrorLine, &m_iErrorColumn))
  {
    qWarning() << tr("NML read error.");
    return;
  }
  m_currentFileName = NMLFile;
  m_currentBlock.clear();
  m_currentCommand.clear();
}

void NMLReader::moveToBlock(const QString &ID)
{
  QDomNodeList _list = m_document.elementsByTagName("Block");
  for(int i=0; i<_list.count(); i++)
  {
    if (_list.at(i).toElement().attribute("ID") == ID)
    {
      m_currentBlock = _list.at(i).toElement();
      m_currentCommand.clear();
      return;
    }
  }
}

void NMLReader::runNext()
{
  if (m_currentBlock.isNull())
  {
    m_currentBlock = m_document.firstChildElement("Block");
    if (m_currentBlock.isNull())
    {
      // 当前文档没有Block节点, 退出
      return;
    }
  }

  if(m_currentCommand.isNull())
  {
    m_currentCommand = m_currentBlock.firstChildElement("Command");
    while(m_currentCommand.isNull())
    {
      // 当前Block没有Command信息
      m_currentBlock = m_currentBlock.nextSiblingElement("Block");
      if (m_currentBlock.isNull())
      {
        // 没有后续的Block节点了，于是退出
        return;
      }
      // 获得Block的第一个命令
      m_currentCommand = m_currentBlock.firstChildElement("Command");
    }
  }
  else
  {
    if (m_currentCommand == m_currentBlock.lastChildElement("Command"))
    {
      // 达到了当前Block的最后一个命令
      do
      {
        // 首先获得下一个Block
        m_currentBlock = m_currentBlock.nextSiblingElement("Block");
        if (m_currentBlock.isNull())
        {
          // 没有后续的Block节点了，于是退出
          return;
        }
        // 获得Block的第一个命令
        m_currentCommand = m_currentBlock.firstChildElement("Command");
      } while(m_currentCommand.isNull()); // 当前命令不存在时继续查找
    }
    else
    {
      // 当前Block还存在其他命令
      m_currentCommand = m_currentCommand.nextSiblingElement("Command");
    }
  }

  doCommand(m_currentCommand);
}

void NMLReader::jump(const QString &block, const QString &file)
{
  if (!file.isEmpty())
  {
    loadNMLFile(file);
  }
  if (!block.isEmpty())
  {
    moveToBlock(block);
  }
  runNext();
}

void NMLReader::doCommand(const QDomElement &command)
{
  if (command.attribute("type") == "jump")
  {
    jump(command.attribute("block"), command.attribute("file"));
  }
  else if (command.attribute("type") == "exec")
  {
    QString _plugin = command.attribute("plugin");
    QString _command = command.attribute("command");
    QList<QVariant> _arguments;
    QDomNodeList _list = command.elementsByTagName("argument");
    for(int i=0; i<_list.count(); i++)
    {
      _arguments << _list.at(i).toElement().text();
    }
    PluginManager::instance()->sendCommand(_plugin, _command, _arguments);
  }
}
