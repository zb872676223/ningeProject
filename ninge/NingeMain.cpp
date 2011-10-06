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

#include "NingeMain.h"
#include "ui_NingeMain.h"

NingeMain::NingeMain(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::NingeMain)
{
  ui->setupUi(this);

  m_pPluginManager = Core::getPluginManager();
}

NingeMain::~NingeMain()
{
  delete ui;
}

void NingeMain::on_action_About_Qt_triggered()
{
  qApp->aboutQt();
}

void NingeMain::on_action_About_ninge_triggered()
{

}
