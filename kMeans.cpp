#include "IHPC.h"

class kMeans : public IHPC
{
private:
	double  MAXVALUE;
	int values_size, K_number;
	vector< pair <double, int> > averages; /*where the first is the value, and the sec is the number of those who belong to that indx(k value) */
	vector< pair <double, int> > values; /*where the first is the value, and the sec is what K-value (class) that value belong to :D*/
	vector <int> Randoms;
	bool callSetRand;


	void setRandomMeans()
	{
		srand(time(NULL));
		vector <bool> vis(values_size);

		for (int i = 0; i < this->K_number; )
		{
			int indx = rand()%values_size;
			if (!vis[indx])
			{
				vis[indx] = 1;
				Randoms.push_back(indx);
				++i;
			}
		}

		//Randoms.push_back(4);
		//Randoms.push_back(7);
		//Randoms.push_back(3);
	}

	void intit_kMeans()
	{
		if (callSetRand) 
			setRandomMeans();
		callSetRand = false;
		for (int i = 0; i < K_number; i++)
		{
			averages[i].first = Randoms[i];
		}

		//f.push_back(values[1].first);
		//averages[0].first = values[1].first;
		//f.push_back(values[0].first);
		//averages[1].first = values[0].first;
		//f.push_back(values[2].first);
		//averages[2].first = values[2].first;

	}

public:
	kMeans () 
	{
		MAXVALUE = 1e6+100;
		callSetRand = true;
	}
	~kMeans()
	{
	}
	void getInputs() {}
	void testInputs() 
	{
		this->K_number = 3;
		this->values_size = 10;

		averages.resize(K_number);
		values.resize(values_size);
		
		values[0].first = .0932;
		values[1].first = 3.9414;
		values[2].first = .2997;
		values[3].first = 4.6875;
		values[4].first = 3.8272;
		values[5].first = 4.6998;
		values[6].first = 1.8072;
		values[7].first = 1.5256;
		values[8].first = 2.3518;
		values[9].first = 4.4015;
		

		//values[0].first = 0;
		//values[1].first = 1;
		//values[2].first = 2;
		//values[3].first = 7;
		//values[4].first = 8;
		//values[5].first = 9;
		//values[6].first = 15;
		//values[7].first = 16;
		//values[8].first = 17;
	}

	//void runSerialSoulation2() 
	//{
	//	this->intit_kMeans();
	//	for (int iteration = 0; iteration < 20; ++iteration)
	//	{
	//		for (int indx = 0; indx < values_size; ++indx)
	//		{
	//			double curVal = values[indx].first;
	//			double min = this -> MAXVALUE;
	//			int finalIndx;

	//			//Clustering step
	//			for (int cluster = 0; cluster < this->K_number; ++cluster)
	//			{
	//				if (abs(curVal - averages[cluster].first) < min)
	//				{
	//					min = abs(curVal - averages[cluster].first);
	//					finalIndx = cluster;
	//					//finalIndx_S.push_back(finalIndx);
	//				}
	//			}

	//			values[indx].second = finalIndx;
	//			++averages[finalIndx].second;
	//		}
	//		//done Clustering step

	//		//update mean step

	//		double sum;
	//		for (int cluster = 0; cluster < K_number; ++cluster)
	//		{
	//			sum = 0;
	//			for (int val = 0; val < values_size; ++val)
	//			{
	//				if (cluster == values[val].second)
	//					sum += values[val].first;	
	//			}
	//			//if (!iteration)
	//			//	printf("cluster = %d, value of this cluster = %d, sum of values belong to this cluster = %d, #items of that cluster = %d\n\n\n", cluster, sum, averages[cluster].first, averages[cluster].second);
	//			averages[cluster].first = sum/averages[cluster].second;
	//			averages[cluster].second = 0;
	//			//done update mean step
	//		}
	//	}

	//	for(int i = 0; i<K_number; ++i)
	//	{
	//		cout << "the cluster number " << i << " has value of " << averages[i].first <<"\n"; 
	//	}
	//}

	//
	void runSerialSoulation()
	{
		this->intit_kMeans();

		vector < pair < double, int> > data (values_size, make_pair(this->MAXVALUE, -1)); /*اللي تحت في كشكول حسن الأزرق صفحة 45*/
		vector < double > sum(K_number, 0);

		for (int i = 0; i<values_size; ++i) values[i].second = 0;

		for (int iteration = 0; iteration < 20; ++iteration)
		{
			//#pragma omp parallel
			for (int kk = K_number - 1; kk > -1; --kk)
			{
				//int clusterID = omp_get_thread_num();
				int clusterID = kk;
				for (int indx = 0; indx < values_size; ++indx)
				{
					//clusterong step
					double curVal = values[indx].first;
					double diff = abs(curVal - averages[clusterID].first);
					#pragma omp critical
					{
						if (diff < data[indx].first)
						{
							if (data[indx].second != -1) 
							{
								averages[data[indx].second].second--;
								sum[data[indx].second] -= curVal;
							}
							data[indx].first = diff;
							data[indx].second = clusterID;
							averages[clusterID].second++;
							sum[clusterID] += curVal;
						}
					}
				}
			}
			for (int clusterID = 0; clusterID < K_number; ++clusterID)
			{
				if (!averages[clusterID].second)
					averages[clusterID].first = 0;
				else
				averages[clusterID].first = sum[clusterID] / averages[clusterID].second;
			}
			for (int i = 0; i < data.size(); ++i)
				data[i] =  make_pair(this->MAXVALUE, -1);
			//sum.resize(sum.size(), 0);
			for (int i = 0; i < sum.size(); ++i)
				sum[i] = 0;
			for (int i = 0; i < K_number; ++i)
				averages[i].second = 0;

		}
		for(int i = 0; i<K_number; ++i)
		{
			cout << "the cluster number " << i << " has value of " << averages[i].first <<"\n"; 
		}
	}

