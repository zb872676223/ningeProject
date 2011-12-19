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

#include "GalTextItem.h"

#include <QtGui/QTextCursor>
#include <QtGui/QTextDocument>

int GalTextItem::m_iInterval = 100;

GalTextItem::GalTextItem(QGraphicsItem *parent)
  : QGraphicsTextItem(parent)
  , m_iIndex(0)
  , m_iMaxRows(100)
  , m_iTimerID(0)
{
  m_pTextCursor = new QTextCursor(document());

  QTextOption _option;
  _option.setWrapMode(QTextOption::WrapAnywhere);
  document()->setDefaultTextOption(_option);
}

void GalTextItem::setText(const QString &text)
{
  m_text = text;
}

void GalTextItem::setEffect(QString effect)
{
  effect.remove('\r').remove('\n');
  m_effect.clear();
  QStringList _effectList;
  QStringListIterator _effectIt = effect.split(" & ");
  while (_effectIt.hasNext())
  {
    _effectList = _effectIt.next().split(" @ ");
    m_effect.insertMulti(_effectList.value(0).toInt(), _effectList.value(1));
  }
}

void GalTextItem::setInterval(int interval)
{
  if (interval > 0 && interval != m_iInterval)
  {
    m_iInterval = interval;
  }
}

void GalTextItem::start()
{
  killTimer(m_iTimerID);
  m_iTimerID = startTimer(m_iInterval);
}

void GalTextItem::timerEvent(QTimerEvent * /*event*/)
{
  if (processText())
  {
    killTimer(m_iTimerID);
  }
}

bool GalTextItem::processText()
{
  if (m_iIndex < m_text.count())
  {
    // 处理效果
    processEffect();
    // 添加下一个字
    m_pTextCursor->insertText(m_text.at(m_iIndex++));
    // 判断当前行数是否超过最大行数
    if(document()->lineCount() > m_iMaxRows)
    {
      m_pTextCursor->movePosition(QTextCursor::Start);
      m_pTextCursor->movePosition(QTextCursor::Down, QTextCursor::KeepAnchor);
      m_pTextCursor->removeSelectedText();
      m_pTextCursor->movePosition(QTextCursor::End);
    }
    return false;
  }
  else
  {
    emit finished();
    return true;
  }
}

void GalTextItem::processEffect()
{
  emit currentPos(m_iIndex);
  QString _effect;
  QString _arg;
  QStringListIterator _effectIt(m_effect.values(m_iIndex));
  while(_effectIt.hasNext())
  {
    _effect = _effectIt.next();
    _arg = _effect.split(" : ").value(1, "");
    if (_effect.startsWith("setColor"))
    {
      m_textCharFormat.setForeground(QBrush(QColor(_arg)));
      m_pTextCursor->setCharFormat(m_textCharFormat);
    }
    else if (_effect.startsWith("setWeight"))
    {
      m_textCharFormat.setFontWeight(_arg.toInt());
      m_pTextCursor->setCharFormat(m_textCharFormat);
    }
    else if (_effect.startsWith("setSize"))
    {
      m_textCharFormat.setFontPointSize(_arg.toDouble());
      m_pTextCursor->setCharFormat(m_textCharFormat);
    }
    else if (_effect.startsWith("pause"))
    {
      emit pause();
      killTimer(m_iTimerID);
    }
  }
}
