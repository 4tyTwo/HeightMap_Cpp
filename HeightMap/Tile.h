#pragma once
enum special {water,mountain,none};
class Tile
{
public:
  Tile();
  ~Tile();
private:
special type;//Сделать enum
double height_;//Альтитуда
int temperature_;//Температура
double humidity_;//Влажность

};

