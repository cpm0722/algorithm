#include<stdio.h>
#include<iostream>

using namespace std;

class KnapsackNode {
private:
	int profit;
	int weight;
	int bound;
	Set<int> set;
	bool isActivate;
public:
	KnapsackNode();
	KnapsackNode(int, int , int, Set<int>);
	~KnapsackNode() {}

	int getProfit() const{ return this->profit; }
	void setProfit(int profit) { this->profit = profit; }
	int getWeight() const{ return this->weight; }
	void setWeight(int weight) { this->weight = weight; }
	int getBound() const{ return this->bound; }
	void setBound(int bound) { this->bound = bound; }
	Set<int> getSet() const { return this->set; }
	void setSet(Set<int> set) { this->set = set; }
	bool getIsActivate() const { return isActivate; }
	void setIsActivate(bool isActivate) { this->isActivate = isActivate; }

	void print()const;
	friend ostream &operator <<(ostream &c, const KnapsackNode);
};

//Default 생성자
KnapsackNode::KnapsackNode() {
	this->profit = this->weight = this->bound = -1;
	set = NULL;
	this->isActivate = false;
}

//생성자
KnapsackNode::KnapsackNode(int profit, int weight, int bound, Set<int> set){
	this->profit = profit;
	this->weight = weight;
	this->bound = bound;
	this->set = set;
	this->isActivate = true;
}

//출력 함수
void KnapsackNode::print() const{
	printf("Profit: %2d\nWeight: %2d\nBound : %2d", profit, weight, bound);
	return;
}

//출력 연산자 Overloading
ostream &operator <<(ostream &c, const KnapsackNode node) {
	node.print();
	return c;
}
