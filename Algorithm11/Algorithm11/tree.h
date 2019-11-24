#include<stdio.h>
#include<iostream>

using namespace std;

template <typename T>
class BinaryTree {
private:
	T *value;
	BinaryTree *parent;
	BinaryTree *left;
	BinaryTree *right;
	int height;
public:
	BinaryTree();
	BinaryTree(T *, BinaryTree *, int);
	~BinaryTree() {}

	T *getValue() { return this->value; }
	void setValue(T *value) { this->value = value; }
	BinaryTree *getParent() const{ return this->parent; }
	void setParent(BinaryTree * parent) { this->parent = parent; }
	BinaryTree *getLeft() const{ return this->left; }
	void setLeft(BinaryTree * left) { this->left = left; }
	BinaryTree *getRight() const{ return this->right; }
	void setRight(BinaryTree * right) { this->right = right; }
	int getHeight() const{ return this->height; }
	void setHeight(int height) { this->height = height; }

	bool isRoot();
	void print();
};

//Default 생성자
template <typename T>
BinaryTree<T>::BinaryTree() {
	this->value = NULL;
	this->parent = this->left = this->right = NULL;
}

//생성자
template <typename T>
BinaryTree<T>::BinaryTree(T *value, BinaryTree *parent, int height) {
	this->value = value;
	this->parent = parent;
	this->left = this->right = NULL;
	this->height = height;
}

//root인지 판별
template <typename T>
bool BinaryTree<T>::isRoot() {
	if (this->parent == NULL)
		return true;
	return false;
}

//출력 함수, 재귀 호출 이용해 자식 노드 모두 출력
template <typename T>
void BinaryTree<T>::print() {
	cout << *(this->value);
	putchar('\n');
	if(this->left != NULL)
		this->left->print();
	if(this->right != NULL)
		this->right->print();
	return;
}
