#include<iostream>
#include<cstring>
#include<cmath>
#include<cstring>

#define BufferSize 256
#define StackSize 50
#define NumberSize 10

using namespace std;

//연결 리스트 Class
class List {
private:
	void* value;		//char와 int 동시에 다루기 위해 void *로 선언
	bool isNumber;		//value가 int형이면 true, char형이면 false
public:
	List* prev;			//양방향 연결 리스트
	List* next;			//양방향 연결 리스트
	List();
	List(int, void *);	//int형으로 생성
	List(char, void *);	//char형으로 생성
	~List();
	void PutNumber(int);	
	bool GetIsNumber();	//isNumber return
	char GetOperator();	//value를 char로 return
	int GetNumber();	//value를 int로 return
	void Print();		//value 출력
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

//List를 사용한 Stack Class
class ListStack {
private:
	List* bottom;		//출력 위해 bottom 값 할당, start pointer
	List* top;			//stack의 top 포인터
public:
	ListStack();
	ListStack(List*);
	~ListStack();
	List *Pop();		//stack의 Pop
	void Push(List *);	//stack의 Push
	List* GetTop();		//top return
	List* GetBottom();		//bottom return
	void Print();		//bottom부터 top까지 List의 Print함수 호출
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

//Stack에서 Pop해주는 함수
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

//Stack에 Push해주는 함수
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

//ListStack 출력해주는 함수
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

//StackLinkedList 출력하는 함수
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

//List를 복사해서 List 내부의 값을 그대로 return (단, prev = next = 0)
List* CopyList(List* a) {
	List* b;
	if (a->GetIsNumber())
		b = new List(a->GetNumber());
	else b = new List(a->GetOperator());
	return b;
}

//연산자 우선순위 파악하는 함수
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

//후위표기식 변환할 때 연산자 Stack 사용하는 함수
void ControlOperator(ListStack* operatorStack, List* now, ListStack* postfix) {
	if (operatorStack->GetTop() == 0) {				//Operator Stack이 비어 있을 경우
		operatorStack->Push(CopyList(now));			//바로 Push
	}
	else {
		char op = now->GetOperator();
		if (op == '(') {							//'(' 처리
			operatorStack->Push(CopyList(now));		//무조건 Operator Stack에 Push
		}
		else if (op == ')') {						//')' 처리
			while (operatorStack->GetTop()->GetOperator() != '(')		//'('이 나올 때까지
				postfix->Push(CopyList(operatorStack->Pop()));			//모두 Pop해서 Postfix에 삽입
			operatorStack->Pop();										//'(' Pop해서 버림
		}
		else {															// 괄호 제외 기타 연산자 처리
			while ((operatorStack->GetTop()!=0)&&(OperatorSequence(operatorStack->GetTop(), now)))		//Operator Stack이 비어 있거나 연산자 우선순위가 Operator Stack의 Top보다 높을 때까지
				postfix->Push(CopyList(operatorStack->Pop()));											//Pop해서 Postfix 연결 리스트에 삽입
			operatorStack->Push(CopyList(now));															//현재 연산자 Operator Stack에 Push
		}
	}
	return;
}

//후위표기식으로 변환해주는 함수
ListStack* MakePostfix(List* bufferStart) {
	List* now = bufferStart;									//Infix 연결리스트
	ListStack* postfix = new ListStack;							//Postfix Stack 연결리스트
	ListStack* operatorStack = new ListStack;					//Operator 전용 Stack 연결리스트
	while (now != 0) {
		if (now->GetIsNumber())			//숫자일 경우
			postfix->Push(CopyList(now));						//posftfix Stack 연결리스트에 바로 입력
		else {
			ControlOperator(operatorStack, now, postfix);		//연산자일 경우 연산자 Controler 호출
		}
		now = now->next;
		/*
		cout << "P\t";
		postfix->Print();
		cout << "S\t";
		operatorStack->Print();
		*/
	}
	while (operatorStack->GetTop() != 0){						//Infix 탐색 끝난 후 Operator Stack에 남아있는 연산자 모두 Pop
		postfix->Push(CopyList(operatorStack->Pop()));
	}
	return postfix;
}

//---------------------------------------------------------------------------

//Buffer에서 Operator 빼내오는 함수
char PopOperator(char* buffer) {
	int len = strlen(buffer);
	char op = buffer[0];
	int i;
	for (i = 1; i < len; i++)
		buffer[i - 1] = buffer[i];
	buffer[i - 1] = '\0';
	return op;
}

//Buffer의 첫번째 값이 Operator인지 확인하는 함수
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

//buffer에서 숫자 빼내오는 함수
int PopNumber(char* buffer) {
	int i, number = 0, numberNow = 0, len = strlen(buffer);
	char numberArray[NumberSize] = { 0, };	//숫자 임시 저장할 배열
	bool isMinus = false;
	if (buffer[0] == '-') {	//음수인지 판별
		i = 1;	//음수일 경우 첫번째 char 건너뜀
		isMinus = true;
	}
	else i = 0;

	for (; ; i++) {
		if (buffer[i] >= '0' && buffer[i] <= '9')	//buffer[i]가 숫자이면
			numberArray[numberNow++] = buffer[i];	//numberArray 배열에 저장
		else
			break;
	}

	int numLen = numberNow;		//음수일 때 - char까지 고려하기 위한 변수
	if (isMinus)
		numLen++;

	for (i = numLen; i < len; i++)	//Pop한 숫자 buffer에서 제거
		buffer[i - numLen] = buffer[i];
	for (i = len - numLen; i < len; i++)
		buffer[i] = '\0';

	for (i = 0; i < numberNow; i++)		//numberNow 계산해 number에 넣음
		number += (numberArray[i] - '0') * (int)pow(10, numberNow - 1 - i);

	if (isMinus)	//음수 처리
		number *= -1;

	return number;
}

//Buffer를 Linked List로 변환하는 함수
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

//Buffer의 공백 및 탭 제거 함수
void DeleteEmptyCharacter(char* buffer) {
	int len = strlen(buffer);
	int i;
	for (i = 0; i < len; i++) {
		if (buffer[i] == ' ' || buffer[i] == '\t') {	//공백 및 탭 제거
			int j;
			for (j = i; j < len; j++)	//공백 및 탭 있을 문자열 한 칸씩 당기기
				buffer[j] = buffer[j + 1];
			len--;
			i--;
		}
	}
	buffer[len] = '\0';
}

//잘못 입력된 문자 찾는 함수
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

//ListStack 복사 함수
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

//실제 계산 함수
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

//스택 계산기 함수
bool StackCalculator(char* buffer) {
	int len = strlen(buffer);
	DeleteEmptyCharacter(buffer);	//공백 및 tab 지우는 함수
	if (FindError(buffer))
		return false;
	List *bufferStart = SeperateToList(buffer);	//Buffer를 Linked List로 변환
	ListStack* postfixStack = MakePostfix(bufferStart);	//후위 표시 Stack List
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
		cout << "입력: ";
		char *buffer = new char[BufferSize];	//입력 버퍼 할당
		cin.getline(buffer, BufferSize);

		if (buffer[0] == 'q' && buffer[1] == '\0') {	//"q" 입력 시 종료
			delete(buffer);
			return 0;
		}

		if (!StackCalculator(buffer)) {		//잘못 입력된 문자 있을 경우 ERROR 출력
			cout << "ERROR" << endl;
			continue;
		}

		delete(buffer);	//입력 버퍼 해제
		
		
	}
	return 0;
}