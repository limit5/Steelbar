#ifndef _PARAMETER_H_
#define _PARAMETER_H_

#include <string>

class parameter
{
public:
	parameter();
	~parameter();
	int vTemplateSizeX;
	int vTemplateSizeY;
	int hTemplateSizeX;
	int hTemplateSizeY;
	int nVtemplate;
	int nHtemplate;
	float cellSizeDx;
	float cellSizeDy;
	int patternSizeNx; 
	int patternSizeNy; 
	float diameter;
	std::string errMsg;
private:

};

#endif