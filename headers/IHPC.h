#pragma once
#include "includes.h"

class IHPC
{
public:
	virtual ~IHPC(){}

	virtual void getInputs() = 0;
	virtual void testInputs() = 0;
	virtual void runSerialSoulation() = 0;
	virtual void runCPUSoluation() = 0;
	virtual void runGPUSoluation() = 0;
	virtual void randomInputs() = 0;
	virtual void compareResults() = 0;
};
