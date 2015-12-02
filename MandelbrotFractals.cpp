#include "IHPC.h"


class Mandelbrot_Fractals : public IHPC
{
public:

	double x [2];
	double y [2];

	double SerialTime, GpuTime, ThreadsTime;

	int numberOfPointsX;
	int numberOfPointsY;
	double incrmentByX;
	double incrmentByY;
	double *planeValues, *xValues, *yValues;

	vector< vector<bool > > plane;

	void init()
	{
		plane.resize(numberOfPointsY);
		for (int i = 0; i < numberOfPointsY; ++i)
		{
			plane[i].resize(numberOfPointsX);
		}

		incrmentByX = (x[1] - x[0])/(double)numberOfPointsX;
		incrmentByY = (y[1] - y[0])/(double)numberOfPointsY;

		planeValues = new double[numberOfPointsX*numberOfPointsY];
		xValues = new double[numberOfPointsX];
		yValues = new double[numberOfPointsY];

		/*we can make the following loops  paerll loops*/
		for (int row = 0; row < numberOfPointsY; ++row)
		{
			yValues[row] = y[0] + row * incrmentByY;
		}

		for (int colum = 0; colum < numberOfPointsX; colum++)
		{
			xValues[colum] = x[0] + colum * incrmentByX;
		}
	}

	inline bool MandelbrotFractalsMethod(double a, double b)
	{
		int n = 1000;
		double zReal = 0, zImag = 0, cReal = a, cImag = b, nextZReal, nextZImag;

		while (n--)
		{
			if ((zReal * zReal) + (zImag * zImag) > 4)
				return 0;
			nextZReal = (zReal * zReal) - (zImag * zImag) + cReal;
			nextZImag = 2*(zReal * zImag) + cImag;

			zReal = nextZReal;
			zImag = nextZImag;
		}
		return 1;
	}

public:

	Mandelbrot_Fractals(){}
	~Mandelbrot_Fractals()
	{
		delete[] planeValues;
		delete[] xValues;
		delete []yValues;
	}
	void getInputs()
	{
		printf("please enter X-axis limits\n");
		for (int i = 0; i < 2; i++)
			cin >> x[i];

		printf("\nplease enter Y-axis limits\n");
		for (int i = 0; i < 2; i++)
			cin >> y[i];

		printf("\nplease enter width\n");
		cin >> numberOfPointsX;

		printf("\nplease enter height\n");
		cin >> numberOfPointsY;

		this->init();
		return;
	}
	void testInputs()
	{
		//cout << "i'm testInputs" << endl;
		x[0] = -2.5;
		x[1] = 1;
		y[0] = -1.5;
		y[1] = 1.5;

		this -> numberOfPointsX = 50;
		this -> numberOfPointsY = 40;
		this -> init();
	}
	void runSerialSoulation()
	{
		double before = omp_get_wtime();
		for (int y = 0; y < numberOfPointsY; ++y)
		{
			for (int x = 0; x < numberOfPointsX; ++x)
			{
				plane[y][x] = this->MandelbrotFractalsMethod((this->x[0] + x*incrmentByX), (this->y[0]+ y*incrmentByY));
			}
		}
		SerialTime = abs(omp_get_wtime() - before);
		cout << "Serial Soulation took " << SerialTime << "\n\nplan as follows\n";

		for (int y = 0; y < numberOfPointsY; ++y)
		{
			for (int x = 0; x < numberOfPointsX; ++x)
			{
				if (plane[y][x] == 0)
					cout << " ";
				else
					cout << "1";
			}
			cout << endl;
		}
		//resetPlane();
	}
	void runCPUSoluation(){}
	void runGPUSoluation()
	{
		array_view<double, 1> x_gpu(numberOfPointsX, xValues);
		array_view<double, 1> y_gpu(numberOfPointsY, yValues);
		array_view<double, 2> plane_gpu(numberOfPointsY, numberOfPointsX, planeValues);

		parallel_for_each(plane_gpu.extent, [=](index<2> index2D) restrict(amp) {
			int row = index2D[0];
			int col = index2D[1];


			int n = 1000;
			double zReal = 0, zImag = 0, cReal = x_gpu(col), cImag = y_gpu(row), nextZReal, nextZImag;

			while (n--)
			{
				if ((zReal * zReal) + (zImag * zImag) > 4)
					break;
				nextZReal = (zReal * zReal) - (zImag * zImag) + cReal;
				nextZImag = 2*(zReal * zImag) + cImag;

				zReal = nextZReal;
				zImag = nextZImag;
			}
			if (n > 0)
				plane_gpu(row, col) =  0;
			else
				plane_gpu(row, col) =  1;



		}
		);

		for (int y = 0; y < numberOfPointsY; ++y)
		{
			for (int x = 0; x < numberOfPointsX; ++x)
			{
				if (plane_gpu[y][x] == 0)
					cout << " ";
				else
					cout << "1";
			}
			cout << endl;
		}
		return;
	}
	void randomInputs() {}
	void compareResults() {}
};
