#include <stdio.h>
#include <windows.h>

class Process {
public: //������
	int at; //Arrival Time �����ð�
	int bt; //Burst Time ó���ð�
	int pr; //Priority �켱����

	int pt = 0; //Processing Time
	int wt = 0; //Wait Time ���ð�
	int ct = 0; //Complete Time �۾��� ������ ���� �ð� 
	int rt = 0; //���� �󸶳� ���Ҵ���
	int tag = 0; //�������� �Է� ����

public:
	Process(int _at, int _bt, int _pr) { //������
		at = _at;
		bt = _bt;
		pr = _pr;
	}
	Process() {
		at = 0;
		bt = 0;
		pr = 0;
	}
};


void Get_Process(Process P[], int *num, int *q);
void Show_Result(Process P[], int num, float Swt, float Stt);

void FCFS(Process P[], int *num);
void SJF(Process P[], int *num);
void Priority_Scheduling(Process P[], int *num);
void RR(Process P[], int *num, int q);

void main()
{
	Process P_L[10]; //Process ����Ʈ
	int num = 0; //Process ����
	int Time_Quantum; //Time Quantum

	Get_Process(P_L, &num, &Time_Quantum);

	FCFS(P_L, &num);
	SJF(P_L, &num);
	Priority_Scheduling(P_L, &num);
	RR(P_L, &num, Time_Quantum);

	system("pause");
}

void Get_Process(Process P[], int *num, int *q)
{
	Process P_s;//�ӽ� Process
	int i = 0;

	printf("\n �۾��� ���� �� Proces ������ �Է��Ͻÿ�: ");
	scanf_s("%d", num);
	while (*num > 10) {
		printf("\n* ERROR * Process�� �ִ� 10������ �Է� �� �� �ֽ��ϴ�!\n�ٽ� �Է����ּ���: ");
		scanf_s("%d", num);
	}
	for (i = 0; i < *num; i++) {
		while (1) {
			printf("\n%d��° Process�� Arrival Time, Burst Time, Priority�� �Է��Ͻÿ�:", i + 1);
			scanf_s("%d %d %d", &P[i].at, &P[i].bt, &P[i].pr);
			if (P[i].bt == 0)
				printf("\n* ERROR * Burst Time�� 0�� �� �� �����ϴ�! �ٽ� �Է����ּ���.");
			else {
				P[i].tag = i;
				break;
			}
		}
	}
	printf("\n Round Robin�� ��� �� Time Quantum�� �Է��Ͻÿ�: ");
	scanf_s("%d", q);

	for (i = *num - 1; i > 0; i--) { //���� ������ ���� ���� ������� �ٲ۴�
		for (int j = 0; j < i; j++)
		{
			if (P[j].at > P[j + 1].at) { //j�� �ʰ� ���� �ڸ� �ٲ۴� 
				P_s = P[j];
				P[j] = P[j + 1];
				P[j + 1] = P_s;
			}
		}
	}
}

void Show_Result(Process P[], int num, float Swt, float Stt) { //����� �Է� ������
	float A_wt = Swt; //Average Wait Time
	float A_tt = Stt; //Average Turnaround Time

	for (int j = 0; j < num; j++) {
		for (int i = 0; i < num; i++) {
			if (j == P[i].tag)
				printf("\n %d��° Process: Processing = %2d,  Waiting = %2d,  Complete = %2d", j + 1, P[i].pt, P[i].wt, P[i].ct);
		}
	}

	A_wt /= num;
	A_tt /= num;
	printf("\n Average Wait Time = %.4f, Average Turnaround Time = %.4f\n", A_wt, A_tt);
};

void FCFS(Process P[], int *num) {
	int i = 0; //�� �� ���μ���
	int s_time = 0; //�� ������ �ð�

	int cnt = 0; //ó���� ���μ����� ��

	float S_wt = 0; //Average Wait Time;
	float S_tt = 0; //Average Turnaround Time;

	for (int k = 0; k < *num; k++) {//�ʱ�ȭ
		P[k].rt = -1; //rt�� ���μ��� ������� ���
		P[k].pt = 0;
	}

	printf("\n\n - FCFS - ");
	
	for (int time = 0; cnt < *num; time++) { //�ð��� �귯 ����

		P[i].at <= time && P[i].rt<0 ? P[i].rt = P[i].bt : NULL; //������ ������ rt�� ���� �־��ش�

		if (P[i].rt > 0) { //������������ ���Ѵ�
			if (P[i].rt == P[i].bt) { //���μ����� �۾��� ����
				if (s_time != time) // ���������� �۾��� �����ð��� ������ �ð��� �ٸ���
					printf("\n%2d ~ %2d ���� Process ���� ��Ȳ ����", s_time, time);
				s_time = time; //���μ����� ���� �ð� ����
			}
			P[i].rt--;
			P[i].pt++;
		}

		if (P[i].rt == 0) { //���μ����� �۾��� ���ƴ�
			P[i].ct = time + 1; //���μ����� ���� �ð�
			P[i].wt = time + 1 - P[i].bt - P[i].at; //���μ����� ��ٸ� �ð�

			S_tt += P[i].ct - P[i].at; //Complete Time - Arrival Time
			S_wt += P[i].wt;

			printf("\n%2d ~ %2d ���� Process %d ����", s_time, time+1, P[i].tag + 1);
			s_time = time + 1;

			cnt++; //���μ����� �ϳ� ����
			i++; //�������μ��� ����Ų�� (������ ������ ���� �������Ƿ�)
		}
	}

	Show_Result(P, cnt, S_wt, S_tt);
}

