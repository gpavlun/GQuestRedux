#include "physics.h"

#include "logging.h"
#include "../rendering/render.h"
#include "../main/game.h"


#define DRAG_CONSTANT 200
#define planetary_mass 10000000000000000000000000
#define planitary_diam 12756000
#define gravitational_const 0.000000000067408
#define cheat_grav 9.8f
#define $friction_coeff 3.5f

#define sensitivity 0.5f


float phys_clamp(float value, float min, float max){
  if(value>max) return max;
  if(value<min) return min;
  return value;
}
void flying_movement_physics(entity_t *actor, float dt){
    float c = cosf(-actor->transform.rot.y);
    float s = sinf(-actor->transform.rot.y);

    vec3 desired_world;
    desired_world.x = actor->physics_actor.desired_velocity.x * c +
                      actor->physics_actor.desired_velocity.z * s;
    desired_world.y = actor->physics_actor.desired_velocity.y;
    desired_world.z =-actor->physics_actor.desired_velocity.x * s +
                      actor->physics_actor.desired_velocity.z * c;



    actor->physics_actor.applied_force.x = actor->physics_actor.max_force *
              (desired_world.x - actor->physics_actor.vel.x);
    actor->physics_actor.applied_force.y = actor->physics_actor.max_force *
              (desired_world.y - actor->physics_actor.vel.y);
    actor->physics_actor.applied_force.z = actor->physics_actor.max_force *
              (desired_world.z - actor->physics_actor.vel.z);

    float force_len = sqrtf(
        actor->physics_actor.applied_force.x * actor->physics_actor.applied_force.x +
        actor->physics_actor.applied_force.y * actor->physics_actor.applied_force.y +
        actor->physics_actor.applied_force.z * actor->physics_actor.applied_force.z
    );

    if(force_len > actor->physics_actor.max_force){
        float scale = actor->physics_actor.max_force / force_len;
        actor->physics_actor.applied_force.x *= scale;
        actor->physics_actor.applied_force.y *= scale;
        actor->physics_actor.applied_force.z *= scale;
    }

    float normal_force = 0.0f;
    float friction_force;

    normal_force = actor->physics_actor.mass * cheat_grav;
    friction_force = 3 * $friction_coeff * normal_force;

    vec3 net_force;
    net_force.x = actor->physics_actor.applied_force.x;
    net_force.y = actor->physics_actor.applied_force.y;
    net_force.z = actor->physics_actor.applied_force.z;
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

    actor->physics_actor.acc.x = net_force.x / actor->physics_actor.mass;
    actor->physics_actor.acc.y = net_force.y / actor->physics_actor.mass;
    actor->physics_actor.acc.z = net_force.z / actor->physics_actor.mass;

    actor->physics_actor.vel.x += actor->physics_actor.acc.x * dt;
    actor->physics_actor.vel.y += actor->physics_actor.acc.y * dt;
    actor->physics_actor.vel.z += actor->physics_actor.acc.z * dt;

    actor->transform.pos.x += actor->physics_actor.vel.x * dt;
    actor->transform.pos.y += actor->physics_actor.vel.y * dt;
    actor->transform.pos.z += actor->physics_actor.vel.z * dt;

    actor->physics_actor.net_force = net_force;
}







float terrain_height(vec3 pos, logical_chunk_t *terrain){
  // bilinear interpolation

  float local_x = pos.x - terrain->pos.x;
  float local_z = pos.z - terrain->pos.z;
  
  float gx = local_x + $chunk_quads * 0.5f;
  float gz = local_z + $chunk_quads * 0.5f;
  
  int x0 = (int)floorf(gx);
  int z0 = (int)floorf(gz);

  int x1 = x0 + 1;
  int z1 = z0 + 1;
  
  if (x0 < 0 || x1 >= $chunk_verts ||
      z0 < 0 || z1 >= $chunk_verts) {
      return 0.0f;
  }

  float tx = gx - (float)x0;
  float tz = gz - (float)z0;

  float h00 = terrain->heightmap[z0 * $chunk_verts + x0];
  float h10 = terrain->heightmap[z0 * $chunk_verts + x1];

  float h01 = terrain->heightmap[z1 * $chunk_verts + x0];
  float h11 = terrain->heightmap[z1 * $chunk_verts + x1];
  
  float top = h00 + tx * (h10 - h00);

  float bottom = h01 + tx * (h11 - h01);

  float height = top + tz * (bottom - top);
  return height;
}

