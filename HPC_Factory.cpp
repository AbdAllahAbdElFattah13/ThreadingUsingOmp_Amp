//#pragma once
//#include "IHPC.h"
#include "TaskEnum.h"
#include "kMeans.cpp"
#include "TextSearch.cpp"
#include "MandelbrotFractals.cpp"
#include "LCS.cpp"
class HPC_Factory
{
public:
	HPC_Factory() {}
	//~HPC_Factory();
	IHPC* GetTask(TaskEnum eTask)
	{
		IHPC *obj = NULL;
		switch (eTask)
		{
		case lcs:
			obj = new LCS();
			break;
		case KMeans:
			obj = new kMeans();
			break;
		case TextSearch:
			obj = new TextSearh();
			break;
		case MandelbrotFractals:
			obj = new Mandelbrot_Fractals();
			break;
		default:
			break;
		};
		return obj;
	}
private:

};
