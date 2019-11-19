#include"set.h"

int ByteToCount(int byte, int sourceNum) {
	int cnt = 0;
	for (int i = 1; i < sourceNum; i++) {
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
template <typename T>
Set<T> Set<T>::operator+(const int &b) {
	int tempByte = 0;
	int aByte = byte;
	int bByte = b;
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
template <typename T>
Set<T> Set<T>::operator-(const int &b) {
	int tempByte = 0;
	int andByte = byte & b;
	//and�� ������ and�ϸ鼭 ������ ����
	tempByte = byte & !andByte;
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

//�ش� �ε��� ���տ� ���ԵǾ��ִ��� Ȯ��
template <typename T>
bool Set<T>::find(const int &num) const{
	int tempByte = byte;
	tempByte = tempByte >> num;
	if ((tempByte & 1) == 1)
		return true;
	else return false;
}

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

template <typename T>
void Set<T>::PrintSet() {
	int tempByte = byte;
	for (int i = 0; i < sourceNum; i++) {
		if ((tempByte & 1) == 1)
			printf("%d\t", i);
		tempByte = tempByte >> 1;
	}
	putchar('\n');
} 