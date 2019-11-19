#include<stdio.h>
#include<math.h>

class Set
{
private:
	int byte;
	int *source;
	int sourceNum;
	int count;

public:
	Set() {}
	Set(int, int[], int);
	Set(const Set &);
	Set(int[], int [], int);
	~Set() {}
	Set operator+(const int &);
	Set operator+(const Set &);
	Set operator-(const int &);
	Set operator-(const Set &);
	Set operator&&(const Set &);
	Set operator=(const Set &);
	Set Complement();
	bool find(const int &) const;
	int getByte() const { return byte; }
	void putByte(int a) { byte = a; }
	int * getSource() const { return source; }
	int getSourceNum() const { return sourceNum; }
	int getCount() const { return count; }
	int* getIndex();
	void PrintSet();

};


