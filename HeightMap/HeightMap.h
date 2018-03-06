#pragma once
class HeightMap
{
public:
  HeightMap();
  HeightMap(int x_size,int y_size);
  inline int x_size() { return x_size_; }
  inline int y_size() { return y_size_; }
  double get(int x,int y);
  double average();
  double* operator[](int x);
  void zeros();
  ~HeightMap();
private:
  double** data;
  int x_size_,y_size_;
};