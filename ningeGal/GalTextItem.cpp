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
#include "core/GlobalSetting.h"

#include <QtGui/QTextCursor>
#include <QtGui/QTextDocument>
#include <QtCore/QSettings>
#include <QtCore/QTextCodec>

int GalTextItem::m_iInterval = 50;

GalTextItem::GalTextItem(QGraphicsItem *parent)
  : QGraphicsTextItem(parent)
  , m_iIndex(0)
  , m_dMaxHeight(100)
  , m_iTimerID(0)
  , m_iState(-1)
{
  m_pTextCursor = new QTextCursor(document());

  // 打开配置文件
  GlobalSetting _settings;
  // 读取配置文件
  QVariant _color = _settings.value("fontColor", "#FFFFFF");
  QVariant _weight = _settings.value("fontWeight", 75);
  QVariant _size = _settings.value("fontSize", 14);
  QVariant _font = _settings.value("font", QString::fromUtf8("Microsoft YaHei"));
  QVariant _lineSpaceing = _settings.value("lineSpaceing", 110);
  QVariant _letterSpaceing = _settings.value("letterSpaceing", 100);
  //设置字符格式
  m_textCharFormat.setForeground(QBrush(QColor(_color.toString())));
  m_textCharFormat.setFontWeight(_weight.toInt());
  m_textCharFormat.setFontPointSize(_size.toInt());
  m_textCharFormat.setFontFamily(_font.toString());
  m_textCharFormat.setFontLetterSpacing(_letterSpaceing.toReal());
  m_pTextCursor->setCharFormat(m_textCharFormat);
  // 设置段落格式
  m_textBlockFormat.setLineHeight(_lineSpaceing.toReal(), QTextBlockFormat::ProportionalHeight);
  m_pTextCursor->setBlockFormat(m_textBlockFormat);
  // 设置换行模式
  QTextOption _option;
  _option.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
  document()->setDefaultTextOption(_option);
}

void GalTextItem::setText(const QString &text)
{
  pause();
  m_text = text;
  m_text.replace("\r\n", "\n");
  m_text.replace("\r", "\n");
  m_textIt = m_text.constBegin();
  m_iIndex = 0;
  m_iState = 0;
}

void GalTextItem::setEffect(const QString &effect)
{
  QString _tmp = effect;
  _tmp.remove('\r').remove('\n');
  m_effect.clear();
  QStringList _effectList;
  QStringListIterator _effectIt = _tmp.split("&&&");
  while (_effectIt.hasNext())
  {
    _effectList = _effectIt.next().split("@@@");
    if (_effectList.size() == 2)
    {
      m_effect.insertMulti(_effectList.value(0).toInt(), _effectList.value(1));
    }
  }
}

void GalTextItem::setInterval(int interval)
{
  if (interval > 0 && interval != m_iInterval)
  {
    m_iInterval = interval;
    start();
  }
}

void GalTextItem::start()
{
  killTimer(m_iTimerID);
  m_iTimerID = startTimer(m_iInterval);
  m_iState = 1;
  emit started();
}

void GalTextItem::pause()
{
  killTimer(m_iTimerID);
  m_iState = 2;
  emit paused();
}

void GalTextItem::clear()
{
  document()->clear();
  m_pTextCursor->setCharFormat(m_textCharFormat);
  m_pTextCursor->setBlockFormat(m_textBlockFormat);
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
  if(m_textIt != m_text.end())
//  if (m_iIndex < m_text.count())
  {
    // 发送当前位置信号
    emit currentPos(m_iIndex);
    // 处理效果
    processEffect();
    // 添加下一个字
//    m_pTextCursor->insertText(m_text.at(m_iIndex++));
    m_pTextCursor->insertText(*m_textIt);
    // 判断当前是否超过最大高度
    if(document()->size().height() >= m_dMaxHeight)
    {
      m_pTextCursor->movePosition(QTextCursor::Start);
      m_pTextCursor->movePosition(QTextCursor::Down, QTextCursor::KeepAnchor);
      m_pTextCursor->removeSelectedText();
      m_pTextCursor->movePosition(QTextCursor::End);

      m_pTextCursor->setCharFormat(m_textCharFormat);
      m_pTextCursor->setBlockFormat(m_textBlockFormat);
    }
    // 下一个字符
    m_textIt++;
    // 当前位置+1
    m_iIndex++;
    return false;
  }
  else
  {
    m_iState = -1;
    emit finished();
    return true;
  }
}

void GalTextItem::processEffect()
{
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
      pause();
    }
  }
}
