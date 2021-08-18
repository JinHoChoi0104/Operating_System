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

int get_Resource(); //�Է¹��� ���ҽ� ���� ��ȯ
int get_Process(int r_num); //�Է¹��� ���μ��� ���� ��ȯ
void show_Input(int r_num, int p_num);

void get_Process_Sequence(int p_num, int P_S[]);
bool Safe_State(int P_num, int R_num, int P_S[]);


void main()
{
	bool Safety; // Sequence�� safety check

	int R_num = get_Resource(); //�Է� ���� Resource�� ����
	int P_num = get_Process(R_num); //�Է� ���� Process�� ����
	int *P_S; //Process_Sequence
	P_S = (int*)malloc(sizeof(int)*P_num);

	show_Input(R_num, P_num);

	int input;
	while (1) {
		cout << "\n\nProcess Sequence�� �Է� �Ͻðڽ��ϱ�?\n1. ��  2. �ƴϿ�\n";
		cin >> input;
		if (input == 1) {
			get_Process_Sequence(P_num, P_S);
			Safety = Safe_State(P_num, R_num, P_S);
			cout << "\nProcess Sequence: ";
			for (int i = 0; i < P_num; i++) {
				cout << P_S[i] << " ";
			}
			if (Safety) 
				cout << "�� SAFE �ϴ�!!";
			else
				cout << "�� UNSAFE �ϴ�!!";
		}
		else
			break;
	}
	system("pause");
}


int get_Resource() {
	int num;//�Է¹��� ���ҽ� ����

	string type; //���ҽ� Ÿ�� �̸�
	int instance;//���ҽ��� Avialble

	cout << " �۾��� ���� �� Resource ������ �Է��Ͻÿ�: ";
	cin >> num;

	for (int i = 0; i < num; i++) {
		cout << "\n" << i + 1 << "��° Resource�� Ÿ�� �̸��� Ÿ�Ժ� instance�� �� ������ �Է��Ͻÿ�: ";
		cin >> type >> instance;
		cin.clear(); //�߰� �Է��� �Է¹��� �ʱ�ȭ
		cin.ignore(10000, '\n');
		RList.push_back(Resource(type, instance)); //0��°���� �Է�
	}

	return num;
}
int get_Process(int r_num) {
	int r = r_num; //�Է¹��� ���ҽ� ����
	int num; //�Է¹��� ���μ��� ����

	int *all; //Allocation
	all = (int*)malloc(sizeof(int)*r);
	int *max; //Max
	max = (int*)malloc(sizeof(int)*r);

	cout << "\n\n �۾��� ���� �� Proces ������ �Է��Ͻÿ�: ";
	cin >> num;
	cin.clear(); //�߰� �Է��� �Է¹��� �ʱ�ȭ
	cin.ignore(10000, '\n');

	for (int i = 0; i < num; i++) {
		cout << "\n" << i + 1 << "��° Process�� ���� �Է¹ްڽ��ϴ�.\n";

		for (int j = 0; j < r; j++)
			cout << RList[j].get_type() << " ";
		cout << "�� Allocation ���� ������� �Է��Ͻÿ�: ";
		for (int j = 0; j < r; j++)
			cin >> all[j];
		for (int j = 0; j < r; j++)
			RList[j].avail -= all[j]; //available���� allocation ��ŭ ���ֱ�
		cin.clear(); //�߰� �Է��� �Է¹��� �ʱ�ȭ
		cin.ignore(10000, '\n');

		for (int j = 0; j < r; j++)
			cout << RList[j].get_type() << " ";
		cout << "�� Max ���� ������� �Է��Ͻÿ�: ";
		for (int j = 0; j < r; j++)
			cin >> max[j];
		cin.clear(); //�߰� �Է��� �Է¹��� �ʱ�ȭ
		cin.ignore(10000, '\n');

		PList.push_back(Process(r, all, max)); //0��°���� ����
	}

	return num;
}
void show_Input(int r_num, int p_num) {
	cout << "\n\nShow Input!!";
	cout << "\nResource�� ������ �� Available:\n";
	for (int i = 0; i < r_num; i++) {
		cout << RList[i].get_type() << ": " << RList[i].avail << "     ";;
	}

	cout << "\n\nProcess�� Allocation, Max, Need�� ������ Ÿ�� ";
	for (int i = 0; i < r_num; i++)
		cout << RList[i].get_type() << " ";
	cout <<	" ������ ���";

	for (int i = 0; i < p_num; i++) {
		cout << "\n" << "Process" << i << ":  ";
		PList[i].show_P();
	}
	cout << "\n ";
}

void get_Process_Sequence(int p_num, int P_S[]) {
	int *check; //���μ������� �� �� �Է��ߴ��� üũ
	check = (int*)malloc(sizeof(int)*p_num);

	while (1) {
		int cnt = 0;
		for (int i = 0; i < p_num; i++)
			check[i] = 0; //�ʱ�ȭ

		cout << "Process Sequence�� �Է����ּ���\n<���� ���μ����� 0������ " << p_num-1<<"������ �ֽ��ϴ�.>\n";
		cout << "<�Է� ��) Sequence <P1, P0, P2 > �� ��� '1 0 2'�� �Է�, ������ �ʰ��ؼ� �Է��� ��� �ڿ� �Էµ� ���� ����>\n";
		for (int i = 0; i < p_num; i++)
			cin >> P_S[i];
		cin.clear(); //�߰� �Է��� �Է¹��� �ʱ�ȭ
		cin.ignore(10000, '\n');

		for (int i = 0; i < p_num; i++) { //�Է� �� �������� �������� üũ�Ѵ�
			if (P_S[i] > p_num - 1) //�������� �ʴ� ���μ��� �� ���
				cout << P_S[i] << "�� �������� �ʴ� ���μ��� �Դϴ�!\n";
			else { //�����ϴ� ���μ��� �� ���
				if (check[P_S[i]] == 0) {
					check[P_S[i]]++;
					cnt++; //�����ϴ� ��� ���μ����� ���Ͽ� �Է��ߴ��� üũ
				}
				else
					cout << P_S[i] << "�� �ߺ��ؼ� �Է� �� ���μ��� �Դϴ�!\n";
			}
		}

		if (cnt == p_num)
			break;
		else
			cout << "\n�ٽ� ";
	}
}
bool Safe_State(int P_num, int R_num, int P_S[]) {
	bool safety = true;
	int cnt;
	int *s_avail; //Sum of Available
	s_avail = (int*)malloc(sizeof(int)*R_num);
	for (int i = 0; i < R_num; i++) //�ʱ� ���ҽ��� Available ������ �ʱ�ȭ
		s_avail[i] = RList[i].avail;

	cout << "\nSafe Stae ���� Ȯ��!";

	for (int i = 0; i < P_num; i++)
	{
		cnt = 0;
		for (int j = 0; j < R_num; j++) {
			if (s_avail[j] >= PList[P_S[i]].get_need(j)) {
				s_avail[j] += PList[P_S[i]].get_allocation(j);
				cnt++;
			}
			else  //Need > Available *UNSAFE*
				cout << "\nProcess" << P_S[i] << "�� �����ϱ� ���� " << RList[j].get_type() << "�� " << PList[P_S[i]].get_need(j) - s_avail[j] << "�� �����մϴ�.";
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