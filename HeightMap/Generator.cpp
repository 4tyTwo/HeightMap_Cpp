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

void Generator::River_generation(int base_x,int base_y,int x, int y,int iteration,double roughness) {
  //int length = (abs(x - base_x)>=abs(y - base_y)) ? x-base_x:y-base_y;
  int length = y - base_y;
  if (length<=32){
    if (base_x == x){
      for(int i = base_y;i<y;i++)
        river[x][i]=1.0;
    }
    else{
      int mx = base_x + ((x - base_x) / 2), my = base_y + ((y - base_y) / 2);
      int coeff = length/2;
      int newx = mx + rand()%coeff - coeff/2;
      if (newx >= river.x_size())
          newx = river.x_size() - newx % river.x_size() - 1;
      if (newx < 0)
         newx = abs(newx);
      int newy = my;
      curve(base_x, base_y, newx, my, x, y);
      return;
    }
}
 // int coeff = length/6;
 // coeff = (coeff == 0) ? 1 : coeff;
 // double rnd = rand() % coeff, rnd2 = (rand() % coeff - coeff/2)/2;
  //int mx = base_x + ((x-base_x)/2) + rnd - coeff/2, my = base_y + ((y-base_y)/2) + rnd2;
  int mx = base_x + ((x - base_x) / 2), my = base_y + ((y - base_y) / 2);
  int coeff = length / 2.5;
  int newx = mx + rand() % coeff - coeff / 2;
  if (newx < 0)
    newx = abs(newx);
  else
    if (newx >= x)
      newx = x-1;
  if (newx < 0)
    newx = abs(newx);
  River_generation(newx,my,x,y,iteration+1,roughness);//верхне-права€ половина
  River_generation(base_x,base_y, newx, my, iteration + 1, roughness);
}

void Generator::line(int x0,int y0,int x1,int y1) {
  //–исует на карте пр€мую линию, соедин€ющую точки (x0,y0) и (x1,y1). √раничные точки €вл€ютс€ частью пр€мой.
  int deltax = abs(x1 - x0), deltay = abs(y1 - y0);
  double error = 0;
  if (deltay <= deltax) {
    double deltaerr = (double)deltay / deltax;
    int y = y0, x = x0;
    int diry = y1 - y0;
    if (diry > 0)
      diry = 1;
    else
      diry = -1;
    if (x1 - x > 0)
      for (x = x0; x < x1; x++) {
        river[x][y] = 1.0;
        error += deltaerr;
        if (error > 0.5) {
          y = y + diry;
          error -= 1.0;
        }
      }
    else
      for (x = x0; x >= x1; x--) {
        river[x][y] = 1.0;
        error += deltaerr;
        if (error > 0.5) {
          y = y + diry;
          error -= 1.0;
        }
      }
  }
  else {
    double deltaerr = (double)deltax / deltay;
    int y = y0, x = x0;
    int dirx = x1 - x0;
    if (dirx > 0)
      dirx = 1;
    else
      dirx = -1;
    if (y0 < y1)
      for (y = y0; y < y1; y++) {
        river[x][y] = 1.0;
        error += deltaerr;
        if (error > 0.5) {
          x = x + dirx;
          error -= 1.0;
        }
      }
    else
      for (y = y0; y >= y1; y--) {
        river[x][y] = 1.0;
        error += deltaerr;
        if (error > 0.5) {
          x = x + dirx;
          error -= 1.0;
        }
      }
  }
}

