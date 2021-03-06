/*
 * Tiled Map Editor (Qt)
 * Copyright 2009 Tiled (Qt) developers (see AUTHORS file)
 *
 * This file is part of Tiled (Qt).
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 59 Temple
 * Place, Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef PROPERTIESVIEW_H
#define PROPERTIESVIEW_H

#include <QTreeView>

namespace Tiled {
namespace Internal {

/**
 * The view that displays the properties.
 */
class PropertiesView : public QTreeView
{
public:
    PropertiesView(QWidget *parent = 0);

protected:
    void keyPressEvent(QKeyEvent *event);
};

} // namespace Internal
} // namespace Tiled

#endif // PROPERTIESVIEW_H
