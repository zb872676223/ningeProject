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

#ifndef GALTEXTITEM_H
#define GALTEXTITEM_H

#include <QtGui/QGraphicsTextItem>
#include <QtGui/QTextCharFormat>
#include <QtGui/QTextBlockFormat>
#include <QtCore/QString>

class QTimerEvent;
class QTextCursor;

class GalTextItem : public QGraphicsTextItem
{
  Q_OBJECT

public:
  GalTextItem(QGraphicsItem * parent = 0);

public:
  void setText(const QString &text);
  void setEffect(const QString &effect);

  int maxRows() { return m_dMaxHeight; }
  void setMaxHeight(qreal val) { m_dMaxHeight = val; }

  int state() { return m_iState; }

public slots:
  void start();
  void pause();
  void clear();
  void setInterval(int interval);

signals:
  void started();
  void paused();
  void finished();
  void currentPos(int);

private:
  virtual void timerEvent(QTimerEvent *event);
  bool processText();
  void processEffect();

private:
  QString m_text;
  QMap<int, QString> m_effect;
  int m_iIndex;
  QString::ConstIterator m_textIt;
  int m_dMaxHeight;

  static int m_iInterval;
  int m_iTimerID;

  int m_iState;

  QTextCursor *m_pTextCursor;
  QTextCharFormat m_textCharFormat;
  QTextBlockFormat m_textBlockFormat;
};

#endif // GALTEXTITEM_H