void Generator::curve(int x0, int y0, int xm, int ym, int x1, int y1) {
  int x = x0 - xm, y = y0 - ym;
  double t = x0 - 2 * xm + x1, r;
  if ((long)x*(x1 - xm) > 0) { /* horizontal cut at P4? */
    if ((long)y*(y1 - ym) > 0) /* vertical cut at P6 too? */
      if (fabs((y0 - 2 * ym + y1) / t * x) > abs(y)) { /* which first? */
        x0 = x1; x1 = x + xm; y0 = y1; y1 = y + ym; /* swap points */
      } /* now horizontal cut at P4 comes first */
    t = (x0 - xm) / t;
    r = (1 - t)*((1 - t)*y0 + 2.0*t*ym) + t * t*y1; /* By(t=P4) */
    t = (x0*x1 - xm * xm)*t / (x0 - xm); /* gradient dP4/dx=0 */
    x = floor(t + 0.5); y = floor(r + 0.5);
    r = (ym - y0)*(t - x0) / (xm - x0) + y0; /* intersect P3 | P0 P1 */
    curveSeg(x0, y0, x, floor(r + 0.5), x, y);
    r = (ym - y1)*(t - x1) / (xm - x1) + y1; /* intersect P4 | P1 P2 */
    x0 = xm = x; y0 = y; ym = floor(r + 0.5); /* P0 = P4, P1 = P8 */
  }
  if ((long)(y0 - ym)*(y1 - ym) > 0) { /* vertical cut at P6? */
    t = y0 - 2 * ym + y1; t = (y0 - ym) / t;
    r = (1 - t)*((1 - t)*x0 + 2.0*t*xm) + t * t*x1; /* Bx(t=P6) */
    t = (y0*y1 - ym * ym)*t / (y0 - ym); /* gradient dP6/dy=0 */
    x = floor(r + 0.5); y = floor(t + 0.5);
    r = (xm - x0)*(t - y0) / (ym - y0) + x0; /* intersect P6 | P0 P1 */
    curveSeg(x0, y0, floor(r + 0.5), y, x, y);
    r = (xm - x1)*(t - y1) / (ym - y1) + x1; /* intersect P7 | P1 P2 */
    x0 = x; xm = floor(r + 0.5); y0 = ym = y; /* P0 = P6, P1 = P7 */
  }
  curveSeg(x0, y0, xm, ym, x1, y1); /* remaining part */
}

void Generator::curveSeg(int x0, int y0, int xm, int ym, int x1, int y1) {
  /* plot a limited quadratic Bezier segment */
  int sx = x1 - xm, sy = y1 - ym;
  long xx = x0 - xm, yy = y0 - ym, xy; /* relative values for checks */
  double dx, dy, err, cur = xx * sy - yy * sx; /* curvature */
  assert(xx*sx <= 0 && yy*sy <= 0); /* sign of gradient must not change */
  if (sx*(long)sx + sy * (long)sy > xx*xx + yy * yy) { /* begin with longer part */
    x1 = x0; x0 = sx + xm; y1 = y0; y0 = sy + ym; cur = -cur; /* swap P0 P2 */
  }
  if (cur != 0) { /* no straight line */
    xx += sx; xx *= sx = x0 < x1 ? 1 : -1; /* x step direction */
    yy += sy; yy *= sy = y0 < y1 ? 1 : -1; /* y step direction */
    xy = 2 * xx*yy; xx *= xx; yy *= yy; /* differences 2nd degree */
    if (cur*sx*sy < 0) { /* negated curvature? */
      xx = -xx; yy = -yy; xy = -xy; cur = -cur;
    }
    dx = 4.0*sy*cur*(xm - x0) + xx - xy; /* differences 1st degree */
    dy = 4.0*sx*cur*(y0 - ym) + yy - xy;
    xx += xx; yy += yy; err = dx + dy + xy; /* error 1st step */
    do {
      river[x0][y0] = 1.0; /* plot curve */
      if (x0 == x1 && y0 == y1) return; /* last pixel -> curve finished */
      ym = 2 * err < dx; /* save value for test of y step */
      if (2 * err > dy) { x0 += sx; dx -= xy; err += dy += yy; } /* x step */
      if (ym) { y0 += sy; dy -= xy; err += dx += xx; } /* y step */
    } while (dy < 0 && dx > 0); /* gradient negates -> algorithm fails */
  }
  line(x0, y0, x1, y1); /* plot remaining part to end */
}

Generator::~Generator()
{
}
