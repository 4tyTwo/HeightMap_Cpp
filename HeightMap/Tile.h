#pragma once
enum special {water,mountain,none};
class Tile
{
public:
  Tile();
  ~Tile();
private:
special type;//������� enum
double height_;//���������
int temperature_;//�����������
double humidity_;//���������

};

