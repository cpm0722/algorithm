#include<iostream>
#include<cstring>
#include<cmath>
#include<cstring>

#define BufferSize 256
#define StackSize 50
#define NumberSize 10

using namespace std;

//���� ����Ʈ Class
class List {
private:
	void* value;		//char�� int ���ÿ� �ٷ�� ���� void *�� ����
	bool isNumber;		//value�� int���̸� true, char���̸� false
public:
	List* prev;			//����� ���� ����Ʈ
	List* next;			//����� ���� ����Ʈ
	List();
	List(int, void *);	//int������ ����
	List(char, void *);	//char������ ����
	~List();
	void PutNumber(int);	
	bool GetIsNumber();	//isNumber return
	char GetOperator();	//value�� char�� return
	int GetNumber();	//value�� int�� return
	void Print();		//value ���
};

List::List(void) {
	value = new int;
	isNumber = true;
	prev = next = 0;
}

List::List(int num, void *pr = 0){
	value = new int;
	*(int *)value = num;
	isNumber = true;
	prev = (List *)pr;
	next = 0;
}

List::List(char c, void *pr = 0) {
	value = new char;
	*(char *)value = c;
	isNumber = false;
	prev = (List *)pr;
	next = 0;
}

List::~List() {
	delete(value);
}

void List::PutNumber(int num) {
	*(int *)value = num;
}

bool List::GetIsNumber() { return isNumber; }

void List::Print() {
	if (isNumber)
		cout << *(int*)value << "";
	else cout << *(char*)value << "";
	return;


}

char List::GetOperator() {
	return *(char*)value;
}

int List::GetNumber() {
	return *(int*)value;
}

//---------------------------------------------------------------------------

//List�� ����� Stack Class
class ListStack {
private:
	List* bottom;		//��� ���� bottom �� �Ҵ�, start pointer
	List* top;			//stack�� top ������
public:
	ListStack();
	ListStack(List*);
	~ListStack();
	List *Pop();		//stack�� Pop
	void Push(List *);	//stack�� Push
	List* GetTop();		//top return
	List* GetBottom();		//bottom return
	void Print();		//bottom���� top���� List�� Print�Լ� ȣ��
};

ListStack::ListStack() { bottom = top = 0; }

ListStack::ListStack(List* lp) { bottom = top = lp; }

ListStack::~ListStack() {
	List* now = top;
	if (now != 0) {
		while (now->prev != 0) {
			now = now->prev;
			delete(now->next);
		}
		delete now;
	}
}

//Stack���� Pop���ִ� �Լ�
List* ListStack::Pop() {
	List* tmp;
	if (top == 0)
		return 0;
	else if (top == bottom) {
		tmp = top;
		top = bottom = 0;
	}
	else {
		top = top->prev;
		tmp = top->next;
		top->next = 0;
		tmp->prev = 0;
	}
	return tmp;
}

//Stack�� Push���ִ� �Լ�
void ListStack::Push(List* lp) {
	if (top == 0) {
		bottom = top = lp;
		lp->prev = 0;
		return;
	}
	else {
		lp->prev = top;
		top->next = lp;
		top = top->next;
	}
	return;
}

List* ListStack::GetTop() { return top; }

List* ListStack::GetBottom() { return bottom; }

//ListStack ������ִ� �Լ�
void ListStack::Print() {
	List* now = bottom;
	while (now != 0) {
		now->Print();
		now = now->next;
		cout << ' ';
	}
	cout << endl;
}

//---------------------------------------------------------------------------

//StackLinkedList ����ϴ� �Լ�
void PrintList(List* start) {
	List* now = start;
	while (1) {
		now->Print();
		now->GetIsNumber();
		if (now->next == 0)
			break;
		now = now->next;
	}
}

//List�� �����ؼ� List ������ ���� �״�� return (��, prev = next = 0)
List* CopyList(List* a) {
	List* b;
	if (a->GetIsNumber())
		b = new List(a->GetNumber());
	else b = new List(a->GetOperator());
	return b;
}

//������ �켱���� �ľ��ϴ� �Լ�
bool OperatorSequence(List* L, List* R) {
	bool Pop = false;
	char l = L->GetOperator();
	char r = R->GetOperator();
	if (r == '*' || r == '/' || r == '%') {
		if (l == '*' || l == '/' || l == '%' || l == '^') {
			Pop = true;
			return Pop;
		}
		else {
			Pop = false;
			return Pop;
		}
	}
	else if (r == '+' || r == '-') {
		if (l == '+' || l == '-' || l == '*' || l == '/' || l == '%' || l == '^') {
			Pop = true;
			return Pop;
		}
		else {
			Pop = false;
			return Pop;
		}
	}
	else if (r == '^') {
		Pop = false;
	}
	return Pop;
}

