#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <cstdarg>
#include <vector>
#include <algorithm>

using namespace std;
#define N 4
#define INF 1000000

// bit ������ ���� ������ �����ϴµ� ����� �Լ��� ��Ƶ� Set class.
class Set
{
public:

	//���պ����ȿ� ��� ���Ұ� �ִ��� Ȯ���ϰ� return ���ִ� �Լ�
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

	//���պ��� �ȿ� ���Ұ� �����ϴ� index ��ġ�� return ���ִ� �Լ�
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

	//������ Ư�� index ��ġ�� ���Ұ� �����ϴ��� Ȯ���ϴ� �Լ�
	bool check_ifhas(long long int subset, int index)
	{
		return (subset & 1 << index) ? true : false;
	}

	//������ ������ִ� �Լ�.
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



	//for all subsets V-{v1} containing k verticices �� �����ϱ� ���� ���տ����� �����ߴ�. nCr
	vector<long long int> combination(int n, int r)
	{
		// r���� 1�� n-r���� 0���� �̷���� ������ ���� ������� 1�� �ش��ϴ� index�� ����� ������ �����ߴ�.
		vector<long long int> vect;
		vector<int> permu;

		long long int temp = 0;
		// r���� 1
		for (int i = 0; i < r; i++) {
			permu.push_back(1);
		}

		// n-r���� 0
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

	//���� ���� �Լ������� Ư�� index�� ���� ������ �����Ѵ�.
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


// DSet�� ������ ǥ���� vector��, value�� �� ���տ� ���� ���� ���ִ� ����ü.
struct setArray
{
	int init = -1;
	vector<long long int> DSet;
	vector<int> value;

};
//D, value ��� �Լ�
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
//traveling salesperson algorithm ����.
int travel(int W[N][N])
{
	int i, k, v, vv;
	vector<int> tempv;

	Set s = Set();
	//D, P ���
	struct setArray D[N];
	struct setArray P[N];

	//�ʱⰪ ���� D[vi][������]
	for (i = 1; i < N; i++)
	{
		D[i].init = W[i][0];

	}
	//vertice�� ���� 1~ N-1���� �ݺ�.
	for (k = 1; k <= N - 1; k++)
	{
		///��� ������ subset�� ����.
		vector<long long int> possible_subset = s.combination_w(N, k, 0);
		for (auto subset = possible_subset.begin(); subset != possible_subset.end(); ++subset)
		{

			for (i = 1; i < N; i++)
			{
				///������ D[1][all]�� ���� ����ó��.
				if (k == N - 1)
				{
					i = 0;
				}

				///������ subset���� i�� �����ϰ� �ִ� subset���� ����
				if (s.check_ifhas(*subset, i)) continue;
				else
				{
					///DSet�� �켱������ ���ο� subset �߰�
					D[i].DSet.push_back(*subset);
					/// min ���� ���ϱ� ���� j ���� ���� �迭����.
					vector<int> possible_j = s.possible_vector(*subset, N);
					int temp;
					temp = INF;
					vv = -1;

					/// ������ j ���鿡 ���� �ݺ���
					for (auto j = possible_j.begin(); j != possible_j.end(); ++j)
					{
						int t = *j;

						/// ���� j ���� index�� ������ ������ subset
						long long int tempset = *subset - (1 << t);

						///subset�� ���� 1�� ���ų� �Ʒ��� ��쿡 ���� ����ó��
						if (s.set_element_count(*subset) <= 1)
						{

							temp = W[i][t] + D[t].init;
							if (temp > INF) temp = INF;

						}
						else {
							/// DSet �� value�� index�� ���� �ش��ϴ� ���� ������ �ִ� ���� �̿��Ͽ�, Dset���� tempset�� �˻��Ͽ�, �� index���� value�� �־ �ʿ��� ���� �˻��Ѵ�.
							if ((v = W[i][t] + D[t].value[distance(D[t].DSet.begin(), find(D[t].DSet.begin(), D[t].DSet.end(), tempset))]) < temp)
							{
								/// �� ���� ���� ������� ��ȯ
								temp = v;
								vv = *j;
							}
						}
					}

					///D�� P�� ���ο� ������ �־��ش�.
					D[i].value.push_back(temp);
					P[i].value.push_back(vv);

					///������ minlength�� ���ϴµ� �ʿ��� �κ�
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