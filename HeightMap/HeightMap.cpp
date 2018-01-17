#include "HeightMap.h"

HeightMap::HeightMap(){
 data = nullptr;
}
HeightMap::HeightMap(int x_size, int y_size) {
  x_size_ = x_size;
  y_size_ = y_size;
  data = new double*[x_size_];
  for (int i=0;i<x_size_;++i)
    data[i] = new double[y_size_];
}

void HeightMap::zeros() {
  for (int i = 0; i < x_size_; i++)
    for (int j = 0; j<y_size_;j++)
      data[i][j]=0.0;
}

double* HeightMap::operator[](int x) {
  return data[x];
}

double HeightMap::get(int x, int y) {
  return data[x][y];
}

double HeightMap::average() {
  double summ=0.0;
  for(int i=0;i<x_size_;i++)
    for(int j=0;j<y_size_;j++)
      summ+=data[i][j];
return summ/(x_size_*y_size_);
}

HeightMap::~HeightMap(){
}
