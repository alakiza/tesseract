#ifndef CUBE_H
#define CUBE_H
#include "Visualizer.h"

struct Point2D
{
  int X;
  int Y;
};

struct Point3D : public Point2D
{
  int Z;
};

long*** GenerateMatrix(int MaxX, int MaxY, int MaxZ)
{
  long*** res = new long**[MaxX];
  for(int i = 0; i < MaxX; ++i)
  {
    res[i] = new long*[MaxY];
    for(int j = 0; j < MaxY; ++j)
    { 
      res[i][j] = new long[MaxZ];
      for(int k = 0; k < MaxZ; ++k)
        res[i][j][k] = 0;
    }
  }
  return res;
}

class Cube
{
private:
	Visualizer* fVisualizer;
	long*** dimensions;
	int flengthX;
	int flengthY;
	int flengthZ;
	inline void InitDimensions(int MaxX, int MaxY, int MaxZ)
	{
		flengthX = MaxX;
		flengthY = MaxY;
		flengthZ = MaxZ;
		dimensions = GenerateMatrix(flengthX, flendthY, flengthZ);
	}
public:
	Cube(int maxX, int MaxY, int MaxZ, Visualizer* visualizer);
	Cube(int MaxX, int MaxY, int MaxZ);

	void SetPixelColor(int x, int y, int z, long color) 
	{ 
		dimensions[x][y][z] = color;
		bool NeedToBackDirection = ((y % 2) && !(z % 2)) || (!(y % 2) && (z % 2));  
    int num;
    
    if(z%2) 
      num = z*flengthX*flengthY + (flengthY-1-y)*flengthX;
    else
      num = z*flengthX*flengthY + y*flengthX;
      
		if(NeedToBackDirection)
			num = num + flengthX - 1 - x;
		else
			num = num + x;
		fVisualizer->SetPixelColor(num, color);
	};
	void SetPixelColor(long*** StateMatrix, int MaxX, int MaxY, int MaxZ)
	{
		for(int i = 0; i < MaxX; ++i)
			for(int j = 0; j < MaxY; ++j)
				for(int k = 0; k < MaxZ; ++k)
					SetPixelColor(i, j, k, StateMatrix[i][j][k]);
	}

	long GetPixelColor(int x, int y, int z) { return dimensions[x][y][z]; }
	bool IsEqual(int x, int y, int z, long color) { return GetPixelColor(x, y, z) == color;}

	int LenX(){return flengthX;}
	int LenY(){return flengthY;}
	int LenZ(){return flengthZ;}

  void Clear(bool Showed){fVisualizer->Clear(Showed);}
  void Show(){fVisualizer->Show();}

	~Cube();
};

Cube::Cube(int MaxX, int MaxY, int MaxZ, Visualizer* visualizer)
{
	fVisualizer = visualizer;
	
	InitDimensions(MaxX, MaxY, MaxZ);
}

Cube::Cube(int MaxX, int MaxY, int MaxZ)
{
	fVisualizer = new Visualizer(8, MaxX*MaxY*MaxZ);
	InitDimensions(MaxX, MaxY, MaxZ);
}

Cube::~Cube()
{
	for(int i = 0; i < flengthX; ++i)
	{
		for(int j = 0; j < flengthX; ++j)
		{
			delete[]dimensions[i][j];
		}		
		delete[] dimensions[i];
	}
	delete[] dimensions;
}

#endif
