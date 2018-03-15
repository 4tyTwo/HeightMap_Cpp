#include "Tile.h"

Tile::Tile(){ // онструктор по-умолчанию, особого смысла не несет
  temperature_ =-273;
  humidity_ = 0.0;
  height_ = 0.0;
  type_ = "none";
}

void Tile::setHeight(double height) {
  if (height < 0.0)
    height = 0.0;
  else
    if (height > 1.0)
      height = 1.0;
  height_=height;
}

void Tile::setHumid(float humid) {
  if (humid < 0.0)
    humid = 0.0;
  else
    if (humid > 1.0)
      humid = 1.0;
  humidity_ = humid;
}

void Tile::setTemp(int temp) {
  temperature_ = (temp<-273) ? -273 : temp;
}

void Tile::setType(std::string type) {
  if (std::find(type_vect.begin(), type_vect.end(), type) != type_vect.end())
    type_ = type;
}

Tile::~Tile()
{
}
