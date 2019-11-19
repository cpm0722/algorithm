#include<stdio.h>
#include<math.h>
#include<iostream>

template <typename T>
class Set
{
private:
	int byte;
	T *source;
	int sourceNum;
	int count;

public:
	Set() {}
	Set(int, T *, int);
	Set(const Set &);
	Set(int[], T *, int);
	~Set() {}
	Set<T> operator+(const int &);
	Set<T> operator+(const Set<T> &);
	Set<T> operator-(const int &);
	Set<T> operator-(const Set<T> &);
	Set<T> operator&&(const Set<T> &);
	Set<T> operator=(const Set<T> &);
	Set<T> operator=(const int);
	Set<T> Complement();
	bool isEmpty(void);
	bool find(const int &) const;
	int getByte() const { return byte; }
	void putByte(int a) { byte = a; }
	T * getSource() const { return source; }
	int getSourceNum() const { return sourceNum; }
	int getCount() const { return count; }
	int* getIndex();
	void PrintSet();

};

int ByteToCount(int byte, int sourceNum) {
	int cnt = 0;
	for (int i = 0; i < sourceNum; i++) {
		if ((byte & 1) == 1)
			cnt++;
		byte = byte >> 1;
	}
	return cnt;
}

template <typename T>
Set<T>::Set(int iByte, T *iSource, int iNum) {
	byte = iByte;
	source = iSource;
	sourceNum = iNum;
	count = ByteToCount(byte, sourceNum);
}

template <typename T>
Set<T>::Set(const Set<T> &b) {
	byte = b.getByte();
	source = b.getSource();
	sourceNum = b.getSourceNum();
	count = ByteToCount(byte, sourceNum);
}

//배열에 들어있는 index들을 집합에 추가
template <typename T>
Set<T>::Set(int arr[], T *iSource, int iNum) {
	byte = 0;
	int len = sizeof(arr) / sizeof(int);
	for (int i = 0; i < len; i++) {
		byte += (int)pow(2, arr[i]);
	}
	source = iSource;
	sourceNum = iNum;
	count = ByteToCount(byte, sourceNum);
}

//합집합 구현
//Set에 int를 더할 경우, int를 index로 인식
template <typename T>
Set<T> Set<T>::operator+(const int &b) {
	int tempByte = 0;
	int aByte = byte;
	int bByte = (int)pow(2, b);
	tempByte = aByte | bByte;
	return Set(tempByte, source, sourceNum);
}

//합집합 구현
template <typename T>
Set<T> Set<T>::operator+(const Set<T> &b){
	int tempByte = 0;
	int aByte = byte;
	int bByte = b.getByte();
	tempByte = aByte | bByte;
	return Set(tempByte, source, sourceNum);
}

//차집합 구현
//Set에 int를 뺄 경우, int를 index로 인식
template <typename T>
Set<T> Set<T>::operator-(const int &b) {
	int tempByte = 0;
	int andByte = (int)pow(2, b);
	//and의 보수를 and하면서 차집합 구현
	tempByte = byte & ~andByte;
	return Set(tempByte, source, sourceNum);
}

//차집합 구현
template <typename T>
Set<T> Set<T>::operator-(const Set<T> &b) {
	int tempByte = 0;
	int andByte = byte & b.getByte();
	//and의 보수를 and하면서 차집합 구현
	tempByte = byte & (~andByte);
	return Set(tempByte, source, sourceNum);
}

//교집합 구현
template <typename T>
Set<T> Set<T>::operator&&(const Set<T> &b) {
	int tempByte = 0;
	tempByte = byte & b.getByte();
	return Set(tempByte, source, sourceNum);
}

//대입 연산자 구현
template <typename T>
Set<T> Set<T>::operator=(const Set<T> &b) {
	byte = b.getByte();
	count = b.getCount();
	return *this;

}

//대입 연산자 구현 (인덱스)
template <typename T>
Set<T> Set<T>::operator=(int n) {
	if ((n > 0) && (n < sourceNum)) {
		byte = (int)pow(2, n);
		count = 1;
	}
	else {
		byte = 0;
		count = 0;
	}

	return *this;
}

//여집합 구현
template <typename T>
 Set<T> Set<T>::Complement() {
	int tempByte = ~byte;
	//allByte: 전체집합
	int allByte = 0;
	for (int i = 0; i < sourceNum; i++) {
		allByte |= 1;
		allByte = allByte << 1;
	}
	allByte = allByte >> 1;
	//전체집합에서 여집합 구함
	tempByte = tempByte & allByte;
	return Set(tempByte, source, sourceNum);
}

 //공집합인지 판별
template <typename T>
bool Set<T>::isEmpty(void) {
	if (count == 0)
		return true;
	else return false;
}

//해당 인덱스 집합에 포함되어있는지 확인
template <typename T>
bool Set<T>::find(const int &num) const{
	int tempByte = byte;
	tempByte = tempByte >> num;
	if ((tempByte & 1) == 1)
		return true;
	else return false;
}

//집합에 포함되는 원소들의 index 배열 생성
template <typename T>
int* Set<T>::getIndex(){
	int *index = new int[count];
	int tempByte = byte;
	int indexCount = 0;
	for (int i = 0; i < sourceNum; i++) {
		if ((tempByte & 1) == 1)
			index[indexCount++] = i;
		tempByte = tempByte >> 1;
	}
	return index;
}

//집합 내의 원소들 출력
template <typename T>
void Set<T>::PrintSet() {
	int tempByte = byte;
	if (count == 0) {
		printf("{Empty Set}\n");
		return;
	}
	putchar('{');
	for (int i = 0; i < sourceNum; i++) {
		if ((tempByte & 1) == 1) {
			std::cout << source[i] << ", ";
		}
		tempByte = tempByte >> 1;
	}
	printf("\b\b}\n");
} 