#define $sample_distance 1.0f
vec3 terrain_normal(vec3 pos, logical_chunk_t *terrain){
  float hL = terrain_height((vec3){pos.x - $sample_distance, 0, pos.z}, terrain);
  float hR = terrain_height((vec3){pos.x + $sample_distance, 0, pos.z}, terrain);

  float hD = terrain_height((vec3){pos.x, 0, pos.z - $sample_distance}, terrain);
  float hU = terrain_height((vec3){pos.x, 0, pos.z + $sample_distance}, terrain);

  float dx = (hR - hL) / (2.0f * $sample_distance);
  float dz = (hU - hD) / (2.0f * $sample_distance);

  vec3 normal;

  normal.x = -dx;
  normal.y = 1.0f;
  normal.z = -dz;

  return vec3_normalize(normal);
}


void movement_physics(entity_t *actor, logical_chunk_t *terrain, float dt){

    float height = terrain_height(actor->transform.pos, terrain);
    vec3 normal  = terrain_normal(actor->transform.pos, terrain);

    float c = cosf(-actor->transform.rot.y);
    float s = sinf(-actor->transform.rot.y);

    vec3 desired_world;
    desired_world.x = actor->physics_actor.desired_velocity.x * c +
                      actor->physics_actor.desired_velocity.z * s;
    desired_world.z = -actor->physics_actor.desired_velocity.x * s +
                       actor->physics_actor.desired_velocity.z * c;
    desired_world.y = actor->physics_actor.desired_velocity.y;

    float into_surface = vec3_dot(desired_world, normal);

    desired_world = vec3_sub(desired_world, vec3_scale(normal, into_surface));


    /* motor force */
    actor->physics_actor.applied_force =
      vec3_scale(
        vec3_sub(desired_world, actor->physics_actor.vel),
        actor->physics_actor.max_force
      );

    float force_len = vec3_length(actor->physics_actor.applied_force);
  
    if(force_len > actor->physics_actor.max_force){
        actor->physics_actor.applied_force =
          vec3_scale(
            actor->physics_actor.applied_force,
            actor->physics_actor.max_force / force_len
          );
    }

    vec3 gravity_force = {0, actor->physics_actor.mass * -cheat_grav, 0};
    vec3 net_force = gravity_force;
   
    float normal_mag = 0;

    /* contact force */
    if(actor->transform.pos.y <= (height + 0.1f)){
      into_surface = vec3_dot(net_force, normal);
      if(into_surface < 0){
        normal_mag = -into_surface;  
        vec3 normal_force = vec3_scale(normal, normal_mag);
      
        net_force = vec3_add(net_force, normal_force);
      }
    }

    /* friction */
    float friction_mag = $friction_coeff * normal_mag;
    vec3 tangent_force =
      vec3_sub(gravity_force, vec3_scale(normal, vec3_dot(gravity_force, normal)));
    
    float tangent_len = vec3_length(tangent_force);
    if(tangent_len <= friction_mag){
        net_force = vec3_sub(net_force, tangent_force);
    }else{
        vec3 friction = vec3_scale(vec3_normalize(tangent_force), -friction_mag);
        net_force = vec3_add(net_force, friction);
    }    

    float traction = $friction_coeff * normal_mag;// * normal.y * 10;

    force_len = vec3_length(actor->physics_actor.applied_force);
    if(force_len > traction){
        actor->physics_actor.applied_force =
          vec3_scale(
            actor->physics_actor.applied_force,
            traction / force_len
          );
    }
  
    net_force = vec3_add(net_force, actor->physics_actor.applied_force);

    /* integrations */
    actor->physics_actor.acc =
      vec3_scale(net_force, 1/actor->physics_actor.mass);
    actor->physics_actor.vel =
      vec3_add(actor->physics_actor.vel, vec3_scale(actor->physics_actor.acc, dt));
    actor->transform.pos =
      vec3_add(actor->transform.pos, vec3_scale(actor->physics_actor.vel, dt));

    float penetration = (height - actor->transform.pos.y) / normal.y;
      if(penetration > 0){
        actor->transform.pos = vec3_add(
            actor->transform.pos,
            vec3_scale(normal, penetration)
        );
      }

    actor->physics_actor.net_force = net_force;
}






void step_physics(
  controller_state_t  *controller,
  simulation_t     *simulation
  ) {
  if(simulation->player_table[0].flying){
    flying_movement_physics(
      &simulation->player_table[0].entity,
      controller->dt
    );
  }else{
    movement_physics(
      &simulation->player_table[0].entity,
      &simulation->terrain_table[0],
      controller->dt
    );
  }
  movement_physics(
    &simulation->entity_table[0],
    &simulation->terrain_table[0],
    controller->dt
  );
  simulation->entity_table[0].render_data.remodel = true;



  simulation->player_table[0].camera.pos =
    vec3_add(
      simulation->player_table[0].entity.transform.pos,
      simulation->player_table[0].camera.offset
    );
  simulation->player_table[0].entity.transform.rot = 
  simulation->player_table[0].camera.rot;
}



