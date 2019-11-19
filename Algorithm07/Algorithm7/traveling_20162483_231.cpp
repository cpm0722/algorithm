#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <cstdarg>
#include <vector>
#include <algorithm>

using namespace std;
#define N 4
#define INF 1000000

// bit 연산을 통해 집합을 구현하는데 사용한 함수를 모아둔 Set class.
class Set
{
public:

	//집합변수안에 몇개의 원소가 있는지 확인하고 return 해주는 함수
	int set_element_count(long long int set)
	{
		int count = 0;
		while (set != 0)
		{
			if (set & 1)count++;
			set = set >> 1;
		}
		return count;
	}

	//집합변수 안에 원소가 존재하는 index 위치를 return 해주는 함수
	vector<int> possible_vector(long long int subset, int set_size)
	{
		vector<int> vect;
		int i = 0;
		while (i < set_size)
		{
			if (subset & (1 << i))
			{
				vect.push_back(i);
			}
			i++;
		}
		return vect;
	}

	//집합의 특정 index 위치에 원소가 존재하는지 확인하는 함수
	bool check_ifhas(long long int subset, int index)
	{
		return (subset & 1 << index) ? true : false;
	}

	//집합을 만들어주는 함수.
	long long int toggle(int args, ...)
	{
		va_list ap;

		__crt_va_start(ap, args);

		long long int set = 0;
		for (int i = 0; i < args; i++)
		{
			int t = __crt_va_arg(ap, int);
			set = set | (int)(pow(2, t));
		}

		__crt_va_end(ap);
		return set;

	}



	//for all subsets V-{v1} containing k verticices 를 구현하기 위해 조합연산을 구현했다. nCr
	vector<long long int> combination(int n, int r)
	{
		// r개의 1과 n-r개의 0으로 이루어진 순열을 만든 결과에서 1에 해당하는 index를 사용해 조합을 구현했다.
		vector<long long int> vect;
		vector<int> permu;

		long long int temp = 0;
		// r개의 1
		for (int i = 0; i < r; i++) {
			permu.push_back(1);
		}

		// n-r개의 0
		for (int i = 0; i < n - r; i++) {
			permu.push_back(0);
		}

		sort(permu.begin(), permu.end());

		do {

			for (int i = 0; i < permu.size(); i++) {
				if (permu[i] == 1) {
					temp = temp | (1 << i);

				}
			}
			vect.push_back(temp);
			temp = 0;
		} while (next_permutation(permu.begin(), permu.end()));

		return vect;
	}

	//위와 같은 함수이지만 특정 index를 비우고 조합을 생성한다.
	vector<long long int> combination_w(int n, int r, int without)
	{
		vector<long long int> vect;
		vector<int> permu;

		long long int temp = 0;

		for (int i = 0; i < r; i++) {
			permu.push_back(1);
		}

		for (int i = 0; i < n - r; i++) {
			permu.push_back(0);
		}

		sort(permu.begin(), permu.end());

		do {

			for (int i = 0; i < permu.size(); i++) {
				if (permu[i] == 1) {
					temp = temp | (1 << i);

				}
			}

			if (temp&(1 << without))
			{
			}
			else
			{
				vect.push_back(temp);
			}
			temp = 0;
		} while (next_permutation(permu.begin(), permu.end()));

		return vect;
	}

};


// DSet는 집합을 표현한 vector고, value는 각 집합에 대한 값이 들어가있는 구조체.
struct setArray
{
	int init = -1;
	vector<long long int> DSet;
	vector<int> value;

};
//D, value 출력 함수
void printSetArray(struct setArray A[N])
{
	cout << endl << endl << "D set values : presented in binary form" << endl;
	for (int k = 0; k < N; k++) {
		for (vector<long long int>::const_iterator i = A[k].DSet.begin(); i != A[k].DSet.end(); ++i)
			cout << *i << ' ';
		cout << endl;
	}
	cout << endl << endl << endl << "D values :" << endl;

	for (int k = 0; k < N; k++) {
		cout << A[k].init << ' ';
		for (vector< int>::const_iterator i = A[k].value.begin(); i != A[k].value.end(); ++i)
			cout << *i << ' ';
		cout << endl;
	}
}
//traveling salesperson algorithm 구현.
int travel(int W[N][N])
{
	int i, k, v, vv;
	vector<int> tempv;

	Set s = Set();
	//D, P 행렬
	struct setArray D[N];
	struct setArray P[N];

	//초기값 세팅 D[vi][공집합]
	for (i = 1; i < N; i++)
	{
		D[i].init = W[i][0];

	}
	//vertice의 수를 1~ N-1까지 반복.
	for (k = 1; k <= N - 1; k++)
	{
		///모든 가능한 subset을 생성.
		vector<long long int> possible_subset = s.combination_w(N, k, 0);
		for (auto subset = possible_subset.begin(); subset != possible_subset.end(); ++subset)
		{

			for (i = 1; i < N; i++)
			{
				///마지막 D[1][all]에 대한 예외처리.
				if (k == N - 1)
				{
					i = 0;
				}

				///생성한 subset들중 i를 포함하고 있는 subset들을 생략
				if (s.check_ifhas(*subset, i)) continue;
				else
				{
					///DSet에 우선적으로 새로운 subset 추가
					D[i].DSet.push_back(*subset);
					/// min 값을 구하기 위해 j 값에 대한 배열생성.
					vector<int> possible_j = s.possible_vector(*subset, N);
					int temp;
					temp = INF;
					vv = -1;

					/// 가능한 j 값들에 대해 반복문
					for (auto j = possible_j.begin(); j != possible_j.end(); ++j)
					{
						int t = *j;

						/// 현재 j 값의 index를 제외한 나머지 subset
						long long int tempset = *subset - (1 << t);

						///subset의 값이 1과 같거나 아래인 경우에 대한 예외처리
						if (s.set_element_count(*subset) <= 1)
						{

							temp = W[i][t] + D[t].init;
							if (temp > INF) temp = INF;

						}
						else {
							/// DSet 과 value는 index에 서로 해당하는 값을 가지고 있는 점을 이용하여, Dset에서 tempset을 검색하여, 그 index값을 value에 넣어서 필요한 값을 검색한다.
							if ((v = W[i][t] + D[t].value[distance(D[t].DSet.begin(), find(D[t].DSet.begin(), D[t].DSet.end(), tempset))]) < temp)
							{
								/// 더 작은 값이 있을경우 교환
								temp = v;
								vv = *j;
							}
						}
					}

					///D와 P에 새로운 값들을 넣어준다.
					D[i].value.push_back(temp);
					P[i].value.push_back(vv);

					///마지막 minlength를 구하는데 필요한 부분
					if (k == N - 1)
					{
						cout << "Minimum traveling path value for this W is " << D[0].value[0] << endl;
						printSetArray(D);
						return D[0].value[0];
					}
				}
			}
		}

	}




}
int main()
{
	Set s = Set();

	int W[N][N] =
	{
{0,2,9,INF},
		{1,0,6,4},
		{INF,7,0,8},
		{6,3,INF,0}
	};


	travel(W);
	return 0;
}