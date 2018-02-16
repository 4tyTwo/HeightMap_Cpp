#include "HeightMap.h"
#include "Generator.h"
#include <Windows.h>
#include <iostream>
#include <algorithm>
#include <ctime>

void CreateBmp24(char *fname, HeightMap& hmap,double sea_level)
{
  HANDLE hFile;
  DWORD RW;
  int i, j;
  RGBTRIPLE color;
  color.rgbtBlue = 255;
  color.rgbtGreen = 0;
  color.rgbtRed = 0;
  // Объявим нужные структуры
  BITMAPFILEHEADER bfh;
  BITMAPINFOHEADER bih;
  BYTE Palette[1024];									// Палитра

                                      // Пусть у нас будет картинка размером 35 x 50 пикселей
  int Width = hmap.y_size();
  int Height = hmap.x_size();
  memset(Palette, 0, 1024);								// В палитре у нас нули
  
                                          // Заполним их
  memset(&bfh, 0, sizeof(bfh));
  bfh.bfType = 0x4D42;									// Обозначим, что это bmp 'BM'
  bfh.bfOffBits = sizeof(bfh) + sizeof(bih) + 1024;		// Палитра занимает 1Kb, но мы его испоьзовать не будем
  bfh.bfSize = bfh.bfOffBits +
    sizeof(color) * Width * Height +
    Height * (Width % 4);						// Посчитаем размер конечного файла

  memset(&bih, 0, sizeof(bih));
  bih.biSize = sizeof(bih);								// Так положено
  bih.biBitCount = 24;									// 16 бит на пиксель
  bih.biCompression = BI_RGB;								// Без сжатия
  bih.biHeight = Height;
  bih.biWidth = Width;
  bih.biPlanes = 1;										// Должно быть 1
                                      // А остальные поля остаются 0
  hFile = CreateFile(fname, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
  if (hFile == INVALID_HANDLE_VALUE)
    return;

  // Запишем заголовки
  WriteFile(hFile, &bfh, sizeof(bfh), &RW, NULL);
  WriteFile(hFile, &bih, sizeof(bih), &RW, NULL);
 // double avg  = hmap.average();
  // Запишем палитру
  WriteFile(hFile, Palette, 1024, &RW, NULL);
  for (i = 0; i < hmap.x_size(); i++){
    for (j = 0; j < hmap.y_size(); j++){
      if (hmap[i][j] > sea_level){
        color.rgbtGreen = 255 - (int)(hmap[i][j] * 255);
        color.rgbtBlue = 0;
        color.rgbtRed = 0;
      }
      else {
        color.rgbtGreen = 0;
        color.rgbtBlue = 255- (int)(hmap[i][j] * 255);;
        color.rgbtRed = 0;
      }
      WriteFile(hFile, &color, sizeof(color), &RW, NULL);
    }
    // Выровняем по границе
    WriteFile(hFile, Palette, Width % 4, &RW, NULL);
  }

  CloseHandle(hFile);
}



int main() {
  srand(time(NULL));
  //Generator gen;
  int h,w;
  double sea_level;
  std::cout.precision(7);
  clock_t start,end;
  double r,avg=0.0;
  std::cout<<"Input height: ";
  std::cin>>h;
  std::cout << "Input width: ";
  std::cin >> w;
  //std::cout << "Input sea percentage: ";
  //std::cin >> sea_level;
  start =clock();
  double newavg,summ,curr;
  FILE *f,*fr;
  errno_t err;
  for (int y=95;y<101;y++){
    sea_level = (double)y/100;
    char filename1[80] = "C:\\Output\\values",filename2[80]="C:\\Output\\roughnesses",buff[15];
    _itoa_s((int)(sea_level * 100),buff,10);
    strcat_s(filename1,80,buff);
    strcat_s(filename1,80,".txt");
    strcat_s(filename2,80, buff);
    strcat_s(filename2,80,".txt");
    err = fopen_s(&f, filename1, "w");
   // err = fopen_s(&fr,filename2, "w");
    for (int u = 0;u<101;u++){
    Generator gen;
    curr = (double)u/100;
    summ = 0.0;
      for (int k=0; k<50;k++){
      gen.Generate(h,w,curr);
      avg = gen.hmap.average();
      for (int i=0;i<gen.hmap.x_size();i++)
        for (int j=0;j<gen.hmap.y_size();j++)
          gen.hmap[i][j]= pow(gen.hmap[i][j],2);
      std::vector<double> vect(w*h);
      int counter = 0;
      for (int i = 0; i<gen.hmap.x_size(); i++)
        for (int j = 0; j<gen.hmap.y_size(); j++)
          vect[counter++] = gen.hmap[i][j];
      std::sort(vect.begin(),vect.end());
      int index = w*h*sea_level;
      avg = vect[(int)(w*h*sea_level)]; 
      vect.clear();
      double a = gen.hmap.average();
      summ+= avg/a;
    }
    fprintf(f,"%f",summ/50);
    fputs(" ",f);
    //fprintf(fr, "%f", curr);
  //  fputs(" ", fr);
    std::cout<<"Average ratio for "<<std::fixed<<curr<<" and with sea percentage of "<<sea_level*100<<"% is "<<summ/50<<std::endl;
  }
  fclose(f);
}
 /* system("pause");
  fclose(f);
  fclose(fr);
  return 0;
  std::cout << "time consumed for generation: " << std::fixed<<(double)(clock() - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
  start = clock();
  CreateBmp24("C:\\Output\\bit.bmp",gen.hmap,avg);
  end = clock();
  double time_cons = (end - start) / ((w*h)*CLOCKS_PER_SEC);
  std::cout<<"time consumed for drawing bitmap: "<<(double)(end-start)/CLOCKS_PER_SEC<<" seconds"<<std::endl<<w*h<<" points calculated"<<std::endl<<time_cons<<" time per point"<<std::endl;
  system("pause");*/
}