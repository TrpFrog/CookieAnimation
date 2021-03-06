#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "img.h"
#include "object.h"
#include "layer.h"
#include "drawtool.h"
#include "fonts.h"

void draw_background(struct color layer[HEIGHT][WIDTH]){
    clear_layer(layer);
    struct color base_color = {40,107,142,1.0};
    struct color base_color2 = {34,93,128,1.0};
    img_fillrect(base_color,100, 150, 200, 300,layer);
    for (double i = 0; i < 20; i++){
        if((int)i%2 == 0){
            img_fillrect(base_color2,i*11+1, 150, 10, 300,layer);
        }
    }
    struct color black = {0,0,0,0.5};
    double max_alpha = black.a;
    for(int y=0; y<HEIGHT; y++){
        black.a = ((120-y > 0 ? 120-y : y-120)/300.0*max_alpha);
        for(int x=0; x<WIDTH; x++){
            layer[y][x] = mix_color(black,layer[y][x]);
        }
    }
}

void draw_cookieband(void){
    struct color c = {0,0,0,0.5};
    for(int i=230; i<=270; i++){
        for(int j=0; j<WIDTH; j++){
            put_pixel(c,j,i);
        }
    }
}

void bake_cookie(struct color layer[HEIGHT][WIDTH]){
    clear_layer(layer);
    int center_x = 100;
    int center_y = 140;
    struct color shadow = {0x60, 0x38, 0x23, 1.0};
    struct color light = {0xad, 0x8b, 0x60, 0.0};
    struct color c;
    //img_fillcircle(shadow,center_x,center_y,60,layer);

    for(int r = 60; r > 0; r--){
        light.a = sqrt(3600-r*r)/60.0;
        c = mix_color(light,shadow);
        c.a = 1.0;
        img_fillcircle(c,center_x,center_y,r,layer);
    }
    int x[][14] = {{80, 95, 105,102,90, 85, 75, 75, 75, 75, 75, 75, 75, 75},
                   {86, 94, 102,105,97, 87, 83, 82, 82, 82, 82, 82, 82, 82},
                   {135,140,142,138,133,124,123,125,125,125,125,125,125,125},
                   {137,140,139,140,138,133,124,121,123,127,132,132,132,132},
                   {94, 99, 95, 91, 88, 84, 79, 76, 73, 71, 74, 79, 81, 91},
                   {108,117,121,119,114,111,108,103,102,105,105,105,105,105},
                   {79, 85, 77, 73, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76}
                 };
    int y[][14] = {{160,165,150,145,135,135,145,145,145,145,145,145,145,145},
                   {186,184,181,175,172,175,181,184,184,184,184,184,184,184},
                   {164,160,147,146,148,145,156,158,158,158,158,158,158,158},
                   {133,131,127,121,122,119,118,119,122,126,127,127,127,127},
                   {117,113,105,102,105,105,103,103,105,108,112,112,114,117},
                   {117,112,103,100,98, 99, 98, 101,110,116,116,116,116,116},
                   {127,122,115,120,126,126,126,126,126,126,126,126,126,126}
                 };
    int dots[] = {7,8,8,11,14,10,5};
    struct color chocochip[HEIGHT][WIDTH];
    struct color temp[HEIGHT][WIDTH];
    struct color choco_color = {0x5c,0x34,0x21,1.0};

    for(int i=0; i<7; i++){
        clear_layer(chocochip);
        fill_polygon(x[i],y[i],dots[i],choco_color,chocochip);
        merge_layer(chocochip);
        unite_layer(layer,chocochip);
    }
}

void bake_background_cookie(struct color layer[HEIGHT][WIDTH], int t){
    clear_layer(layer);
    int x0, y0;
    struct color light = {0xad, 0x8b, 0x60, 1.0};
    struct color choco_color = {0x5c, 0x34, 0x21, 1.0};
    for (int i = 0; i < 20;i++){
        x0 = (20 * i)%200;
        y0 = 300 - (7 * t - (int)(100 * sin(i * 1.6))) % 250;
        img_fillcircle(light, x0, y0, 8, layer);
        img_fillcircle(choco_color, x0 - 1, y0 + 2, 3, layer);
        img_fillcircle(choco_color, x0 + 4 , y0 + 2, 1, layer);
        img_fillcircle(choco_color, x0 + 1 , y0 - 4, 1, layer);
        img_fillrect(choco_color, x0 - 1 , y0 + 5, 3, 1, layer);
        img_fillrect(choco_color, x0 - 2 , y0 - 4, 3, 1, layer);
        img_fillrect(choco_color, x0 + 4 , y0 - 2, 3, 1, layer);
    }
}

