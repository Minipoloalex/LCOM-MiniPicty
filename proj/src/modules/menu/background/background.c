#include "background.h"

#define STAR_COLOR 0xFFFFFF 
#define SUN_COLOR 0xFFFF00
#define SUN_RADIUS 60
#define TERRAIN_COLOR 0xCD853F
#define TERRAIN_HEIGHT 300
#define NUMBER_STARTS 100
#define BLACK_SKY_COLOR 0x242424
/**
 * @brief 
 * 
 */
void (draw_sun)(Background* background_scene, uint32_t hour);
/**
 * @brief 
 * 
 */
void (draw_stars)(Background* background_scene);
/**
 * @brief 
 * 
 */
void (draw_sky)(Background* background_scene, uint32_t hour);
/**
 * @brief 
 * 
 */
void (draw_terrain)(Background* background_scene);
/**
 * @brief 
 * 
 */
int (calculate_sun_height)(int hour);
/**
 * @brief 
 * 
 */
uint32_t (calculate_sky_color)(int hour);

Background * (create_background)(){
  Background *background = (uint8_t *) malloc(get_vram_size() * sizeof(uint8_t));
  if(background == NULL) return NULL;
  return background;
}

void (destroy_background)(Background *background){
  free(background);
}

void (draw_timelapse)(Background* scene, int hour){
  draw_sky(scene, hour);
  draw_sun(scene, hour);
  draw_terrain(scene);
}

void (draw_sun)(Background* scene, uint32_t hour){
  if(hour > 19 || hour < 6) return;
  int hour_space = (get_h_res() - SUN_RADIUS * 2) / 13;
  hour -= 6;
  int x = (get_h_res() / 2) - 6*hour_space + (hour)*hour_space;
  int y = calculate_sun_height(hour);
  if(vg_draw_circle_to_buffer(scene, x - (SUN_RADIUS/2), get_v_res() - TERRAIN_HEIGHT - y, SUN_RADIUS, SUN_COLOR)){
    printf("vg_draw_circle inside %s\n", __func__);
    return;
  }
}

void (draw_stars)(Background* scene){
  srand(time(0));
  int x = 0, y = 0;
  for(int i = 0; i < NUMBER_STARTS; i++){
    x = rand() % get_h_res();
    y = rand() % get_v_res();
    if(vg_draw_circle_to_buffer(scene, x, y, 2, STAR_COLOR)){
      printf("vg_draw_pixel inside %s\n", __func__);
      return;
    }
  }
}

void (draw_sky)(Background* scene, uint32_t hour){
  uint32_t color = calculate_sky_color(hour);
  if(vg_draw_rectangle_to_buffer(scene, 0, 0, get_h_res(), get_v_res() - TERRAIN_HEIGHT, color)){
    printf("vg_draw_rectangle inside %s\n", __func__);
    return;
  }

  if(hour > 19){
    draw_stars(scene);
  }
}

void (draw_terrain)(Background* scene){
  if(vg_draw_rectangle_to_buffer(scene, 0, get_v_res() - TERRAIN_HEIGHT, get_h_res(), get_v_res(), TERRAIN_COLOR)){
    printf("vg_draw_rectangle inside %s\n", __func__);
    return;
  }
}

int (calculate_sun_height)(int hour){
  int x = hour - 6;
  int a = 6;
  int b = 20;

  return (int) pow(b,2) * sqrt(1 - (pow(x, 2) / pow(a, 2)));
}

uint32_t (calculate_sky_color)(int hour) {
  if(hour >= 19 || hour <= 6) return BLACK_SKY_COLOR;

  if(hour > 12){
    hour = 12 - (hour - 12);
  }

  uint32_t red = 0x00;
  uint32_t green = (hour * 255) / 12;
  uint32_t blue = (hour * 255) / 12;

  return (red << 16) | (green << 8) | blue;
}
