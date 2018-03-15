#pragma once
#include <string>
#include <vector>
#include <algorithm>

static const std::vector<std::string> type_vect{"water","mountain","sand","none"};//��� �����
class Tile
{
public:
  //������������

  Tile();

  //Set ������

  void setHeight(double height);
  void setTemp(int temp);
  void setHumid(float humid);
  void setType(std::string type);

  //Get ������

  inline double Height() { return height_; }
  inline int Temperature() {return temperature_; }
  inline float Humidity() { return humidity_; } 
  inline std::string Type() { return type_; }

  //���������� �� ���������

  ~Tile();
  //inline std::string Type() { return type_; }

  enum class special { water, mountain, sand, none }; //����� ���� �������� ������ ����

private:
  std::string type_;//���
  double height_;//��������� �� 0 �� 1.
  int temperature_;//����������� � �������� �������, ���������� ������ ����� �� ��������� ��������
  float humidity_;//���������. ���������� � ���������������� ���� �� 0 �� 1, ��� 0 - ������ �������.
};

