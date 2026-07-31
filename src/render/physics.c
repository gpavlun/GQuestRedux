#include "physics.h"
#include "render.h"




#define DRAG_CONSTANT 200
#define planetary_mass 10000000000000000000000000
#define planitary_diam 12756000
#define gravitational_const 0.000000000067408
#define cheat_grav 9.8f
#define $friction_coeff 2.0f

#define sensitivity 0.5f




actor_t *player_glob;
camera_t *camera_glob;

float phys_clamp(float value, float min, float max){
  if(value>max) return max;
  if(value<min) return min;
  return value;
}
void flying_movement_physics(actor_t *actor, float dt){
    float c = cosf(actor->theta);
    float s = sinf(actor->theta);

    vec3 desired_world;
    desired_world.x = actor->desired_velocity.x * c +
                      actor->desired_velocity.z * s;
    desired_world.y = actor->desired_velocity.y;
    desired_world.z = -actor->desired_velocity.x * s +
                       actor->desired_velocity.z * c;



    actor->applied_force.x = actor->max_force *
              (desired_world.x - actor->vel.x);
    actor->applied_force.y = actor->max_force *
              (desired_world.y - actor->vel.y);
    actor->applied_force.z = actor->max_force *
              (desired_world.z - actor->vel.z);

    float force_len = sqrtf(
        actor->applied_force.x * actor->applied_force.x +
        actor->applied_force.y * actor->applied_force.y +
        actor->applied_force.z * actor->applied_force.z
    );

    if(force_len > actor->max_force){
        float scale = actor->max_force / force_len;
        actor->applied_force.x *= scale;
        actor->applied_force.y *= scale;
        actor->applied_force.z *= scale;
    }

    float normal_force = 0.0f;
    float friction_force;

    normal_force = actor->mass * cheat_grav;
    friction_force = 3 * $friction_coeff * normal_force;

    vec3 net_force;
    net_force.x = actor->applied_force.x;
    net_force.y = actor->applied_force.y;
    net_force.z = actor->applied_force.z;
    float d3_force = sqrtf(
        net_force.x * net_force.x +
        net_force.y * net_force.y +
        net_force.z * net_force.z
    );
    if(d3_force > friction_force){
      float scale = friction_force / d3_force;
      net_force.x *= scale;
      net_force.y *= scale;
      net_force.z *= scale;
    }

    actor->acc.x = net_force.x / actor->mass;
    actor->acc.y = net_force.y / actor->mass;
    actor->acc.z = net_force.z / actor->mass;

    actor->vel.x += actor->acc.x * dt;
    actor->vel.y += actor->acc.y * dt;
    actor->vel.z += actor->acc.z * dt;

    actor->pos.x += actor->vel.x * dt;
    actor->pos.y += actor->vel.y * dt;
    actor->pos.z += actor->vel.z * dt;

    actor->net_force = net_force;
}







float terrain_height(vec3 pos){
  while(!glob_terrain);
  
  // bilinear interpolation

  float local_x = pos.x - glob_terrain->object.pos.x;
  float local_z = pos.z - glob_terrain->object.pos.z;
  
  float gx = local_x + $chunk_quads * 0.5f;
  float gz = local_z + $chunk_quads * 0.5f;
  
  int x0 = floorf(gx);
  int z0 = floorf(gz);

  int x1 = x0 + 1;
  int z1 = z0 + 1;
  
  if (x0 < 0 || x1 >= $chunk_verts ||
      z0 < 0 || z1 >= $chunk_verts) {
      return 0.0f;
  }

  float tx = gx - x0;
  float tz = gz - z0;

  float h00 = glob_terrain->heightmap[z0 * $chunk_verts + x0];
  float h10 = glob_terrain->heightmap[z0 * $chunk_verts + x1];

  float h01 = glob_terrain->heightmap[z1 * $chunk_verts + x0];
  float h11 = glob_terrain->heightmap[z1 * $chunk_verts + x1];
  
  float top = h00 + tx * (h10 - h00);

  float bottom = h01 + tx * (h11 - h01);

  float height = top + tz * (bottom - top);
  return height;
}

