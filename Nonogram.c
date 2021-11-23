#include <stdio.h>
#include <time.h>
#include <Windows.h>
#include <conio.h>

char rfile();

void Nonogram(int *lvl);
void AddNum(int **field,int siz);
int emptyRC (int **field, int siz);
void CompletedRow(int **field,int siz);
void CompletedCol(int **field,int siz);
void OutputGame(int **field,int siz,int x,int y);
void control(int **field,int *lives,int win,char *select,char *move,int siz,int *x,int *y);

int main()
{
	
	char endorcont;
	int lvl=1;
	
	endorcont=rfile();
	if(endorcont!=13) return 0;
	system("cls");
	while(1){
		Nonogram(&lvl);
		if(lvl==0 || lvl>4) break;
		printf("\nPress Enter to continue");
		while(13!=getch());  
		system("cls");
	}
	
	return 0;
}

char rfile(){
	
	int i=0;
	char endorcont;
	char *text=(char*)calloc(2,sizeof(char));
	FILE *fp;
	if ((fp = fopen("rules.dat","r")) == NULL){
		printf("File error\n");
	}
	else{
		while(1){
			if(EOF==fscanf(fp,"%c",&text[i++])) break;
			text=(char*)realloc(text,(i+1)*sizeof(char));	
		}
		puts(text);
		printf("\n");
		
	}
	
	printf("Press Enter to start\n");
	
	free(text);
	fclose(fp);
	
	while(1){
		endorcont=getch();
		if(endorcont==13 || endorcont==27){
			return endorcont;
		}
	}	
}
void Nonogram(int *lvl){
	
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	
	int x=5,y=5,i,j,k=0;
	int lives=3,win=1,siz=10+(*lvl);
	char select,move;
	
	int **field=(int**)calloc(siz,sizeof(int*));
	for(i=0;i<siz;i++){
		field[i]=(int*)calloc(siz,sizeof(int));
	}
	
	while(emptyRC(field,siz)){
		AddNum(field,siz);
	}		
	
	while(1){	
		
		CompletedRow(field,siz);
		CompletedCol(field,siz);
		
		printf("Level %d\n\nLives: %d\n",*lvl,lives);
		OutputGame(field,siz,x,y);
		
		if(lives!=0 && win!=0){
			printf("\n\n\nW A S D - control\n");
			printf("E - start selecting\nQ - stop selecting");
		}
		
		if(lives==0) {
			SetConsoleTextAttribute(console, FOREGROUND_RED);
   			printf("\nYOU TRIED\n");
   			SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
   			(*lvl)=1;
   			break;
		}
		
		if(win==0) {
			SetConsoleTextAttribute(console, FOREGROUND_GREEN);
  			printf("\nWINNER\n");
   			SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
   			(*lvl)++;
   			break;	
		}
		
		win=0;
		for(i = 5; i < siz; i++){
			for(j = 5; j < siz; j++){
				if(field[i][j]==1){
					win++;
				}
			}
		}
		
		if(move==27){
			(*lvl)=0;
			return;
		}
		
		control(field,&lives,win,&select,&move,siz,&x,&y);
		
		system("cls");
		
	}	
}

void AddNum(int **field,int siz){
	int i,j,k;
	
	srand(time(NULL));
	for(i = 5; i < siz; i++){
		for(j = 5; j < siz; j++){
			field[i][j]=rand()%2;
		}
	}
	
	for ( i = 5; i < siz; i++ ){	
		for(j = 5; j < siz; j++){	
			if(field[i][j-1]==0 && field[i][j]==1 && field[i][4]!=0){
				k=0;
				while(k<4){
					field[i][k]=field[i][k+1];
					k++;
				}
				field[i][k]=0;
			}
			if(field[i][j]==1)
				field[i][4]++;
			
		}
	}
	
	for ( j = 5; j < siz; j++ ){	
		for(i = 5; i < siz; i++){	
			if(field[i-1][j]==0 && field[i][j]==1 && field[4][j]!=0){
				k=0;
				while(k<4){
					field[k][j]=field[k+1][j];
					k++;
				}
				field[k][j]=0;
			}
			if(field[i][j]==1)
				field[4][j]++;
			
		}
	}
}

