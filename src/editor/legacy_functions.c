
/*
void draw_sprite(gui_engine_t *gui, object_t object) {
  window_t *window = &gui->window;
  pos_t relative;
  SDL_Rect shape;

  hexcode_u color;

  shape.x = window->cen.x;
  shape.y = window->cen.y;
  shape.w = object.dim.w;
  shape.h = object.dim.h;
  float focal = 400;

  relative.x = object.pos.x - player.pos.x;
  relative.y = object.pos.y - player.pos.y;
  relative.z = object.pos.z - player.pos.z;

  if (relative.z <= 0.01f)
    return;

  float screen_x = relative.x / relative.z;
  float screen_y = relative.y / relative.z;

  shape.w = (int)(object.dim.w * focal / relative.z);
  shape.h = (int)(object.dim.h * focal / relative.z);

  shape.x = window->cen.x + (int)(screen_x * focal) - shape.w / 2;
  shape.y = window->cen.y - (int)(screen_y * focal) - shape.h / 2;

  sh.dist = sqrt(pow(object.pos.x - player.pos.x, 2) +
                 pow(object.pos.y - player.pos.y, 2) +
                 pow(object.pos.z - player.pos.z, 2));

  color.code = 0xFFFFFF;
  set_color(gui->sdl2.renderer, color);
  SDL_RenderFillRect(gui->sdl2.renderer, &shape);

  color.code = 0x0000FF;
  set_color(gui->sdl2.renderer, color);
  SDL_RenderDrawRect(gui->sdl2.renderer, &shape);
}

void draw_mesh_wireframe(gui_engine_t *gui, mesh_t mesh){ //, float theta
  window_t *window = &gui->window;
  pos_t relative;
  pos_t rotated;
  pos_t pitched;
  float focal = 400;
  float screen_x, screen_y;

  hexcode_u color;
  color.code = 0xFF0000;
  set_color(gui->sdl2.renderer, color);

  // SDL_Rect verts_vis[mesh.nverts];
  point_t points[mesh.nverts];

  for(int i = 0; i<mesh.nverts; i++){
    relative.x = mesh.verts[i].x - player.pos.x;
    relative.y = mesh.verts[i].y - player.pos.y;
    relative.z = mesh.verts[i].z - player.pos.z;

    rotated.x = relative.x * cos(theta) - relative.z * sin(theta);
    rotated.y = relative.y;
    rotated.z = relative.x * sin(theta) + relative.z * cos(theta);

    pitched.x = rotated.x;
    pitched.y = rotated.y * cos(phi) - rotated.z * sin(phi);
    pitched.z = rotated.y * sin(phi) + rotated.z * cos(phi);

    if(pitched.z<=0.01f){
      points[i].visible = 0;
      continue;
    }

    points[i].visible = 1;

    screen_x = pitched.x / pitched.z;
    screen_y = pitched.y / pitched.z;

    // will draw the vertices if desired
    // verts_vis[i].w = verts_vis[i].h = (int)(1 * focal / rotated.z);
    // verts_vis[i].x = window->cen.x + (int)(screen_x * focal);
    // verts_vis[i].y = window->cen.y - (int)(screen_y * focal);

    // SDL_RenderFillRect(gui->sdl2.renderer, &verts_vis[i]);

    points[i].x = window->cen.x + (int)(screen_x * focal);
    points[i].y = window->cen.y - (int)(screen_y * focal);
  }

  for(int i = 0; i<mesh.nedges; i++){
    if(points[mesh.edges[i].A].visible && points[mesh.edges[i].B].visible){
      SDL_RenderDrawLine(gui->sdl2.renderer,
                         points[mesh.edges[i].A].x,
                         points[mesh.edges[i].A].y,
                         points[mesh.edges[i].B].x,
                         points[mesh.edges[i].B].y);
    }
  }
}

*/