void pour_milk(int t,int h){
    struct color milk_color = {255,254,246,0.8};
    for(int x=0; x<WIDTH; x++){
        int endpoint = (int)(7*sin(0.1*t+0.02*x)+50+h);
        for(int y = 0; y<=endpoint; y++){
            int diff = 10;
            struct color c = {(milk_color.r-diff)+(diff/endpoint*y),
                              (milk_color.g-diff)+(diff/endpoint*y),
                              (milk_color.b-diff)+(diff/endpoint*y),
                              (milk_color.a-0.3)+(0.3/endpoint*y)};
            put_pixel(c,x,y);
        }
    }
}

int rest(int i,int divisor){
    while(i<0){
        i += divisor;
    }
    return i%divisor;
}

double get_degree(double x, double y){
    double d = atan2(y,x)*180/3.14159265359;
    return d >= 0 ? d : d+360;
}

void draw_shine(int time, int wings){
    int center_x = 100;
    int center_y = 140;
    struct color shine_color = {200,200,200,0};
    double X, Y, r;
    double gradation_r = 120.0;

    for(int y=0; y<HEIGHT; y++){
        for(int x=0; x<WIDTH; x++) {

            X = x-center_x; Y = y-center_y;
            r = sqrt(pow(X,2)+pow(Y,2));
            double degree = get_degree(X,Y);

            for(int i=0; i<wings; i++){
                double d1 = rest(-0.3*time+360/wings*i,360);
                double d2 = rest(-0.3*time+360/(2*wings)+360/wings*i,360);
                if((d1<=degree && degree<=d2 && d1<=d2) || ((d1<=degree || degree<=d2) && d1>d2)){
                    shine_color.a = 1 - r/gradation_r;
                    put_pixel(shine_color,x,y);
                }
            }

        }
    }
}


void draw_glow_circle(){
    struct color layer[HEIGHT][WIDTH];
    clear_layer(layer);
    struct color light = {0xff,0xff,0xff,0.0};
    clear_layer(layer);
    for(int r = 120; r > 0; r--){
        light.a = 0.015;
        img_fillcircle(light,100,140,r,layer);
    }
    merge_layer(layer);
}

struct color cursor[HEIGHT][WIDTH];
struct color rotated_cursor[HEIGHT][WIDTH];
void draw_cursor(int x0, int y0, struct color layer[HEIGHT][WIDTH]){
    struct color white = {255, 255, 255, 1.0};
    struct color black = {0, 0, 0, 1.0};
    int xb[] = {4, 4, 4, 3, 3, 3, 2, 2, 1, 1, 1, 0, 0, -1, -1, -1, -1, -1, -1, -2, -2, -3, -3, -3, -3, -3, -3, -3, -3, -4, -4, -5, -5};
    int yb[] = {1, 0, -1, 2, -2, -3, 3, -4, 3, 2, -4, 4, -4, 6, 5, 4, 3, 2, -4, 7, -4, 6, 5, 4, 3, 2, 1, -2, -3, 2, -1, 1, 0};
    int xw[] = {3, 3, 3, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -3, -3, -4, -4};
    int yw[] = {1, 0, -1, 2, 1, 0, -1, -2, -3, 1, 0, -1, -2, -3, 3, 2, 1, 0, -1, -2, -3, 1, 0, -1, -2, -3, 6, 5, 4, 3, 2, 1, 0, -1, -2, -3, 0, -1, 1, 0};
    for (int i = 0; i < 33; i++){
        layer[y0 + yb[i]][x0 + xb[i]] = black;
    }
    for (int j = 0; j < 40; j++){
        layer[y0 + yw[j]][x0 + xw[j]] = white;
    }
    copy_layer(cursor,layer);
    clear_layer(layer);
    for(int i=0; i<4; i++){
        rotate_layer(layer,90,100,140);
        unite_layer(layer,cursor);
    }
    copy_layer(cursor,layer);
    for(int i=1; i<=10; i++){
        copy_layer(rotated_cursor,cursor);
        rotate_layer(rotated_cursor,10*i,100,140);
        unite_layer(layer,rotated_cursor);
    }
}

