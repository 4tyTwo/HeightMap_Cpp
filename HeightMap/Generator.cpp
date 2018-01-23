#include "Generator.h"

Generator::Generator(){
}

double Generator::square_displace(int i, int j) {
  double total = 0.0, avg;
  int zeros=0;
  int quater_step = step_size/4;
  if (i - quater_step >= 0)
    total += hmap[i- quater_step][j];
  else
    total += -1;
  if (i + quater_step < hmap.x_size())
    total += hmap[i+ quater_step][j];
  else
    total += -1;
  if (j - quater_step >= 0)
    total += hmap[i][j- quater_step];
  else
    total += -1;
  if (j + quater_step < hmap.y_size())
    total += hmap[i][j+ quater_step];
  else
    total+=-1;
  if (total < 0)
    total = 0;
  avg = total/4.0;
  return roughness_*(double)(rand()) / RAND_MAX  + (1-roughness_)*avg;
}

double Generator::diamond_displace(int i, int j) {
  double avg,ul,ur,ll,lr;
  int quater_step = step_size/4;
  ul = hmap[i - quater_step][j - quater_step];
  ur = hmap[i - quater_step][j + quater_step];
  ll = hmap[i + quater_step][ j - quater_step];
  lr = hmap[i + quater_step][ j + quater_step];
  avg = (ul + ur + ll + lr) / 4.0;
  return roughness_ * (double)(rand()) / RAND_MAX + (1 - roughness_) * avg;
}

void Generator::diamond(){
  int half_step = step_size/2, quater_step = step_size/4;
  for (int i=quater_step;i<hmap.x_size();i+=half_step)
    for (int j=quater_step;j<hmap.y_size()-1;j+=half_step)
      hmap[i][j] = diamond_displace(i,j);
}

void Generator::square() {
  int half_step = step_size/2, quater_step = step_size/4;
  for (int i = 0; i < hmap.x_size(); i+=half_step)
    for (int j = quater_step; j < hmap.y_size(); j+=half_step)
      hmap[i][j] = square_displace(i,j);
  for (int i = quater_step;i<hmap.x_size();i+=half_step)
    for (int j = 0; j < hmap.y_size(); j+=half_step)
      hmap[i][j] = square_displace(i, j);
}

void Generator::Generate(int x_size_, int y_size_, double roughness) {
  hmap = HeightMap(x_size_,y_size_);
  int iterations;
  roughness_ = roughness;
  hmap.zeros();
  int x_size = hmap.x_size(), y_size = hmap.y_size();
  hmap[0][0] = (double)(rand()) / RAND_MAX;
  hmap[0][y_size-1] = (double)(rand()) / RAND_MAX;
  hmap[x_size-1][0] = (double)(rand()) / RAND_MAX;
  hmap[x_size-1][hmap.y_size()-1] = (double)(rand()) / RAND_MAX;
  hmap[0][(y_size-1) / 2] = (double)(rand()) / RAND_MAX;
  hmap[x_size-1][(y_size-1)/2] = (double)(rand()) / RAND_MAX;
  for (int i = 0; i < 15; i++) 
    if (x_size == pow(2,i) + 1) {
      iterations = i;
      break;
  }
  for (int i = 0; i < iterations; i++) {
    roughness_ = pow(roughness,i);
    step_size = ((y_size-1)/pow(2,i));
    if (i == 1) {
      hmap[0][0] = 0;
      hmap[0][y_size - 1] = 0;
      hmap[x_size - 1][0] = 0;
      hmap[x_size - 1][hmap.y_size() - 1] = 0;
      hmap[0][(y_size - 1) / 2] = 0;
      hmap[x_size - 1][(y_size - 1) / 2] = 0;
    }
    diamond();
    square();
  }
}

Generator::~Generator()
{
}
