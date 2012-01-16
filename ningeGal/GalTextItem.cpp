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
#include <QtCore/QSettings>
#include <QtCore/QTextCodec>

int GalTextItem::m_iInterval = 100;

GalTextItem::GalTextItem(QGraphicsItem *parent)
  : QGraphicsTextItem(parent)
  , m_iIndex(0)
  , m_dMaxHeight(100)
  , m_iTimerID(0)
{
  m_pTextCursor = new QTextCursor(document());

  // 读取并初始化配置
  QVariant _color("#FFFFFF");
  QVariant _weight(75);
  QVariant _size(14);
  QVariant _font(QString::fromUtf8("黑体"));
  QSettings _settings("ninge.cfg", QSettings::IniFormat);
  _color = _settings.value("fontColor", _color);
  _settings.setValue("fontColor", _color);
  _weight = _settings.value("fontWeight", _weight);
  _settings.setValue("fontWeight", _weight);
  _size = _settings.value("fontSize", _size);
  _settings.setValue("fontSize", _size);
  _font = _settings.value("font", _font);
  _settings.setValue("font", _font);

  m_textCharFormat.setForeground(QBrush(QColor(_color.toString())));
  m_textCharFormat.setFontWeight(_weight.toInt());
  m_textCharFormat.setFontPointSize(_size.toInt());
  m_textCharFormat.setFontFamily(_font.toString());
  m_pTextCursor->setCharFormat(m_textCharFormat);

  QTextOption _option;
  _option.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
  document()->setDefaultTextOption(_option);
}

void GalTextItem::setText(const QString &text)
{
  m_text = text;
  m_text.replace("\r\n", "\n");
  m_text.replace("\r", "\n");
}

void GalTextItem::setEffect(QString effect)
{
  effect.remove('\r').remove('\n');
  m_effect.clear();
  QStringList _effectList;
  QStringListIterator _effectIt = effect.split("&&&");
  while (_effectIt.hasNext())
  {
    _effectList = _effectIt.next().split("@@@");
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
    // 判断当前是否超过最大高度
    if(document()->size().height() >= m_dMaxHeight)
    {
//      document()->clear();
      m_pTextCursor->movePosition(QTextCursor::Start);
      m_pTextCursor->movePosition(QTextCursor::Down, QTextCursor::KeepAnchor);
      m_pTextCursor->removeSelectedText();
      m_pTextCursor->movePosition(QTextCursor::End);

      m_pTextCursor->setCharFormat(m_textCharFormat);
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
    _arg = _effect.split(":::").value(1, "");
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
