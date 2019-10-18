#ifndef CUBE_H
#define CUBE_H
#include "Visualizer.h"

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
		dimensions = new long**[flengthX];
		for(int i = 0; i < flengthX; ++i)
		{
			dimensions[i] = new long*[flengthY];
			for(int j = 0; j < flengthY; ++j)
			{ 
				dimensions[i][j] = new long[flengthZ];
				for(int k = 0; k < flengthZ; ++k)
					dimensions[i][j][k] = 0;
			}
		}	
	}
public:
	Cube(int maxX, int MaxY, int MaxZ, Visualizer* visualizer);
	Cube(int MaxX, int MaxY, int MaxZ);

	void SetPixelColor(int x, int y, int z, long color) 
	{ 
		dimensions[x][y][z] = color;
		bool NeedToBackDirection = ((y % 2) && !(z % 2)) || (!(y % 2) && (z % 2));  
		int num = z*flengthX*flengthY + y*flengthX;
		if(NeedToBackDirection)
			num = num + flengthX - x;
		else
			num = num + x;
		fVisualizer->SetPixelColor(num, color);
	};
	void SetPixelColor(int*** StateMatrix, int MaxX, int MaxY, int MaxZ)
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