#include"IHPC.h"

class TextSearh : public IHPC
{
private:
	string Text;
	string KeyWord;
	double startTime;
	double endTime;
	double ResultTime;
public:
	void testInputs()
	{
		Text = "Boda is testing nooooow  :D";
		KeyWord = ":D";
	}
	void getInputs()
	{
		cin.ignore();
		cout << "Enter Text : ";
		getline(cin, Text);
		cout << "Enter KeyWord : ";
		getline(cin, KeyWord);
	}
	void runSerialSoulation()
	{
		int Size;
		string Sub;
		Size = KeyWord.size();
		startTime = omp_get_wtime();
		int j = 0;
		for ( ; j < Text.size(); j++)
		{
			Sub = Text.substr(j, Size);
			if (Sub == KeyWord)
			{
				cout << "Serial Soulation found at postion: ";
				cout << j << endl;
				break;
			}
		}
		endTime = omp_get_wtime();
		ResultTime = (endTime - startTime);
		cout << "sTime for TextSearch :" << endl;
		cout << ResultTime << endl;
		if (j == Text.size())
		{
			cout << "Not Found :( .. sorry dude!" << endl;
		}

	}
	void runCPUSoluation()
	{
		bool flag = true;
		string Sub;
		int Size;
		int id;
		int j;
		Size = KeyWord.size();
		startTime = omp_get_wtime();
		omp_set_num_threads(4);
#pragma omp parallel private(id,j,Sub)
		{
			string Local = Text;
			id = omp_get_thread_num();
			if (flag)
			{
				for (j = id; j <= Text.size() - Size; j += Size)
				{
					if (flag == false)
						break;
					Sub = Local.substr(j, Size);
					if (Sub == KeyWord)
					{
						flag = false;
						cout << "found by thread num: " << omp_get_thread_num() << endl;
						cout << "Founded at position: " << j << endl;
						break;
					}
				}
			}
		}
		endTime = omp_get_wtime();
		ResultTime = (endTime - startTime);
		cout << "pTime for TextSearch :" << endl;
		cout << ResultTime << endl;
		if (flag)
		{
			cout << "Not Found :( .. sorry dude!" << endl;
		}
	}


	
	void runGPUSoluation() 
	{

	}
		void randomInputs()
	{
		Text.clear();
		KeyWord.clear();
		srand(time(NULL));
		char arr[26];
		for (char i = 'a'; i <= 'z'; i++)	arr[i-'a'] = i;	
		int length1, length2;
		length1 = rand() %1000000;
		length2 = rand() %1000000;
		for (int i = 0; i < length1; i++) Text += arr[rand()%26];
		for (int i = 0; i < length2; i++)	 KeyWord += arr[rand()%26];
	}
	void compareResults()
	{

	}
};
