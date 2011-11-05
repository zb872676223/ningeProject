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

#ifndef NMLREADER_H
#define NMLREADER_H

#include <QtCore/QObject>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>

namespace ninge {

  class NMLReader : public QObject
  {
    Q_OBJECT

    NMLReader();
  public:
    static NMLReader* instance();

  public:
    void runNext();
    void jump(const QString &block, const QString &file=QString());

  private:
    void loadNMLFile(const QString &NMLFile);
    void moveToBlock(const QString &ID);
    void doCommand(const QDomElement &command);

  private:
    QDomDocument m_document;
    QString m_currentFileName;
    QDomElement m_currentBlock;
    QDomElement m_currentCommand;

    QString m_strErrorMsg;
    int m_iErrorLine;
    int m_iErrorColumn;

  private:
    static NMLReader *m_pInstance;
  };

}

#endif // NMLREADER_H
