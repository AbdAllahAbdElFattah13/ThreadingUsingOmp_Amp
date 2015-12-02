#include "IHPC.h"

class LCS : public IHPC
{
private:
	string a, b;/*max size = 1000000*/

public:
	LCS(){}
	~LCS(){}

	void getInputs()
	{
		cin.ignore();
		cout << "please enter the first String"<<endl;
		getline(cin, a);
		cout << "please enter the seconed String"<<endl;
		getline(cin, b);
	}
	void testInputs()
	{
		a = "computer";
		b = "houseboat";
	}
	void runSerialSoulation()
	{
		if (a.size() < b.size())
			swap(a, b);
		//int mat[200][200] = { 0 };
		int m = a.size(), n = b.size();
		vector < vector <int> > mat (m+100);
		for (int i = 0; i < mat.size(); ++i)
		{
			mat[i].resize(n+100);
		}

		int length1 = m + n - 2;
		double befor = omp_get_wtime();
		for (int i = 1; i < length1; i++)
		{
			for (int j = 0; j < min(m - 1, i); j++)
			{

				if ((i - j - 1) <= m && (j) <= n)
				{
					//printf("(%d, %d) ", i - j, j + 1);
					if (a[i - j -1] == b[j+1-1])
						mat[i - j][j + 1] = mat[i - j - 1][j] + 1;
					else
						mat[i - j][j + 1] = max(mat[i - j][j], mat[i - j - 1][j + 1]);
				}
				//cout << endl;

			}
			//cout << endl;
		}

		cout << "sTime for LCS :" << endl;
		cout << abs(omp_get_wtime() - befor) << endl;
		//for (int i = 0; i < m; i++)
		//{
		//	for (int j = 0; j < n; j++)
		//	{
		//		cout << mat[i][j] << " ";
		//	}
		//	cout << endl;
		//}
		printf("sLCS is: %d\n", mat[m - 1][n - 1] + (a[m-1] == b[n-1]));

		return;
	}
	void runCPUSoluation()
	{
		if (a.size() < b.size())
			swap(a, b);
		//int mat[200][200] = { 0 };
		int m = a.size(), n = b.size();
		vector < vector <int> > mat (m+100);
		for (int i = 0; i < mat.size(); ++i)
		{
			mat[i].resize(n+100);
		}

		omp_set_num_threads(6);
		int length1 = m + n - 2;
		double befor = omp_get_wtime();
		for (int i = 1; i < length1; i++)
		{
#pragma omp parallel
			{
				//omp_set_num_threads(min(m - 1, i));
				
				#pragma omp for
				for (int j = 0; j < min(m - 1, i); j++)
				{
					if ((i - j - 1) <= m && (j) <= n)
					{
						//printf("(%d, %d) ", i - j, j + 1);
						if (a[i - j -1] == b[j+1-1])
							mat[i - j][j + 1] = mat[i - j - 1][j] + 1;
						else
							mat[i - j][j + 1] = max(mat[i - j][j], mat[i - j - 1][j + 1]);
					}
					//cout << endl;
				}
			}
			//cout << endl;
		}

		cout << "sTime for LCS :" << endl;
		cout << abs(omp_get_wtime() - befor) << endl;
		//for (int i = 0; i < m; i++)
		//{
		//	for (int j = 0; j < n; j++)
		//	{
		//		cout << mat[i][j] << " ";
		//	}
		//	cout << endl;
		//}
		printf("sLCS is: %d\n", mat[m - 1][n - 1] + (a[m-1] == b[n-1]));

		return;
	}
	void runGPUSoluation(){}
	void randomInputs()
	{
		a.clear();
		b.clear();
		srand(time(NULL));
		char arr[26];
		for (char i = 'a'; i <= 'z'; i++)	arr[i-'a'] = i;	
		int length1, length2;
		length1 = rand() %1000000;
		length2 = rand() %1000000;
		for (int i = 0; i < length1; i++)	a += arr[rand()%26];
		for (int i = 0; i < length2; i++)	 b += arr[rand()%26];
	}
	void compareResults(){}

};
