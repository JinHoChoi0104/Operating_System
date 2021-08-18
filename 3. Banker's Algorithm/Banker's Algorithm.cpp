#include <iostream>
#include <string>
#include <windows.h>
#include <vector>

using namespace std;

class Resource {
private:
	string type;
	int instance;
public:
	int avail; //Available
public:
	Resource(string name, int num)
	{
		type = name;
		instance = num;
		avail = instance;
	}
	Resource() { }

	string get_type() {
		return type;
	}
	int get_instance() {
		return instance;
	}
}; 

class Process {
private:
	int R_num;

	int *allocation;
	int *max;
	int *need;
public:
	Process(int size, int all_i[], int max_i[]) {
		R_num = size;
		allocation = (int*)malloc(sizeof(int)*size);
		max = (int*)malloc(sizeof(int)*size);
		need = (int*)malloc(sizeof(int)*size);
		for (int i = 0; i < size; i++) {
			allocation[i] = all_i[i];
			max[i] = max_i[i];
			need[i] = max[i] - allocation[i];
		}
	}
	Process() {}

	void show_P()
	{
		for (int i = 0; i < R_num; i++)  //Allocation
			cout << allocation[i] << " ";
		cout << "     ";
		for (int i = 0; i < R_num; i++)  //Max
			cout << max[i] << " ";
		cout << "     ";
		for (int i = 0; i < R_num; i++)  //Need
			cout << need[i] << " ";
	}

	int get_allocation(int num) {
		return allocation[num];
	}
	int get_need(int num) {
		return need[num];
	}
};

std::vector<Resource> RList;
std::vector<Process> PList;

int get_Resource(); //입력받은 리소스 개수 반환
int get_Process(int r_num); //입력받은 프로세스 개수 반환
void show_Input(int r_num, int p_num);

void get_Process_Sequence(int p_num, int P_S[]);
bool Safe_State(int P_num, int R_num, int P_S[]);


void main()
{
	bool Safety; // Sequence의 safety check

	int R_num = get_Resource(); //입력 받은 Resource의 개수
	int P_num = get_Process(R_num); //입력 받은 Process의 개수
	int *P_S; //Process_Sequence
	P_S = (int*)malloc(sizeof(int)*P_num);

	show_Input(R_num, P_num);

	int input;
	while (1) {
		cout << "\n\nProcess Sequence를 입력 하시겠습니까?\n1. 예  2. 아니오\n";
		cin >> input;
		if (input == 1) {
			get_Process_Sequence(P_num, P_S);
			Safety = Safe_State(P_num, R_num, P_S);
			cout << "\nProcess Sequence: ";
			for (int i = 0; i < P_num; i++) {
				cout << P_S[i] << " ";
			}
			if (Safety) 
				cout << "는 SAFE 하다!!";
			else
				cout << "는 UNSAFE 하다!!";
		}
		else
			break;
	}
	system("pause");
}


