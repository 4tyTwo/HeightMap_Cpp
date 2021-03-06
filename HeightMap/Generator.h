#pragma once
#include "HeightMap.h"
#include <assert.h>
#include <iostream>
#include <random>
class Generator
{
public:
  Generator();
  void Generate(int x_size, int y_size, double roughness);
  HeightMap hmap;
  HeightMap river;
  ~Generator();
  void River_generation(int base_x, int base_y,int x,int y,int& sign);
  void line(int x0, int y0, int x1, int y1);
  void curve(int x0,int y0,int xm,int ym,int x1,int y1);
  void curveSeg(int x0, int y0, int xm, int ym, int x1, int y1);
private:
 double roughness_;
 int step_size;
 void square();
 void diamond();
 double square_displace(int i,int j);
 double diamond_displace(int i, int j);
};

