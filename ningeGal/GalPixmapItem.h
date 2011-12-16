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

#ifndef GALPIXMAPITEM_H
#define GALPIXMAPITEM_H

#include <QGraphicsPixmapItem>

class QMovie;

class GalPixmapItem : public QObject, public QGraphicsPixmapItem
{
  Q_OBJECT

public:
  GalPixmapItem(QGraphicsItem * parent = 0);
  GalPixmapItem(const QPixmap & pixmap, QGraphicsItem * parent = 0);

public:
  void setGif(const QString &url);

public slots:
  void start();
  void stop();
  void setPaused(bool paused);
  void setSpeed(int percentSpeed);

private slots:
  void frameChanged(int frameNumber);
  void resized(const QSize &size);

private:
  QMovie *m_pMovie;
};

#endif // GALPIXMAPITEM_H
