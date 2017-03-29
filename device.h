#include"shape.h"



class Device {
public:

	//window's weight and height
	int nx, ny;

	//color buffer in backbuffer, 32 bit system , 4bytes per pixel( rank for B-G-R-A style).  
	BYTE* pBuffer;

	//depth buffer in backbuffer;
	char* zBuffer;

	//texture buffer
	int* textBuffer;

	//texture weight and height
	int textx, texty;

	Device() { nx = ny = textx = texty = 0; pBuffer = NULL; zBuffer = NULL; textBuffer = NULL; }

};


void SetDevice(Device *device,int nx, int ny, int textx, int texty) {
	
	// set the backbuffer size (unit: byte)
	int bufferSize = nx*ny * 8;

	//create nx*ny*4*2 bytes buffer, store char style,and return a pointer to the first char's address;
	char* ptr = (char*)malloc(bufferSize);

	//let the pointers of pBuffer and zBuffer point to buffer;
	//device->pBuffer = (char*)ptr;
	//device->zBuffer = (char*)ptr + nx*ny * 4;

	for (int j = 0; j < ny; j++)
		for (int i = 0; i < nx*4; i = i+4) {
			device->pBuffer[j+i] = (char)0;
			device->pBuffer[j+i+1]= (char)0;
			device->pBuffer[j+i+2] = (char)0;
			device->pBuffer[j + i + 3] = (char)0;
		}
}
