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

//�迭�� ����ִ� index���� ���տ� �߰�
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

//������ ����
//Set�� int�� ���� ���, int�� index�� �ν�
template <typename T>
Set<T> Set<T>::operator+(const int &b) {
	int tempByte = 0;
	int aByte = byte;
	int bByte = (int)pow(2, b);
	tempByte = aByte | bByte;
	return Set(tempByte, source, sourceNum);
}

//������ ����
template <typename T>
Set<T> Set<T>::operator+(const Set<T> &b){
	int tempByte = 0;
	int aByte = byte;
	int bByte = b.getByte();
	tempByte = aByte | bByte;
	return Set(tempByte, source, sourceNum);
}

//������ ����
//Set�� int�� �� ���, int�� index�� �ν�
template <typename T>
Set<T> Set<T>::operator-(const int &b) {
	int tempByte = 0;
	int andByte = (int)pow(2, b);
	//and�� ������ and�ϸ鼭 ������ ����
	tempByte = byte & ~andByte;
	return Set(tempByte, source, sourceNum);
}

//������ ����
template <typename T>
Set<T> Set<T>::operator-(const Set<T> &b) {
	int tempByte = 0;
	int andByte = byte & b.getByte();
	//and�� ������ and�ϸ鼭 ������ ����
	tempByte = byte & (~andByte);
	return Set(tempByte, source, sourceNum);
}

//������ ����
template <typename T>
Set<T> Set<T>::operator&&(const Set<T> &b) {
	int tempByte = 0;
	tempByte = byte & b.getByte();
	return Set(tempByte, source, sourceNum);
}

//���� ������ ����
template <typename T>
Set<T> Set<T>::operator=(const Set<T> &b) {
	byte = b.getByte();
	count = b.getCount();
	return *this;

}

//���� ������ ���� (�ε���)
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

//������ ����
template <typename T>
 Set<T> Set<T>::Complement() {
	int tempByte = ~byte;
	//allByte: ��ü����
	int allByte = 0;
	for (int i = 0; i < sourceNum; i++) {
		allByte |= 1;
		allByte = allByte << 1;
	}
	allByte = allByte >> 1;
	//��ü���տ��� ������ ����
	tempByte = tempByte & allByte;
	return Set(tempByte, source, sourceNum);
}

 //���������� �Ǻ�
template <typename T>
bool Set<T>::isEmpty(void) {
	if (count == 0)
		return true;
	else return false;
}

//�ش� �ε��� ���տ� ���ԵǾ��ִ��� Ȯ��
template <typename T>
bool Set<T>::find(const int &num) const{
	int tempByte = byte;
	tempByte = tempByte >> num;
	if ((tempByte & 1) == 1)
		return true;
	else return false;
}

//���տ� ���ԵǴ� ���ҵ��� index �迭 ����
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

//���� ���� ���ҵ� ���
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