#define $sample_distance 1.0f
vec3 terrain_normal(vec3 pos){
  float hL = terrain_height((vec3){pos.x - $sample_distance, 0, pos.z});
  float hR = terrain_height((vec3){pos.x + $sample_distance, 0, pos.z});

  float hD = terrain_height((vec3){pos.x, 0, pos.z - $sample_distance});
  float hU = terrain_height((vec3){pos.x, 0, pos.z + $sample_distance});

  float dx = (hR - hL) / (2.0f * $sample_distance);
  float dz = (hU - hD) / (2.0f * $sample_distance);

  vec3 normal;

  normal.x = -dx;
  normal.y = 1.0f;
  normal.z = -dz;

  return vec3_normalize(normal);
}


void movement_physics(actor_t *actor, float dt){

    float height = terrain_height(actor->pos);
    vec3 normal = terrain_normal(actor->pos);

    float c = cosf(actor->theta);
    float s = sinf(actor->theta);

    vec3 desired_world;
    desired_world.x = actor->desired_velocity.x * c +
                      actor->desired_velocity.z * s;
    desired_world.z = -actor->desired_velocity.x * s +
                       actor->desired_velocity.z * c;
    desired_world.y = actor->desired_velocity.y;
    
    float into_surface = vec3_dot(desired_world, normal);

    desired_world = vec3_sub(desired_world, vec3_scale(normal, into_surface));

    /* motor force */
    actor->applied_force = vec3_scale(vec3_sub(desired_world, actor->vel), actor->max_force);

    float force_len = vec3_length(actor->applied_force);
    if(force_len > actor->max_force){
        actor->applied_force = vec3_scale(actor->applied_force,
                                          actor->max_force / force_len);
    }

    vec3 gravity_force = {0, actor->mass * -cheat_grav, 0};
    vec3 net_force = gravity_force;
   
    float normal_mag = 0;
    vec3 normal_force = {0};

    /* contact force */
    if(actor->pos.y <= (height + 0.1f)){  

      into_surface = vec3_dot(net_force, normal);
      if(into_surface < 0){
        normal_mag = -into_surface;  
        normal_force = vec3_scale(normal, normal_mag);
      
        net_force = vec3_add(net_force, normal_force);
      }
    }

    /* friction */
    float friction_mag = $friction_coeff * normal_mag;
    vec3 tangent_force = vec3_sub(gravity_force,
                                  vec3_scale(normal,
                                  vec3_dot(gravity_force, normal)));
    
    float tangent_len = vec3_length(tangent_force);
    if(tangent_len <= friction_mag){
        net_force = vec3_sub(net_force, tangent_force);
    }else{
        vec3 friction = vec3_scale(vec3_normalize(tangent_force), -friction_mag);
        net_force = vec3_add(net_force, friction);
    }    
    

    float traction = $friction_coeff * normal_mag;// * normal.y * 10;

    
    force_len = vec3_length(actor->applied_force);
    if(force_len > traction){
        actor->applied_force = vec3_scale(actor->applied_force, traction / force_len);
    }
  
    net_force = vec3_add(net_force, actor->applied_force);


    /* integrations */
    actor->acc = vec3_scale(net_force, 1/actor->mass);
    actor->vel = vec3_add(actor->vel, vec3_scale(actor->acc, dt));
    actor->pos = vec3_add(actor->pos, vec3_scale(actor->vel, dt));

    float penetration = height - actor->pos.y / normal.y;
    if(penetration > 0){
      actor->pos = vec3_add(
          actor->pos,
          vec3_scale(normal, penetration)
      );
    }


    actor->net_force = net_force;
}