void draw_pop_up_icon(struct color layer[HEIGHT][WIDTH], int x0, int y0){
    struct color brown = {0xcb, 0x72, 0x48, 1.0};
    struct color green = {0x98, 0xae, 0x86, 1.0};
    struct color yellow = {0xf5, 0xe7, 0xb4, 1.0};
    struct color blue = {0x1e, 0x38, 0x42, 1.0};
    struct color white = {0xfb, 0xf8, 0xf4, 1.0};
    int xbl[] = {5, 5, 5, 5, 5, 5, 5, 5, 4, 4, 3, 3, 3, 2, -1, -2, -2, -2, -3, -4, -4, -5, -5, -5, -5, -5, -5, -5};
    int ybl[] = {2, 1, 0, -1, -2, -3, -4, -5, 3, -6, 6, 5, 4, 4, 4, 6, 5, 4, 3, 2, -6, 1, 0, -1, -2, -3, -4, -5};
    int xg[] = {4, 4, 4, 3, 3, 3, 2, 2, 2, 1, 1, 1, 0, 0, 0, 0, -1, -1, -1, -1, -2, -2, -2, -3, -3, -3, -3, -4, -4, -4, -4, -4, -4, -4};
    int yg[] = {2, 1, -5, 3, 2, -6, 6, 5, -6, 4, 3, -6, 6, 5, 4, -6, 6, 5, 3, -6, 3, 2, -6, 2, 1, -5, -6, 1, 0, -1, -2, -3, -4, -5};
    int xy[] = {4, 4, 4, 4, 4, 3, 2, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -2, -2, -2, -2, -2, -2, -2, -3, -3, -3, -3, -3};
    int yy[] = {0, -1, -2, -3, -4, -5, -5, 2, 1, 0, -1, -2, -3, -4, -5, 3, 2, 1, 0, -1, -2, -3, -4, -5, 2, 1, 0, -1, -2, -3, -4, -5, 1, 0, -1, -2, -3, -4, -5, 0, -1, -2, -3, -4};
    int xbr[] = {2, 1, 0, -1};
    int ybr[] = {7, 7, 7, 7};
    int xw[] = {3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1};
int yw[] = {1, 0, -1, -2, -3, -4, 3, 2, 1, 0, -1, -2, -3, -4, 6, 5};
for (int i = 0; i < 28; i++){
    put_pixel(blue, x0 + xbl[i], y0 + ybl[i]);
    }
    for (int j = 0; j < 34;j++){
        //put_pixel(green, x0 + xg[j], y0 + yg[j]);
        layer[y0 + yg[j]][x0 + xg[j]] = green;    
        }
    for (int k = 0; k < 44;k++){
        //put_pixel(yellow, x0 + xy[k], y0 + yy[k]);
        layer[y0 + yy[k]][x0 + xy[k]] = yellow;
    }
    for (int l = 0; l < 4;l++){
        //put_pixel(brown, x0 + xbr[l], y0 + ybr[l]);
        layer[y0 + ybr[l]][x0 + xbr[l]] = brown;
    }
    for (int m = 0; m < 16;m++){
        //put_pixel(white, x0 + xw[m], y0 + yw[m]);
        layer[y0 + yw[m]][x0 + xw[m]] = white;
    }
}

void draw_pop_up(struct color layer[HEIGHT][WIDTH],int t){
    clear_layer(layer);
    if(t<0) return;
    struct color black = {22,22,22,1.0};
    struct color gold = {209, 174, 21, 1.0};
    t = t<33 ? t : 33;
    img_fillrect(gold, 100, -16 + t, 120, 31, layer);
    img_fillrect(black, 100, -16 + t-1, 117, 28, layer);
    draw_pop_up_icon(layer, 50, -16 + t);
    print_achievement(layer,t);
    print_cookiedunker(layer,t);
}