//����ǥ��� ��ȯ�� �� ������ Stack ����ϴ� �Լ�
void ControlOperator(ListStack* operatorStack, List* now, ListStack* postfix) {
	if (operatorStack->GetTop() == 0) {				//Operator Stack�� ��� ���� ���
		operatorStack->Push(CopyList(now));			//�ٷ� Push
	}
	else {
		char op = now->GetOperator();
		if (op == '(') {							//'(' ó��
			operatorStack->Push(CopyList(now));		//������ Operator Stack�� Push
		}
		else if (op == ')') {						//')' ó��
			while (operatorStack->GetTop()->GetOperator() != '(')		//'('�� ���� ������
				postfix->Push(CopyList(operatorStack->Pop()));			//��� Pop�ؼ� Postfix�� ����
			operatorStack->Pop();										//'(' Pop�ؼ� ����
		}
		else {															// ��ȣ ���� ��Ÿ ������ ó��
			while ((operatorStack->GetTop()!=0)&&(OperatorSequence(operatorStack->GetTop(), now)))		//Operator Stack�� ��� �ְų� ������ �켱������ Operator Stack�� Top���� ���� ������
				postfix->Push(CopyList(operatorStack->Pop()));											//Pop�ؼ� Postfix ���� ����Ʈ�� ����
			operatorStack->Push(CopyList(now));															//���� ������ Operator Stack�� Push
		}
	}
	return;
}

//����ǥ������� ��ȯ���ִ� �Լ�
ListStack* MakePostfix(List* bufferStart) {
	List* now = bufferStart;									//Infix ���Ḯ��Ʈ
	ListStack* postfix = new ListStack;							//Postfix Stack ���Ḯ��Ʈ
	ListStack* operatorStack = new ListStack;					//Operator ���� Stack ���Ḯ��Ʈ
	while (now != 0) {
		if (now->GetIsNumber())			//������ ���
			postfix->Push(CopyList(now));						//posftfix Stack ���Ḯ��Ʈ�� �ٷ� �Է�
		else {
			ControlOperator(operatorStack, now, postfix);		//�������� ��� ������ Controler ȣ��
		}
		now = now->next;
		/*
		cout << "P\t";
		postfix->Print();
		cout << "S\t";
		operatorStack->Print();
		*/
	}
	while (operatorStack->GetTop() != 0){						//Infix Ž�� ���� �� Operator Stack�� �����ִ� ������ ��� Pop
		postfix->Push(CopyList(operatorStack->Pop()));
	}
	return postfix;
}

//---------------------------------------------------------------------------

//Buffer���� Operator �������� �Լ�
char PopOperator(char* buffer) {
	int len = strlen(buffer);
	char op = buffer[0];
	int i;
	for (i = 1; i < len; i++)
		buffer[i - 1] = buffer[i];
	buffer[i - 1] = '\0';
	return op;
}

//Buffer�� ù��° ���� Operator���� Ȯ���ϴ� �Լ�
bool IsOperator(char* buffer, List* now) {
	if (buffer[0] >= '0' && buffer[0] <= '9')
		return false;
	else if (buffer[0] == '-') {
		if (now->GetIsNumber() == false)
			return false;
		else return true;
	}
	else return true;
}

//buffer���� ���� �������� �Լ�
int PopNumber(char* buffer) {
	int i, number = 0, numberNow = 0, len = strlen(buffer);
	char numberArray[NumberSize] = { 0, };	//���� �ӽ� ������ �迭
	bool isMinus = false;
	if (buffer[0] == '-') {	//�������� �Ǻ�
		i = 1;	//������ ��� ù��° char �ǳʶ�
		isMinus = true;
	}
	else i = 0;

	for (; ; i++) {
		if (buffer[i] >= '0' && buffer[i] <= '9')	//buffer[i]�� �����̸�
			numberArray[numberNow++] = buffer[i];	//numberArray �迭�� ����
		else
			break;
	}

	int numLen = numberNow;		//������ �� - char���� ����ϱ� ���� ����
	if (isMinus)
		numLen++;

	for (i = numLen; i < len; i++)	//Pop�� ���� buffer���� ����
		buffer[i - numLen] = buffer[i];
	for (i = len - numLen; i < len; i++)
		buffer[i] = '\0';

	for (i = 0; i < numberNow; i++)		//numberNow ����� number�� ����
		number += (numberArray[i] - '0') * (int)pow(10, numberNow - 1 - i);

	if (isMinus)	//���� ó��
		number *= -1;

	return number;
}

