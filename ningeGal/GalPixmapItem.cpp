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

#include "GalPixmapItem.h"

#include <QtGui/QMovie>

GalPixmapItem::GalPixmapItem(QGraphicsItem * parent)
  :QGraphicsPixmapItem(parent)
{
  m_pMovie = NULL;
}

GalPixmapItem::GalPixmapItem(const QPixmap & pixmap, QGraphicsItem * parent)
  :QGraphicsPixmapItem(pixmap, parent)
{
  m_pMovie = NULL;
}

void GalPixmapItem::setGif(const QString &url)
{
  if (m_pMovie)
  {
    m_pMovie->stop();
    disconnect(m_pMovie, SIGNAL(frameChanged(int)), this, SLOT(frameChanged(int)));
    disconnect(m_pMovie, SIGNAL(resized(QSize)), this, SLOT(resized(QSize)));
    delete m_pMovie;
  }
  m_pMovie = new QMovie(url);
  connect(m_pMovie, SIGNAL(frameChanged(int)), this, SLOT(frameChanged(int)));
  connect(m_pMovie, SIGNAL(resized(QSize)), this, SLOT(resized(QSize)));
  m_pMovie->start();
  setPixmap(m_pMovie->currentPixmap());
}

void GalPixmapItem::start()
{
  if (m_pMovie)
  {
    m_pMovie->start();
  }
}

void GalPixmapItem::stop()
{
  if (m_pMovie)
  {
    m_pMovie->stop();
  }
}

void GalPixmapItem::setPaused(bool paused)
{
  if (m_pMovie)
  {
    m_pMovie->setPaused(paused);
  }
}

void GalPixmapItem::setSpeed(int percentSpeed)
{
  if (m_pMovie)
  {
    m_pMovie->setSpeed(percentSpeed);
  }
}

void GalPixmapItem::frameChanged(int /*frameNumber*/)
{
  if (m_pMovie)
  {
    setPixmap(m_pMovie->currentPixmap());
  }
}

void GalPixmapItem::resized(const QSize &/*size*/)
{
  if (m_pMovie)
  {
    setPixmap(m_pMovie->currentPixmap());
  }
}

