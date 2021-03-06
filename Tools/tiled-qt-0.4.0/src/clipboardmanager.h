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

#ifndef CLIPBOARDMANAGER_H
#define CLIPBOARDMANAGER_H

#include <QObject>

class QClipboard;

namespace Tiled {

class Map;

namespace Internal {

class MapDocument;

/**
 * The clipboard manager deals with interaction with the clipboard.
 */
class ClipboardManager : public QObject
{
    Q_OBJECT

public:
    ClipboardManager(QObject *parent = 0);

    /**
     * Returns whether the clipboard has a map.
     */
    bool hasMap() const { return mHasMap; }

    /**
     * Retrieves the map from the clipboard. Returns 0 when there was no map or
     * loading failed.
     */
    Map *map() const;

    /**
     * Sets the given map on the clipboard.
     */
    void setMap(const Map *map);

    /**
     * Convenience method to copy the current selection to the clipboard.
     */
    void copySelection(const MapDocument *mapDocument);

signals:
    /**
     * Emitted when whether the clip has a map changed.
     */
    void hasMapChanged();

private slots:
    void updateHasMap();

private:
    QClipboard *mClipboard;
    bool mHasMap;
};

#endif // CLIPBOARDMANAGER_H

} // namespace Internal
} // namespace Tiled
