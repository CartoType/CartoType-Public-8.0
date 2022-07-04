/*
cartotype_qt_map_renderer.h

Copyright (C) 2017 CartoType Ltd.
See www.cartotype.com for more information.
*/

#pragma once

#include <QOpenGLFunctions>
#include "cartotype.h"

namespace CartoTypeCore
{

class CVectorTileServer;

/** A graphics-accelerated map renderer for use with Qt. */
class CQtMapRenderer
    {
    public:
    CQtMapRenderer(Framework& aFramework);
    void Init();
    void Draw();

    private:
    Framework& m_framework;
    std::shared_ptr<CVectorTileServer> m_vector_tile_server;
    };

}