//Buffer�� Linked List�� ��ȯ�ϴ� �Լ�
List* SeperateToList(char* buffer) {
	List* start;
	if ((buffer[0] >= '0' && buffer[0] <= '9') || (buffer[0] == '-'))
		start = new List(PopNumber(buffer));
	else start = new List(PopOperator(buffer));
	List* now = start;
	while (buffer[0] != '\0') {
		if (IsOperator(buffer, now)) {
			now->next = new List(PopOperator(buffer), now);
			now = now->next;
		}
		else {
			now->next = new List(PopNumber(buffer), now);
			now = now->next;
		}
	}
	return start;
}

//Buffer�� ���� �� �� ���� �Լ�
void DeleteEmptyCharacter(char* buffer) {
	int len = strlen(buffer);
	int i;
	for (i = 0; i < len; i++) {
		if (buffer[i] == ' ' || buffer[i] == '\t') {	//���� �� �� ����
			int j;
			for (j = i; j < len; j++)	//���� �� �� ���� ���ڿ� �� ĭ�� ����
				buffer[j] = buffer[j + 1];
			len--;
			i--;
		}
	}
	buffer[len] = '\0';
}

//�߸� �Էµ� ���� ã�� �Լ�
bool FindError(char* buffer) {
	int len = strlen(buffer);
	int i;
	for (i = 0; i < len; i++) {
		if (!((buffer[i] >= '0' && buffer[i] <= '9') || (buffer[i] == '(' || \
			buffer[i] == ')' || buffer[i] == '^' || buffer[i] == '+' || \
			buffer[i] == '-' || buffer[i] == '*' || buffer[i] == '/' || buffer[i] == '%')))
			return true;
	}
	return false;
}

//ListStack ���� �Լ�
ListStack* CopyListStack(ListStack* a) {
	ListStack* b = new ListStack;
	List* start = a->GetTop();
	List* now = start;
	while (now != 0) {
		b->Push(CopyList(now));
		now = now->prev;
	}
	return b;
}

int Calculate(int l, int r, char op) {
	int result = 0;
	switch (op) {
	case '+':
		return l + r;
		break;
	case '-':
		return l - r;
		break;
	case '*':
		return l * r;
		break;
	case '/':
		return l / r;
		break;
	case '%':
		return l % r;
		break;
	case '^':
		return (int)pow(l, r);
		break;
	}
}

//���� ��� �Լ�
 bool CalculateAll(ListStack* postfix, int* result) {
	bool error = false;
	int numCount = 0, opCount = 0;
	ListStack numStack;
	ListStack opStack;
	while (postfix->GetTop() != 0) {
		if (postfix->GetTop()->GetIsNumber()) {
			numStack.Push(CopyList(postfix->Pop()));
			numCount++;
		}
		else {
			opStack.Push(CopyList(postfix->Pop()));
			opCount++;
		}
		while ((numCount >= 2) && (opStack.GetTop() != 0)) {
			char op = opStack.GetTop()->GetOperator();
			int tmpNum = 0;
			tmpNum = Calculate(numStack.Pop()->GetNumber(), 
				\numStack.Pop()->GetNumber(), opStack.Pop()->GetOperator());
			opCount--;
			numCount -= 2;
			List *tmpList = new List(tmpNum);
			numStack.Push(tmpList);
			numCount++;
		}
	}
	if (!((numCount == 1) && (opCount == 0))) error = true;
	else {
		error = false;
		*result = numStack.Pop()->GetNumber();
	}
	return error;
}

//���� ���� �Լ�
bool StackCalculator(char* buffer) {
	int len = strlen(buffer);
	DeleteEmptyCharacter(buffer);	//���� �� tab ����� �Լ�
	if (FindError(buffer))
		return false;
	List *bufferStart = SeperateToList(buffer);	//Buffer�� Linked List�� ��ȯ
	ListStack* postfixStack = MakePostfix(bufferStart);	//���� ǥ�� Stack List
	ListStack* CalculateStack = CopyListStack(postfixStack);
	int result = 0;
	if (CalculateAll(CalculateStack, &result)) {
		return false;
	}
	postfixStack->Print();
	cout << result << endl;
	return true;
}

int main(void) {
	while (1) {
		cout << "�Է�: ";
		char *buffer = new char[BufferSize];	//�Է� ���� �Ҵ�
		cin.getline(buffer, BufferSize);

		if (buffer[0] == 'q' && buffer[1] == '\0') {	//"q" �Է� �� ����
			delete(buffer);
			return 0;
		}

		if (!StackCalculator(buffer)) {		//�߸� �Էµ� ���� ���� ��� ERROR ���
			cout << "ERROR" << endl;
			continue;
		}

		delete(buffer);	//�Է� ���� ����
		
		
	}
	return 0;
}