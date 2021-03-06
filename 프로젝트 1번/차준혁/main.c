#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#define MAX 100

int** GraphList[MAX] = { NULL, }; //각 그래프를 담을 배열 이거 동적할당해도 되긴 함
int GraphIndex = 0;
int VertexNumList[MAX] = { 0, }; //각 그래프의 정점 개수를 담을 배열

int* visited; //visted배열은 각 정점에 방문했는지를 체크함.
int* path; //path배열은 경로를 저장하는 배열.
int pathIndex; //path배열의 index
int* Q; //BFS에서 쓸 Queue
int front; //BFS에서 쓸 index, Queue에서 제일 앞에 있는 원소를 가리키는 index
int rear; //BFS에서 쓸 index, Queue에서 제일 뒤에 있는 원소를 가리키는 index

void readMap(char fileloc[20]) {
	// "input.txt" txt파일 읽기
	FILE* fp = fopen(fileloc, "r");
	if (fp == NULL) {
		printf("파일 읽기 실패\n");
	}
	else {
		//한 줄씩 읽기
		char buffer[MAX] = { 0, };
		int check = 0; //정점의 개수를 나타내는 줄과 인접 노드 정보를 나타내는 줄 구분하는 기준
		int rowIndex = 0;
		int colIndex = 0;
		while (fgets(buffer, sizeof(buffer), fp)) {
			//printf("%s\n", buffer);

			//인접 노드 정보를 나타내는 줄을 읽을 때
			if (check != 0) {
				check--;

				//인접행렬에 인접 노드 정보 채우기
				int bufferSize = strlen(buffer);
				if (bufferSize > 2) {
					for (int i = 2; i < bufferSize; i++) {
						if ((i % 2) == 1) continue; //버퍼에서 짝수index에 인접 노드 정보가 있으므로
						colIndex = (buffer[i] - '0') - 1;
						GraphList[GraphIndex - 1][rowIndex][colIndex] = 1; //정점 a와 b는 인접을 뜻함.
					}
				}
				rowIndex++;
				colIndex = 0;
				continue;
			}

			//정점 개수를 나타내는 줄을 읽을 때
			int NumSize = sscanf(buffer,"%d",&check);//그래프 정점의 개수
			//printf("%d\n", check);
			//printf("%d\n", NumSize); // NumSize는 buffer에서 읽은 데이터의 크기(숫자 자리수를 뜻함.)
			VertexNumList[GraphIndex] = check;
			rowIndex = 0;
			//정점 개수만큼 그래프 인접 행렬로 동적할당해서 생성 ex.GraphList[0] => 첫번째 그래프의 인접행렬
			GraphList[GraphIndex] = (int**)malloc(sizeof(int) * check);
			for (int i = 0; i < check; i++) {
				GraphList[GraphIndex][i] = (int*)malloc(sizeof(int) * check);
			}

			//인접행렬 0으로 초기화
			for (int i = 0; i < check; i++) {
				for (int j = 0; j < check; j++) {
					GraphList[GraphIndex][i][j] = 0;
				}
			}
			GraphIndex += 1;
		}
		//printf("전체 그래프 개수 : %d\n\n", GraphIndex); //전체 그래프 개수 출력

		//만들어진 그래프들 출력
		/*
		for (int k = 0; k < GraphIndex; k++) {
			printf("%d   \n",VertexNumList[k]);
			for (int a = 0; a < VertexNumList[k]; a++) {
				for (int b = 0; b < VertexNumList[k]; b++) {
					printf("%d ", GraphList[k][a][b]);
				}
				printf("\n");
			}
			printf("\n");
		}
		*/
	}
	fclose(fp);
}

void makeArr(int Gindex, int SearchCheck) { //SearchCheck가 1이면 DFS, 2이면 BFS
	visited = (int*)calloc(VertexNumList[Gindex], sizeof(int));
	path = (int*)calloc(VertexNumList[Gindex], sizeof(int));
	pathIndex = 0;
	if (SearchCheck == 2) {
		Q = (int*)calloc( VertexNumList[Gindex] + 1 , sizeof(int));
		front = -1;
		rear = -1;
	}
}

void freeArr(int SearchCheck) { 
	if (SearchCheck == 1) {
		printf("깊이 우선 탐색\n\n");
	}
	else if (SearchCheck == 2) {
		printf("너비 우선 탐색\n\n");
		free(Q);
	}
	int	pathSize = _msize(path) / sizeof(int);
	for (int i = 0; i < pathSize; i++) {
		printf("%d ", path[i]);
		if (i != (pathSize - 1)) {
			printf("- ");
		}
		else {
			printf("\n\n");
		}
	}

	free(visited);
	free(path);
}

void DFS(int Gindex, int v) { //Gindex가 1이면 첫번째 그래프를 뜻함. v는 시작 노드
	if (visited[v] == 0) {
		visited[v] = 1;
		path[pathIndex++] = v+1;
		for (int i = 0; i < VertexNumList[Gindex]; i++) { //여기서 i는 v와 연결된 노드를 의미함.
			if (GraphList[Gindex][v][i] != 0 && visited[i] == 0) {
				DFS(Gindex, i);
			}
		}
		return;
	}
	else {
		return;
	}

}

void BFS(int Gindex, int v) { //Gindex가 1이면 첫번째 그래프를 뜻함. v는 시작 노드
	if (visited[v] == 0) {
		visited[v] = 1;
		path[pathIndex++] = v + 1;
		Q[++rear] = v;
		while (front != rear) {
			v = Q[++front];
			for (int i = 0; i < VertexNumList[Gindex]; i++) { //여기서 i는 v와 연결된 노드를 의미함.
				if (GraphList[Gindex][v][i] != 0 && visited[i] == 0) {
					visited[i] = 1;
					path[pathIndex++] = i + 1;
					if (path[_msize(path) / sizeof(int) - 1] != 0) return; //path가 다 채워지면 return
					Q[++rear] = i;
				}
			}
		}
		return;
	}
	else {
		return;
	}
}

void FreeMemory() { //동적할당한 메모리 해제
	for (int i = 0; i < GraphIndex; i++) {
		int temp = _msize(GraphList[i]) / sizeof(int);
		//printf("%d\n", temp);
		for (int j = 0; j < temp; j++) {
			free(GraphList[i][j]);
		}
		free(GraphList[i]);
	}	
}

int main() {
	
	readMap("input.txt");
	for (int i = 0; i < GraphIndex; i++) {
		printf("그래프 [%d]\n\n", (i + 1));
		printf("---------------------------------\n\n");
		//DFS 깊이 우선 탐색
		makeArr(i, 1);
		DFS(i, 0);
		freeArr(1);
		//BFS 너비 우선 탐색
		makeArr(i, 2);
		BFS(i, 0);
		freeArr(2);
		printf("=================================\n\n\n");
	}
	//동적할당한 메모리 해제
	FreeMemory();
}