int get_Resource() {
	int num;//입력받을 리소스 개수

	string type; //리소스 타입 이름
	int instance;//리소스의 Avialble

	cout << " 작업을 수행 할 Resource 개수를 입력하시오: ";
	cin >> num;

	for (int i = 0; i < num; i++) {
		cout << "\n" << i + 1 << "번째 Resource의 타입 이름과 타입별 instance의 총 개수를 입력하시오: ";
		cin >> type >> instance;
		cin.clear(); //추가 입력한 입력버퍼 초기화
		cin.ignore(10000, '\n');
		RList.push_back(Resource(type, instance)); //0번째부터 입력
	}

	return num;
}
int get_Process(int r_num) {
	int r = r_num; //입력받은 리소스 개수
	int num; //입력받을 프로세스 개수

	int *all; //Allocation
	all = (int*)malloc(sizeof(int)*r);
	int *max; //Max
	max = (int*)malloc(sizeof(int)*r);

	cout << "\n\n 작업을 수행 할 Proces 개수를 입력하시오: ";
	cin >> num;
	cin.clear(); //추가 입력한 입력버퍼 초기화
	cin.ignore(10000, '\n');

	for (int i = 0; i < num; i++) {
		cout << "\n" << i + 1 << "번째 Process의 값을 입력받겠습니다.\n";

		for (int j = 0; j < r; j++)
			cout << RList[j].get_type() << " ";
		cout << "의 Allocation 값을 순서대로 입력하시오: ";
		for (int j = 0; j < r; j++)
			cin >> all[j];
		for (int j = 0; j < r; j++)
			RList[j].avail -= all[j]; //available에서 allocation 만큼 빼주기
		cin.clear(); //추가 입력한 입력버퍼 초기화
		cin.ignore(10000, '\n');

		for (int j = 0; j < r; j++)
			cout << RList[j].get_type() << " ";
		cout << "의 Max 값을 순서대로 입력하시오: ";
		for (int j = 0; j < r; j++)
			cin >> max[j];
		cin.clear(); //추가 입력한 입력버퍼 초기화
		cin.ignore(10000, '\n');

		PList.push_back(Process(r, all, max)); //0번째부터 삽입
	}

	return num;
}
void show_Input(int r_num, int p_num) {
	cout << "\n\nShow Input!!";
	cout << "\nResource의 데이터 별 Available:\n";
	for (int i = 0; i < r_num; i++) {
		cout << RList[i].get_type() << ": " << RList[i].avail << "     ";;
	}

	cout << "\n\nProcess의 Allocation, Max, Need를 데이터 타입 ";
	for (int i = 0; i < r_num; i++)
		cout << RList[i].get_type() << " ";
	cout <<	" 순으로 출력";

	for (int i = 0; i < p_num; i++) {
		cout << "\n" << "Process" << i << ":  ";
		PList[i].show_P();
	}
	cout << "\n ";
}

void get_Process_Sequence(int p_num, int P_S[]) {
	int *check; //프로세스별로 몇 번 입력했는지 체크
	check = (int*)malloc(sizeof(int)*p_num);

	while (1) {
		int cnt = 0;
		for (int i = 0; i < p_num; i++)
			check[i] = 0; //초기화

		cout << "Process Sequence를 입력해주세요\n<현재 프로세스는 0번에서 " << p_num-1<<"번까지 있습니다.>\n";
		cout << "<입력 예) Sequence <P1, P0, P2 > 일 경우 '1 0 2'로 입력, 개수를 초과해서 입력할 경우 뒤에 입력된 수는 무시>\n";
		for (int i = 0; i < p_num; i++)
			cin >> P_S[i];
		cin.clear(); //추가 입력한 입력버퍼 초기화
		cin.ignore(10000, '\n');

		for (int i = 0; i < p_num; i++) { //입력 된 시퀀스가 정상인지 체크한다
			if (P_S[i] > p_num - 1) //존재하지 않는 프로세스 일 경우
				cout << P_S[i] << "는 존재하지 않는 프로세스 입니다!\n";
			else { //존재하는 프로세스 일 경우
				if (check[P_S[i]] == 0) {
					check[P_S[i]]++;
					cnt++; //존재하는 모든 프로세스에 대하여 입력했는지 체크
				}
				else
					cout << P_S[i] << "는 중복해서 입력 된 프로세스 입니다!\n";
			}
		}

		if (cnt == p_num)
			break;
		else
			cout << "\n다시 ";
	}
}
bool Safe_State(int P_num, int R_num, int P_S[]) {
	bool safety = true;
	int cnt;
	int *s_avail; //Sum of Available
	s_avail = (int*)malloc(sizeof(int)*R_num);
	for (int i = 0; i < R_num; i++) //초기 리소스의 Available 값으로 초기화
		s_avail[i] = RList[i].avail;

	cout << "\nSafe Stae 여부 확인!";

	for (int i = 0; i < P_num; i++)
	{
		cnt = 0;
		for (int j = 0; j < R_num; j++) {
			if (s_avail[j] >= PList[P_S[i]].get_need(j)) {
				s_avail[j] += PList[P_S[i]].get_allocation(j);
				cnt++;
			}
			else  //Need > Available *UNSAFE*
				cout << "\nProcess" << P_S[i] << "를 수행하기 위한 " << RList[j].get_type() << "가 " << PList[P_S[i]].get_need(j) - s_avail[j] << "개 부족합니다.";
		}

		if (cnt == R_num) {
			cout << "\nProcess" << P_S[i] << " release resource, Available: ";
			for (int j = 0; j < R_num; j++)
				cout << s_avail[j] << " ";
		}
		else {
			safety = false;
			break;
		}
	}
	return safety;
}