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
  // ������� ������ ���������
  BITMAPFILEHEADER bfh;
  BITMAPINFOHEADER bih;
  BYTE Palette[1024];									// �������

                                      // ����� � ��� ����� �������� �������� 35 x 50 ��������
  int Width = hmap.y_size();
  int Height = hmap.x_size();
  memset(Palette, 0, 1024);								// � ������� � ��� ����
  
                                          // �������� ��
  memset(&bfh, 0, sizeof(bfh));
  bfh.bfType = 0x4D42;									// ���������, ��� ��� bmp 'BM'
  bfh.bfOffBits = sizeof(bfh) + sizeof(bih) + 1024;		// ������� �������� 1Kb, �� �� ��� ����������� �� �����
  bfh.bfSize = bfh.bfOffBits +
    sizeof(color) * Width * Height +
    Height * (Width % 4);						// ��������� ������ ��������� �����

  memset(&bih, 0, sizeof(bih));
  bih.biSize = sizeof(bih);								// ��� ��������
  bih.biBitCount = 24;									// 16 ��� �� �������
  bih.biCompression = BI_RGB;								// ��� ������
  bih.biHeight = Height;
  bih.biWidth = Width;
  bih.biPlanes = 1;										// ������ ���� 1
                                      // � ��������� ���� �������� 0
  hFile = CreateFile(fname, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
  if (hFile == INVALID_HANDLE_VALUE)
    return;

  // ������� ���������
  WriteFile(hFile, &bfh, sizeof(bfh), &RW, NULL);
  WriteFile(hFile, &bih, sizeof(bih), &RW, NULL);
 // double avg  = hmap.average();
  // ������� �������
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
    // ��������� �� �������
    WriteFile(hFile, Palette, Width % 4, &RW, NULL);
  }

  CloseHandle(hFile);
}



int main() {
  srand(time(NULL));
  Generator gen;
  int h,w;
  clock_t start,end;
  double r;
  std::cout<<"Input height: ";
  std::cin>>h;
  std::cout << "Input width: ";
  std::cin >> w;
  std::cout << "Input roughness: ";
  std::cin >> r;
  start =clock();
  gen.Generate(h,w,r);
  double avg = gen.hmap.average();
  for (int i=0;i<gen.hmap.x_size();i++)
    for (int j=0;j<gen.hmap.y_size();j++)
      gen.hmap[i][j]= pow(gen.hmap[i][j],2);
  std::vector<double> vect(w*h);
  int counter = 0;
  for (int i = 0; i<gen.hmap.x_size(); i++)
    for (int j = 0; j<gen.hmap.y_size(); j++)
      vect[counter++] = gen.hmap[i][j];
  std::sort(vect.begin(),vect.end());
  avg = vect[(int)((w*h*3)/4)];
  std::cout << "time consumed for generation: " << (double)(clock() - start) / CLOCKS_PER_SEC << " seconds" << std::endl;
  start = clock();
  CreateBmp24("C:\\Output\\bit.bmp",gen.hmap,avg);
  end = clock();
  double time_cons = (end - start) / ((w*h)*CLOCKS_PER_SEC);
  std::cout<<"time consumed for drawing bitmap: "<<(double)(end-start)/CLOCKS_PER_SEC<<" seconds"<<std::endl<<w*h<<" points calculated"<<std::endl<<time_cons<<" time per point"<<std::endl;
  system("pause");
}