	void runCPUSoluation()
	{
		this->intit_kMeans();

		omp_set_num_threads(K_number);
		vector < pair < double, int> > data (values_size, make_pair(this->MAXVALUE, -1)); /*اللي تحت في كشكول حسن الأزرق صفحة 45*/
		vector < double > sum(K_number, 0);

		for (int i = 0; i<values_size; ++i) values[i].second = 0;

		for (int iteration = 0; iteration < 20; ++iteration)
		{
			#pragma omp parallel
			{
				#pragma omp for schedule (static, 1)
				for (int kk = K_number - 1; kk > -1; --kk)
				{
					//int clusterID = omp_get_thread_num();
					int clusterID = kk;
					for (int indx = 0; indx < values_size; ++indx)
					{
						//clusterong step
						double curVal = values[indx].first;
						double diff = abs(curVal - averages[clusterID].first);
						#pragma omp critical
						{
							if (diff < data[indx].first)
							{
								if (data[indx].second != -1) 
								{
									averages[data[indx].second].second--;
									sum[data[indx].second] -= curVal;
								}
								data[indx].first = diff;
								data[indx].second = clusterID;
								averages[clusterID].second++;
								sum[clusterID] += curVal;
							}
						}
					}
			}
				//unseen barrier
			#pragma omp for schedule (static, 1)
				for (int clusterID = 0; clusterID < K_number; ++clusterID)
				{
					if (!averages[clusterID].second)
						averages[clusterID].first = 0;
					else
						averages[clusterID].first = sum[clusterID] / averages[clusterID].second;
				}
			}
			for (int i = 0; i < data.size(); ++i)
				data[i] =  make_pair(this->MAXVALUE, -1);
			//sum.resize(sum.size(), 0);
			for (int i = 0; i < sum.size(); ++i)
				sum[i] = 0;
			for (int i = 0; i < K_number; ++i)
				averages[i].second = 0;

		}
		for(int i = 0; i<K_number; ++i)
		{
			cout << "the cluster number " << i << " has value of " << averages[i].first <<"\n"; 
		}
	}

//	void runCPUSoluation2()
//	{
//		this->intit_kMeans();
//
//		omp_set_num_threads(K_number);
//		vector < pair < double, int> > data (values_size, make_pair(this->MAXVALUE, -1)); /*اللي تحت في كشكول حسن الأزرق صفحة 45*/
//		vector < double > sum(K_number, 0);
//
//		for (int i = 0; i<values_size; ++i) values[i].second = 0;
//
//		for (int iteration = 0; iteration < 20; ++iteration)
//		{
//#pragma omp parallel
//			{
//				int clusterID = omp_get_thread_num();
//				for (int indx = 0; indx < values_size; ++indx)
//				{
//					//clusterong step
//					double curVal = values[indx].first;
//					double diff = abs(curVal - averages[clusterID].first);
//					if (diff < data[indx].first)
//					{
//						if (data[indx].second != -1) 
//						{
//							averages[data[indx].second].second--;
//							sum[data[indx].second] -= curVal;
//						}
//						data[indx].first = diff;
//						data[indx].second = clusterID;
//						averages[clusterID].second++;
//						sum[clusterID] += curVal;
//					}
//				}
//				#pragma omp barrier
//
//#pragma omp critical
//				{
//					vector <pair <double, int> > *temp = new vector < pair <double, int > > (averages);
//					vector <double> *s = new vector <double>(sum);
//					averages[clusterID].first = sum[clusterID] / averages[clusterID].second;
//				}
//
//				for (int i = 0; i < data.size(); ++i)
//					data[i] =  make_pair(this->MAXVALUE, -1);
//				for (int i = 0; i < sum.size(); ++i)
//					sum[i] = 0;
//				for (int i = 0; i < K_number; ++i)
//					averages[i].second = 0;
//				#pragma omp barrier
//			}
//		}
//		for(int i = 0; i<K_number; ++i)
//		{
//			cout << "the cluster number " << i << " has value of " << averages[i].first <<"\n"; 
//		}
//	}

	void runGPUSoluation() {}
	void randomInputs() 
	{
		srand(time(NULL));
		this->values_size = rand()%(10000000+1);
		this->K_number = rand() % ((values_size/2)+1);

		values_size = 5;
		K_number = 2;
		values.resize(values_size);
		for (int i = 0; i < values_size; ++i)
		{
			values[i].first = rand() % 101;
			cout << values[i].first << " ";
		}
		cout << endl;
		averages.resize(K_number);
	}
	void compareResults() {}
};