int emptyRC (int **field, int siz){
	int firstnum=4;
	int i,j;
	
	
	for( i = 5; i < siz; i++){
		if (field[i][firstnum]==0){
			for(i = 0; i < siz; i++){
				for(j = 0; j < siz; j++){
					field[i][j]=0;
				}
			}
			return 1;
		}
	}
	
	for( j = 5; j < siz; j++){
		if (field[firstnum][j]==0){
			for(i = 0; i < siz; i++){
				for(j = 0; j < siz; j++){
					field[i][j]=0;
				}
			}
			return 1;
		}
	}
	
	return 0;
}

void CompletedRow(int **field,int siz){
	int i,j,k=0,sumrow=0;
	
	for(i = 5; i < siz; i++){
		sumrow=0;
		k=0;
		while(k<5) sumrow+=field[i][k++];
		
		for(j = 5; j < siz; j++){
			if(field[i][j]==1 ) break;
			else if(field[i][j]==2 ){
				sumrow--;
			}
			else if(sumrow==0) break;
		}
		
		if(sumrow==0){
			for(j = 5; j < siz; j++){
				if(field[i][j]==0) field[i][j]=3;
			}
		}			
	}
}

void CompletedCol(int **field,int siz){
	int i,j,k,sumcol;
	
	for ( j = 5; j < siz; j++ ){
		sumcol=0;
		k=0;
		while(k<5) sumcol+=field[k++][j];
		
		for(i = 5; i < siz; i++){	
			if(field[i][j]==1 ) break;
			else if(field[i][j]==2 ){
				sumcol--;
			}
			else if(sumcol==0) break;
		}
		
		if(sumcol==0){
			for(i = 5; i < siz; i++){
				if(field[i][j]==0) field[i][j]=3;
			}
		}		
	}
}

void OutputGame(int **field,int siz,int x,int y){
	
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	int i,j;
	
	for ( i = 0; i < siz; i++ ){
			for( j = 0; j < siz; j++ ){
				if(i>4 && j>4 ){
					if(field[i][j]==2){
						SetConsoleTextAttribute(console, FOREGROUND_GREEN);
						if(i==y && j==x) {
							printf(": # :");
						}
       					else printf("| # |");
       					SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
					} 
					if(field[i][j]==3){
						SetConsoleTextAttribute(console, FOREGROUND_RED);
						if(i==y && j==x){
							printf(": X :");
						} 
          				else printf("| X |");
       					SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
					}
					if(field[i][j]==1 || field[i][j]==0){
						if(i==y && j==x){
							printf(":   :");
						}
						else printf("|   |");
					}
				}
				else{
					SetConsoleTextAttribute(console, FOREGROUND_BLUE);
					if(field[i][j]!=0) printf( "|%2d |", field[i][j]);
					if(field[i][j]==0) printf("     ");
					SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
				}
			}
		printf( "\n");
		}
}

void control(int **field,int *lives,int win,char *select,char *move,int siz,int *x,int *y){
	if(lives!=0 && win!=0) *move=getch();
	if(*move=='E' || *move=='e') *select=*move;
	if(*move=='Q' || *move=='q') *select=0;
	
	switch(*move){
		case 72:
		case'W':
		case'w':{
			if(*y<6) break; 
			(*y)--;
		};break;
		
		case 80:
		case'S':
		case's':{
			if(*y==siz-1) break;
			(*y)++;
		};break;
		
		case 75:
		case'A':
		case'a':{
			if(*x<6) break; 
				(*x)--;
			};break;
		
		case 77:
		case'D':
		case'd':{
			if(*x==siz-1) break;
			(*x)++;
			};break;
	}
	
	if(*select=='E' || *select=='e'){
		if(field[*y][*x]==1) field[*y][*x]=2;
		if(field[*y][*x]==0) {
			field[*y][*x]=3;
			(*lives)--;
		} 
	}
}
