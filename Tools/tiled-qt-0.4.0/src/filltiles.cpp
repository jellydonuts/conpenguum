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

#include "filltiles.h"

#include "tilelayer.h"
#include "tilepainter.h"

#include <QCoreApplication>

using namespace Tiled;
using namespace Tiled::Internal;

FillTiles::FillTiles(MapDocument *mapDocument,
                     TileLayer *tileLayer,
                     const QRegion &fillRegion,
                     const TileLayer *fillStamp)
    : QUndoCommand(QCoreApplication::translate("Undo Commands", "Fill Area"))
    , mMapDocument(mapDocument)
    , mTileLayer(tileLayer)
    , mFillRegion(fillRegion)
    , mOriginalTiles(tileLayer->copy(mFillRegion))
    , mFillStamp(static_cast<TileLayer*>(fillStamp->clone()))
{
}

FillTiles::~FillTiles()
{
    delete mOriginalTiles;
    delete mFillStamp;
}

void FillTiles::undo()
{
    const QRect boundingRect = mFillRegion.boundingRect();
    TilePainter painter(mMapDocument, mTileLayer);
    painter.setTiles(boundingRect.x(),
                     boundingRect.y(),
                     mOriginalTiles,
                     mFillRegion);
}

void FillTiles::redo()
{
    TilePainter painter(mMapDocument, mTileLayer);
    painter.drawStamp(mFillStamp, mFillRegion);
}
