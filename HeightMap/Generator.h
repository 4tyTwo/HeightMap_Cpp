#pragma once
#include "HeightMap.h"
#include <iostream>
#include <random>
class Generator
{
public:
  Generator();
  void Generate(int x_size, int y_size, double roughness);
  HeightMap hmap;
  ~Generator();
private:
 double roughness_;
 int step_size;
 void square();
 void diamond();
 double square_displace(int i,int j);
 double diamond_displace(int i, int j);
};