void SJF(Process P[], int *num) { //Shortest Job First
	int i = 0; //�� �� ���μ���
	int index = 1000; //�� �ϴ� ���μ���
	int s_time = 0; //�� ������ �ð�

	int cnt = 0; //ó���� ���μ���
	int pr = 0; //Preemptive�� üũ�Ѵ�

	float S_wt = 0; //Sum of Wait Time;
	float S_tt = 0; //Sum of Turnaround Time;

	for (int k = 0; k < *num; k++) { //�ʱ�ȭ
		P[k].rt = -1; //rt�� ���μ��� ������� ���
		P[k].pt = 0;
	}

	printf("\n\n - SJF - *Preemptive ����*");

	int time;
	for (time = 0; cnt < *num; time++) { //SJF �˰��� ����
		for (int k = 0; k < *num; k++) {
			if (P[k].at == time) { //���μ��� ����
				P[k].rt = P[k].bt; 
				pr = 0; //Preemptive �߻��� ���� �ִ�.
			}
		}

		if (pr == 0) { //�۾��ϴ� ���μ����� �����ų� ���� ������ ���μ����� �ְų�
			if (cnt == *num - 1) //�� ���� ���μ����� �ϳ�
				for (int k = 0; k < *num; k++)
					P[k].rt > 0 ? i = k : NULL;
			else { //�� ���� ���μ����� �� �� �̻�
				i = 0;
				for (int k = 1; k < *num; k++) {//���������� checek
					if (P[i].rt > 0 && P[k].rt > 0) //�ΰ� �� �� ����
						P[i].rt > P[k].rt ? i = k : NULL;
					else if (P[i].rt <= 0 && P[k].rt <= 0);
					else
						P[k].rt > 0 ? i = k : NULL;
				}//������ �ִ� ���μ����� �� SJ ���� ó���Ѵ�
			}
		}

		if (P[i].rt >= 0) { //������������ ���Ѵ�
			if (index != i || P[i].rt == 0) { // ���� �߻� or �۾��� ����
				if (index == 1000 && time > 0)
					printf("\n%2d ~ %2d ���� Process ���� ��Ȳ ����", s_time, time);
				else if ( time > 0)
					printf("\n%2d ~ %2d ���� Process %d ����", s_time, time, P[index].tag + 1);
				index = i;
				s_time = time;
			}
			pr++;
			P[i].rt -= 1;
			P[i].rt >= 0 ? P[i].pt++ : index = 1000; //���μ���Ÿ��
		}


		if (P[i].rt == 0) { //���μ����� �۾��� ��ħ!
			P[i].ct = time + 1; //���� ��������
			P[i].wt = time + 1 - P[i].bt - P[i].at; //�󸶳� ��ٷȴ���
			S_tt += P[i].ct - P[i].at;//average turnaround time = complete time - arrival time
			S_wt += P[i].wt; //��� ��ٸ� �ð�
	
			pr = 0;
			cnt++;//�� �ϳ� �� �ߴ�
		}
	}
	printf("\n%2d ~ %2d ���� Process %d ����", s_time, time, P[index].tag + 1);

	Show_Result(P, cnt, S_wt, S_tt);
}

