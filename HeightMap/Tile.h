#pragma once
#include <string>
#include <vector>
#include <algorithm>

static const std::vector<std::string> type_vect{"water","mountain","sand","none"};//Еще биомы
class Tile
{
public:
  //Конструкторы

  Tile();

  //Set методы

  void setHeight(double height);
  void setTemp(int temp);
  void setHumid(float humid);
  void setType(std::string type);

  //Get методы

  inline double Height() { return height_; }
  inline int Temperature() {return temperature_; }
  inline float Humidity() { return humidity_; } 
  inline std::string Type() { return type_; }

  //Деструктор по умолчанию

  ~Tile();
  //inline std::string Type() { return type_; }

  enum class special { water, mountain, sand, none }; //Биомы тоже наверное пойдут сюда

private:
  std::string type_;//Тип
  double height_;//Альтитуда от 0 до 1.
  int temperature_;//Температура в градусах цельсия, ограничена только снизу по очевидным причинам
  float humidity_;//Влажность. Выражается в нормальизованном виде от 0 до 1, где 0 - полная сухость.
};