#define jump_interval 200
#define walkspeed 12
#define strafespeed (walkspeed/1.3f)
u8 flying;
void multipress(actor_t *player, inputs_t key){
  player->desired_velocity.x = player->desired_velocity.y = player->desired_velocity.z = 0;
  float input_x = 0, input_y = 0, input_z = 0;
  static u64 last_jump;

  if(key.w){
    input_z -= 1;
  }

  if(key.s){
    input_z += 1;
  }

  if(key.a){
    input_x -= 1;
  }

  if(key.d){
    input_x += 1;
  }

  if(key.space){
    input_y += 1;
  }
  if(key.shift){
    input_y -= 1;
  }

  if(input_y){
    u64 now;
    now = SDL_GetTicks64();
    if(input_y>0 && now - last_jump < jump_interval && now - last_jump > jump_interval/3){
      flying = !flying;
      if(flying){
        player->vel.x = 0;
        player->vel.y = 0;
        player->vel.z = 0;
      }
    }
    float height = terrain_height(player_glob->pos);
    if(!flying && player->pos.y<=(height + 0.1f)){
      player->vel.y = walkspeed;
    }
    last_jump = now;
  }

  float len;
  if(flying){

    len = sqrtf(input_x * input_x + input_y * input_y + input_z * input_z);
    if(len>0){
      input_x /= len;
      input_y /= len;
      input_z /= len;
    }
    player->desired_velocity.x = input_x * walkspeed * 2;
    player->desired_velocity.y = input_y * walkspeed * 2;
    player->desired_velocity.z = input_z * walkspeed * 2;

  }else{

    len = sqrtf(input_x * input_x + input_z * input_z);
    if(len>0){
      input_x /= len;
      input_z /= len;
    }
    player->desired_velocity.x = input_x * strafespeed;
    player->desired_velocity.z = input_z *  (input_z<0.0f ? walkspeed : strafespeed);
  
  }
}

void *editor_event_handler(void *args){
  SDL_Event event;
  const uint8_t *keyscan = SDL_GetKeyboardState(NULL);
  inputs_t inputs = {0};
  u64 last_movement = SDL_GetPerformanceCounter();

  u64 current_movement;
  float dt;

  actor_t player = {0};
  player_glob = &player;
  camera_t camera = {0};
  camera_glob = &camera;

  player.mass = 80;
  player.max_force = 2745;
  player.pos.y = terrain_height(player.pos) + 1;
  flying = 0;
  u8 mousemode = 0;

  while(modes.RUNNING){
    current_movement = SDL_GetPerformanceCounter();
    if(!flying) inputs.space = 0;
    dt = (float)(current_movement - last_movement) /
         (float)SDL_GetPerformanceFrequency();

    while(SDL_PollEvent(&event)){
      switch(event.type){
        case(SDL_QUIT):{

          modes.RUNNING = 0;

        $}case(SDL_KEYUP):{
          switch(event.key.keysym.sym){
            case(SDLK_ESCAPE):{
              modes.RUNNING = 0;
            $}case(SDLK_p):{
              modes.WIREFRAME = !modes.WIREFRAME;
            $}default:{$}
          }
        $}case(SDL_KEYDOWN):{
          switch(event.key.keysym.sym){
            case(SDLK_SPACE):{
              if(!flying && !event.key.repeat){
                inputs.space = 1;
              }
            $}default:{$}
          }

        $}case(SDL_MOUSEMOTION):{
          if(mousemode){
            camera.theta -= (float)event.motion.xrel * 0.01f;
            camera.phi   += (float)event.motion.yrel * 0.01f;

            if(camera.phi>1.55) camera.phi = 1.55f;
            else if(camera.phi<-1.55) camera.phi = -1.55f;
          }
        $}case(SDL_MOUSEWHEEL):{
          if(mousemode){

            camera.psi -= (float)event.wheel.y * 0.1f;

            if(camera.psi > 1.55) camera.psi  = 1.55f;
            else if(camera.psi < -1.55) camera.psi = -1.55f;
          }
        $}case(SDL_MOUSEBUTTONUP):{
          if(mousemode) SDL_SetRelativeMouseMode(SDL_FALSE);
          else SDL_SetRelativeMouseMode(SDL_TRUE);

          mousemode = !mousemode;
        $}default:{$}
      }

      inputs.w = keyscan[SDL_SCANCODE_W];
      inputs.a = keyscan[SDL_SCANCODE_A];
      inputs.s = keyscan[SDL_SCANCODE_S];
      inputs.d = keyscan[SDL_SCANCODE_D];

      if(flying){
        inputs.shift = keyscan[SDL_SCANCODE_LSHIFT];
        inputs.space = keyscan[SDL_SCANCODE_SPACE];
      }

    }
    multipress(&player, inputs);
    player.theta = camera.theta;
    player.phi = camera.phi;
    if(flying){
      flying_movement_physics(&player, dt);
    }else{
      movement_physics(&player, dt);
    }
    camera.pos = player.pos;
    camera.pos.y += 1.8f;


    last_movement = current_movement;
    usleep(event_rate);
  }

  pthread_exit(0);
}



