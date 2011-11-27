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

#ifndef CORE_H
#define CORE_H

#include "core_global.h"

#include "PluginManager.h"
#include "NMLReader.h"

class CORESHARED_EXPORT Core
{
public:
    Core();

    static ninge::PluginManager * getPluginManager() { return ninge::PluginManager::instance(); }
    static ninge::NMLReader * getNMLReader() { return ninge::NMLReader::instance(); }
};

#endif // CORE_H
