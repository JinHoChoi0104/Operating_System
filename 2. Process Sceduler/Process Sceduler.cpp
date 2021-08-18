#include <stdio.h>
#include <windows.h>

class Process {
public: //데이터
	int at; //Arrival Time 도착시간
	int bt; //Burst Time 처리시간
	int pr; //Priority 우선순위

	int pt = 0; //Processing Time
	int wt = 0; //Wait Time 대기시간
	int ct = 0; //Complete Time 작업이 끝났을 때의 시간 
	int rt = 0; //일이 얼마나 남았는지
	int tag = 0; //데이터의 입력 순번

public:
	Process(int _at, int _bt, int _pr) { //생성자
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
	Process P_L[10]; //Process 리스트
	int num = 0; //Process 갯수
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
	Process P_s;//임시 Process
	int i = 0;

	printf("\n 작업을 수행 할 Proces 개수를 입력하시오: ");
	scanf_s("%d", num);
	while (*num > 10) {
		printf("\n* ERROR * Process는 최대 10개까지 입력 할 수 있습니다!\n다시 입력해주세요: ");
		scanf_s("%d", num);
	}
	for (i = 0; i < *num; i++) {
		while (1) {
			printf("\n%d번째 Process의 Arrival Time, Burst Time, Priority를 입력하시오:", i + 1);
			scanf_s("%d %d %d", &P[i].at, &P[i].bt, &P[i].pr);
			if (P[i].bt == 0)
				printf("\n* ERROR * Burst Time은 0이 될 수 없습니다! 다시 입력해주세요.");
			else {
				P[i].tag = i;
				break;
			}
		}
	}
	printf("\n Round Robin에 사용 될 Time Quantum을 입력하시오: ");
	scanf_s("%d", q);

	for (i = *num - 1; i > 0; i--) { //버블 정렬을 통해 도착 순서대로 바꾼다
		for (int j = 0; j < i; j++)
		{
			if (P[j].at > P[j + 1].at) { //j가 늦게 도착 자리 바꾼다 
				P_s = P[j];
				P[j] = P[j + 1];
				P[j + 1] = P_s;
			}
		}
	}
}

void Show_Result(Process P[], int num, float Swt, float Stt) { //출력은 입력 순으로
	float A_wt = Swt; //Average Wait Time
	float A_tt = Stt; //Average Turnaround Time

	for (int j = 0; j < num; j++) {
		for (int i = 0; i < num; i++) {
			if (j == P[i].tag)
				printf("\n %d번째 Process: Processing = %2d,  Waiting = %2d,  Complete = %2d", j + 1, P[i].pt, P[i].wt, P[i].ct);
		}
	}

	A_wt /= num;
	A_tt /= num;
	printf("\n Average Wait Time = %.4f, Average Turnaround Time = %.4f\n", A_wt, A_tt);
};

void FCFS(Process P[], int *num) {
	int i = 0; //일 할 프로세스
	int s_time = 0; //일 시작한 시간

	int cnt = 0; //처리한 프로세스의 수

	float S_wt = 0; //Average Wait Time;
	float S_tt = 0; //Average Turnaround Time;

	for (int k = 0; k < *num; k++) {//초기화
		P[k].rt = -1; //rt를 프로세스 진행률로 사용
		P[k].pt = 0;
	}

	printf("\n\n - FCFS - ");
	
	for (int time = 0; cnt < *num; time++) { //시간이 흘러 간다

		P[i].at <= time && P[i].rt<0 ? P[i].rt = P[i].bt : NULL; //도착해 있으면 rt에 값을 넣어준다

		if (P[i].rt > 0) { //도착해있으면 일한다
			if (P[i].rt == P[i].bt) { //프로세스가 작업을 시작
				if (s_time != time) // 마지막으로 작업이 끝난시간과 시작한 시간이 다르다
					printf("\n%2d ~ %2d 까지 Process 진행 상황 없음", s_time, time);
				s_time = time; //프로세스의 시작 시간 저장
			}
			P[i].rt--;
			P[i].pt++;
		}

		if (P[i].rt == 0) { //프로세스가 작업을 마쳤다
			P[i].ct = time + 1; //프로세스가 끝난 시간
			P[i].wt = time + 1 - P[i].bt - P[i].at; //프로세스가 기다린 시간

			S_tt += P[i].ct - P[i].at; //Complete Time - Arrival Time
			S_wt += P[i].wt;

			printf("\n%2d ~ %2d 까지 Process %d 진행", s_time, time+1, P[i].tag + 1);
			s_time = time + 1;

			cnt++; //프로세스가 하나 끝남
			i++; //다음프로세스 가리킨다 (도착한 순으로 정렬 되있으므로)
		}
	}

	Show_Result(P, cnt, S_wt, S_tt);
}

void SJF(Process P[], int *num) { //Shortest Job First
	int i = 0; //일 할 프로세스
	int index = 1000; //일 하던 프로세스
	int s_time = 0; //일 시작한 시간

	int cnt = 0; //처리한 프로세스
	int pr = 0; //Preemptive를 체크한다

	float S_wt = 0; //Sum of Wait Time;
	float S_tt = 0; //Sum of Turnaround Time;

	for (int k = 0; k < *num; k++) { //초기화
		P[k].rt = -1; //rt를 프로세스 진행률로 사용
		P[k].pt = 0;
	}

	printf("\n\n - SJF - *Preemptive 적용*");

	int time;
	for (time = 0; cnt < *num; time++) { //SJF 알고리즘 시작
		for (int k = 0; k < *num; k++) {
			if (P[k].at == time) { //프로세스 도착
				P[k].rt = P[k].bt; 
				pr = 0; //Preemptive 발생할 수도 있다.
			}
		}

		if (pr == 0) { //작업하던 프로세스가 끝났거나 새로 도착한 프로세스가 있거나
			if (cnt == *num - 1) //안 끝난 프로세스가 하나
				for (int k = 0; k < *num; k++)
					P[k].rt > 0 ? i = k : NULL;
			else { //안 끝난 프로세스가 두 개 이상
				i = 0;
				for (int k = 1; k < *num; k++) {//끝난일인지 checek
					if (P[i].rt > 0 && P[k].rt > 0) //두개 다 안 끝남
						P[i].rt > P[k].rt ? i = k : NULL;
					else if (P[i].rt <= 0 && P[k].rt <= 0);
					else
						P[k].rt > 0 ? i = k : NULL;
				}//도착해 있는 프로세스들 중 SJ 먼저 처리한다
			}
		}

		if (P[i].rt >= 0) { //도착해있으면 일한다
			if (index != i || P[i].rt == 0) { // 선점 발생 or 작업이 끝남
				if (index == 1000 && time > 0)
					printf("\n%2d ~ %2d 까지 Process 진행 상황 없음", s_time, time);
				else if ( time > 0)
					printf("\n%2d ~ %2d 까지 Process %d 진행", s_time, time, P[index].tag + 1);
				index = i;
				s_time = time;
			}
			pr++;
			P[i].rt -= 1;
			P[i].rt >= 0 ? P[i].pt++ : index = 1000; //프로세싱타임
		}


		if (P[i].rt == 0) { //프로세스가 작업을 마침!
			P[i].ct = time + 1; //언제 끝났는지
			P[i].wt = time + 1 - P[i].bt - P[i].at; //얼마나 기다렸는지
			S_tt += P[i].ct - P[i].at;//average turnaround time = complete time - arrival time
			S_wt += P[i].wt; //평균 기다린 시간
	
			pr = 0;
			cnt++;//일 하나 다 했다
		}
	}
	printf("\n%2d ~ %2d 까지 Process %d 진행", s_time, time, P[index].tag + 1);

	Show_Result(P, cnt, S_wt, S_tt);
}

void Priority_Scheduling(Process P[], int *num) {
	int i = 0; //일 할 프로세스
	int index = 1000; //일 하던 프로세스
	int s_time = 0; //일 시작한 시간

	int cnt = 0; //처리한 프로세스
	int pr = 0; //Preemptive를 체크한다

	float S_wt = 0; //Sum of Wait Time;
	float S_tt = 0; //Sum of Turnaround Time;

	for (int k = 0; k < *num; k++) { //초기화
		P[k].rt = -1; //rt를 프로세스 진행률로 사용
		P[k].pt = 0;
	}

	printf("\n\n - Priority Scheduling - *Preemptive 적용*");

	int time;
	for (time = 0; cnt < *num; time++) { // Prirority Sceduling 시작
		for (int k = 0; k < *num; k++) {
			if (P[k].at == time) {
				P[k].rt = P[k].bt; //프로세스 도착
				pr = 0;
			}
		}

		if (pr == 0) { //작업하던 프로세스가 끝났거나 새로 도착한 프로세스가 있거나
			if (cnt == *num - 1) //안 끝난 프로세스가 하나
				for (int k = 0; k < *num; k++)
					P[k].rt > 0 ? i = k : NULL;
			else { //안 끝난 프로세스가 두 개 이상
				i = 0;
				for (int k = 1; k < *num; k++) {//끝난일인지 checek
					if (P[i].rt > 0 && P[k].rt > 0) //두개 다 안 끝남
						P[i].pr > P[k].pr ? i = k : NULL;
					else if (P[i].rt <= 0 && P[k].rt <= 0); //두개 다 끝났거나 안 들어왔거나
					else
						P[k].rt > 0 ? i = k : NULL;
				} //도착해 있는 프로세스들 중 Priority가 낮은 프로세스보다 처리한다.
			}
		}

		if (P[i].rt >= 0) { //도착해있으면 일한다
			if (index != i || P[i].rt == 0) { // 선점 발생 or 작업이 끝남
				if (index == 1000 && time > 0)
					printf("\n%2d ~ %2d 까지 Process 진행 상황 없음", s_time, time);
				else if (time > 0)
					printf("\n%2d ~ %2d 까지 Process %d 진행", s_time, time, P[index].tag + 1);
				index = i;
				s_time = time;
			}
			pr++;
			P[i].rt -= 1;
			P[i].rt >= 0 ? P[i].pt++ : index = 1000; //프로세싱타임
		}

		if (P[i].rt == 0) { //프로세스가 작업을 마침!
			P[i].ct = time + 1; //언제 끝났는지
			P[i].wt = time + 1 - P[i].bt - P[i].at; //얼마나 기다렸는지

			S_tt += P[i].ct - P[i].at;//average turnaround time = complete time - arrival time
			S_wt += P[i].wt; //평균 기다린 시간

			pr = 0;
			cnt++;//일 하나 다 했다
		}
	}

	printf("\n%2d ~ %2d 까지 Process %d 진행", s_time, time, P[index].tag + 1);

	Show_Result(P, cnt, S_wt, S_tt);
}

void RR(Process P[], int *num, int q) { //Round Robin
	int i = 0;//일 할 프로세스
	int index = 1000; //일 하던 프로세스
	int s_time = 0; //일 시작한 시간

	int cnt = 0;//처리한 프로세스
	int wkt = 0;//프로세스당 일한 시간!
	
	float S_wt = 0; //Sum of Wait Time;
	float S_tt = 0; //Sum of Turnaround Time;

	for (int k = 0; k < *num; k++) { //초기화
		P[k].rt = -1; //rt를 프로세스 진행률로 사용
		P[k].pt = 0;
	}

	printf("\n\n - Round Robin - *Preemptive 적용*  (Time_Quantum은 %d입니다.)", q);
	
	int time;
	for (time = 0; cnt < *num; time++) {
		for (int k = 0; k < *num; k++)
			if (P[k].at == time) {
				P[k].rt = P[k].bt; //프로세스 도착
				P[i].rt <= 0 ? i = k : NULL;		
			}
	
		if (P[i].rt >= 0) { //도착해있으면 일한다
			if (index != i || P[i].rt == 0) { // 선점 발생 or 작업이 끝남
				if (index == 1000 && time > 0)
					printf("\n%2d ~ %2d 까지 Process 진행 상황 없음", s_time, time);
				else if (time > 0)
					printf("\n%2d ~ %2d 까지 Process %d 진행", s_time, time, P[index].tag + 1);
				index = i;
				s_time = time;
			}

			P[i].rt -= 1;
			if (P[i].rt >= 0) { //작업하던 프로세스가 끝났지만 작업할 프로세스가 없으면 -1 이다
				wkt++;
				P[i].pt++;
			}
			else 
				index = 1000;//프로세싱타임
		}

		if (P[i].rt == 0) { //프로세스가 작업을 마침!
			P[i].ct = time + 1; //언제 끝났는지
			P[i].wt = time + 1 - P[i].bt - P[i].at; //얼마나 기다렸는지

			S_tt += P[i].ct - P[i].at;//average turnaround time = complete time - arrival time
			S_wt += P[i].wt; //총 기다린 시간

			wkt = 0;
			cnt++;//일 하나 다 했다
			
			//index = i;
			int k = 0;
			while (cnt != *num) { //작업할 프로세스가 남아 있다
				i == *num - 1 ? i = 0 : i++; //i = 1 -> 2 -> ... -> n -> 1
				k++;
				if (P[i].rt > 0) //다음 작업할 프로세스 찾았다
					break;
				else if (k == *num) //작업할 프로세스가 없다
					break;
			}
		}

		else if (wkt == q) { //한 텀 일 다함
			while (1) {
				i == *num - 1 ? i = 0 : i++; //i = 1 -> 2 -> ... -> n -> 1
				if (P[i].rt > 0) //다음 작업할 프로세스 찾았다
					break;
			}
			wkt = 0;
		}
	}
	printf("\n%2d ~ %2d 까지 Process %d 진행", s_time, time, P[index].tag + 1);

	Show_Result(P, cnt, S_wt, S_tt);
}