The top down rendering for this is fairly complicated, primarily due to the
different coordinate systems at play.

Terminology
===========
    Render-Frame:
        The render-frame is the array that comprises the rendered-area. This is
        denoted as a two-dimensional array of chunks, with the dimensions being
        a square whose sides are the length of the render distance. So a render
        distance of 5 would lead to a 5x5 array of rendered chunks. This leads
        to some restrictions with how the render distance can be figured. The
        map editor relies on there being a center chunk, which means that even
        numbers are not viable render distances in the editor. This restriction
        does not apply to the game world. The index of the render-frame is
        indexed via a row-column system, stored in 8 bit integers, giving a 
        theoretical max render-distance of 256 chunks, assuming you did some
        funny shifting (i.e. 0 corresponds to 1 chunk).

    Window:
        The window is the literal window that is generated for the desktop. The
        window works on an unsigned coordinate system. In a way it is just an
        array of pixels. Drawing images on the window is through an x/y system.
        With this in mind, the position of the render-frame must be set relative
        to the window, so a 256 chunk map would not be able to fit within the
        bounds of the window unless the cell resolution was reduced drastically.
        Due to this, the render distance really does not need to be more than 5
        chunks for the current system. The translation of the render-frame onto
        the window is not very complicated, it is just an offset from the center
        of the window.

        render.pos.y = window->cen.y - render.width/2;
        render.pos.x = window->cen.x - render.width/2;

        This is the exact code for the editor. It shows the the start of the
        render-frame is just half of the width of the map offset from the
        center. The logic for the game is more complicated, due to not having a
        fixed viewpoint. It is not yet implemented well enough to record here.

    World-Map:
        The map is more interesting. It is a signed coordinate system that 
        starts from an origin 0,0 at the center of the map. You can then move
        in positive or negative directions. This is a better layout from a world
        design perspective, but it leads to complications when overlaying on the
        unsigned arrays that make up the bedrock of the renderer. For the
        edito, it does not produce a lot of issues, due to the current single
        chunk movement system, but for the game it will be much more intensive.

    Chunks:
        Chunks are the typical unit of the gameworld. Chunks are comprised of
        cells, which are currently 100 in number per chunk. The chunk is labeled
        with the signed integer coordinate corresponding to them and are stored
        individually as binary data files. The rendering of chunks is determined
        by the following.

        current_ch.y = render.pos.y + render.idx.r*chunk.width;
        current_ch.x = render.pos.x + render.idx.c*chunk.width;

        The render position is the start of the render-frame, then it is a
        simple indexing formula. Interestingly, There is no need to
        unravel the array as a flat map, since the window renders in x/y
        coordinate systems.

    Cells:
        Cells are less important. They are the smallest unit of storage on the
        map. The idea is that they function as a data type for the terrain of
        the specific region. These are likely going to be depricated in favor of
        the new 3D map graphing system