void Priority_Scheduling(Process P[], int *num) {
	int i = 0; //�� �� ���μ���
	int index = 1000; //�� �ϴ� ���μ���
	int s_time = 0; //�� ������ �ð�

	int cnt = 0; //ó���� ���μ���
	int pr = 0; //Preemptive�� üũ�Ѵ�

	float S_wt = 0; //Sum of Wait Time;
	float S_tt = 0; //Sum of Turnaround Time;

	for (int k = 0; k < *num; k++) { //�ʱ�ȭ
		P[k].rt = -1; //rt�� ���μ��� ������� ���
		P[k].pt = 0;
	}

	printf("\n\n - Priority Scheduling - *Preemptive ����*");

	int time;
	for (time = 0; cnt < *num; time++) { // Prirority Sceduling ����
		for (int k = 0; k < *num; k++) {
			if (P[k].at == time) {
				P[k].rt = P[k].bt; //���μ��� ����
				pr = 0;
			}
		}

		if (pr == 0) { //�۾��ϴ� ���μ����� �����ų� ���� ������ ���μ����� �ְų�
			if (cnt == *num - 1) //�� ���� ���μ����� �ϳ�
				for (int k = 0; k < *num; k++)
					P[k].rt > 0 ? i = k : NULL;
			else { //�� ���� ���μ����� �� �� �̻�
				i = 0;
				for (int k = 1; k < *num; k++) {//���������� checek
					if (P[i].rt > 0 && P[k].rt > 0) //�ΰ� �� �� ����
						P[i].pr > P[k].pr ? i = k : NULL;
					else if (P[i].rt <= 0 && P[k].rt <= 0); //�ΰ� �� �����ų� �� ���԰ų�
					else
						P[k].rt > 0 ? i = k : NULL;
				} //������ �ִ� ���μ����� �� Priority�� ���� ���μ������� ó���Ѵ�.
			}
		}

		if (P[i].rt >= 0) { //������������ ���Ѵ�
			if (index != i || P[i].rt == 0) { // ���� �߻� or �۾��� ����
				if (index == 1000 && time > 0)
					printf("\n%2d ~ %2d ���� Process ���� ��Ȳ ����", s_time, time);
				else if (time > 0)
					printf("\n%2d ~ %2d ���� Process %d ����", s_time, time, P[index].tag + 1);
				index = i;
				s_time = time;
			}
			pr++;
			P[i].rt -= 1;
			P[i].rt >= 0 ? P[i].pt++ : index = 1000; //���μ���Ÿ��
		}

		if (P[i].rt == 0) { //���μ����� �۾��� ��ħ!
			P[i].ct = time + 1; //���� ��������
			P[i].wt = time + 1 - P[i].bt - P[i].at; //�󸶳� ��ٷȴ���

			S_tt += P[i].ct - P[i].at;//average turnaround time = complete time - arrival time
			S_wt += P[i].wt; //��� ��ٸ� �ð�

			pr = 0;
			cnt++;//�� �ϳ� �� �ߴ�
		}
	}

	printf("\n%2d ~ %2d ���� Process %d ����", s_time, time, P[index].tag + 1);

	Show_Result(P, cnt, S_wt, S_tt);
}

void RR(Process P[], int *num, int q) { //Round Robin
	int i = 0;//�� �� ���μ���
	int index = 1000; //�� �ϴ� ���μ���
	int s_time = 0; //�� ������ �ð�

	int cnt = 0;//ó���� ���μ���
	int wkt = 0;//���μ����� ���� �ð�!
	
	float S_wt = 0; //Sum of Wait Time;
	float S_tt = 0; //Sum of Turnaround Time;

	for (int k = 0; k < *num; k++) { //�ʱ�ȭ
		P[k].rt = -1; //rt�� ���μ��� ������� ���
		P[k].pt = 0;
	}

	printf("\n\n - Round Robin - *Preemptive ����*  (Time_Quantum�� %d�Դϴ�.)", q);
	
	int time;
	for (time = 0; cnt < *num; time++) {
		for (int k = 0; k < *num; k++)
			if (P[k].at == time) {
				P[k].rt = P[k].bt; //���μ��� ����
				P[i].rt <= 0 ? i = k : NULL;		
			}
	
		if (P[i].rt >= 0) { //������������ ���Ѵ�
			if (index != i || P[i].rt == 0) { // ���� �߻� or �۾��� ����
				if (index == 1000 && time > 0)
					printf("\n%2d ~ %2d ���� Process ���� ��Ȳ ����", s_time, time);
				else if (time > 0)
					printf("\n%2d ~ %2d ���� Process %d ����", s_time, time, P[index].tag + 1);
				index = i;
				s_time = time;
			}

			P[i].rt -= 1;
			if (P[i].rt >= 0) { //�۾��ϴ� ���μ����� �������� �۾��� ���μ����� ������ -1 �̴�
				wkt++;
				P[i].pt++;
			}
			else 
				index = 1000;//���μ���Ÿ��
		}

		if (P[i].rt == 0) { //���μ����� �۾��� ��ħ!
			P[i].ct = time + 1; //���� ��������
			P[i].wt = time + 1 - P[i].bt - P[i].at; //�󸶳� ��ٷȴ���

			S_tt += P[i].ct - P[i].at;//average turnaround time = complete time - arrival time
			S_wt += P[i].wt; //�� ��ٸ� �ð�

			wkt = 0;
			cnt++;//�� �ϳ� �� �ߴ�
			
			//index = i;
			int k = 0;
			while (cnt != *num) { //�۾��� ���μ����� ���� �ִ�
				i == *num - 1 ? i = 0 : i++; //i = 1 -> 2 -> ... -> n -> 1
				k++;
				if (P[i].rt > 0) //���� �۾��� ���μ��� ã�Ҵ�
					break;
				else if (k == *num) //�۾��� ���μ����� ����
					break;
			}
		}

		else if (wkt == q) { //�� �� �� ����
			while (1) {
				i == *num - 1 ? i = 0 : i++; //i = 1 -> 2 -> ... -> n -> 1
				if (P[i].rt > 0) //���� �۾��� ���μ��� ã�Ҵ�
					break;
			}
			wkt = 0;
		}
	}
	printf("\n%2d ~ %2d ���� Process %d ����", s_time, time, P[index].tag + 1);

	Show_Result(P, cnt, S_wt, S_tt);
}