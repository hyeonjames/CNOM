#include <stdlib.h>
#include <stdio.h>
#include <stack>
#include <iostream>
#include <conio.h>
#include<winsock2.h>
#include <string>
#define ID_YES 87
#define ID_NO 88
using namespace std;
void title(char *change);
void SetColor(int color);
void sysclear();
void SetConsoleSize(int lines,int cols){
	char tm[255];
	sprintf(tm,"mode con lines=%d cols=%d",lines,cols);
	system(tm);
}
int MenuSecond(int type,int count,int cols,int lines,int ccols,int clines,char* descip,string *menuname,int *menuport,int stx);

void gotoxy(int x, int y)
{
	COORD pos={x,y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
void SetColor(int color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
void title(char *change){
	char *tm=new char[sizeof(change)+7];
	sprintf(tm,"title %s",change);
	system(tm);
} 
COORD getxy(void) 
{ 
CONSOLE_SCREEN_BUFFER_INFO buffInfo; 

GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&buffInfo); 
return buffInfo.dwCursorPosition; 
} 
void MakeSq(int cols,int lines,int scols,int slines,int color){
	int i,j;
	COORD nw=getxy();
	gotoxy(cols,lines);
	SetColor(color);
	printf("旨");
	for(i=0;i<scols-2;i++) printf("收");
	printf("旬");
	for(i=0;i<slines-2;i++){
		gotoxy(cols,lines+i+1);
		printf("早");
		for(j=0;j<scols-2;j++) printf("﹛");
		printf("早");
	}
	gotoxy(cols,lines+i+1);
	printf("曲");
	for(i=0;i<scols-2;i++) printf("收");
	printf("旭");
	SetColor(7);
	gotoxy(nw.X,nw.Y);
}
void SetBGColor(int lines,int cols,int clines,int ccols,int color){
	int i,j;
	SetColor(color);
	for(i=0;i<=clines-lines;i++){
		gotoxy(cols,lines+i);
		for(j=0;j<=ccols-cols;j++) printf(" ");
	}
}
int MenuSecond(int type,int count,int cols,int lines,int ccols,int clines,char* descip,string *menuname,int *menuport,int stx){
	int i=0,k;
	gotoxy(cols,lines);
	printf(" %s",descip);
	if(type==1) MakeSq(cols,lines+1,ccols,clines+1,12);
	while(1){
		for(i=0;i<count;i++){
			gotoxy(cols+2,lines+i+2);
			if(stx==i){
				SetColor(31);
				printf("≡%s",(char*)menuname[i].c_str());
				SetBGColor(lines+i+2,cols+strlen((char*)menuname[i].c_str())+4,lines+i+2,cols+(ccols*2)-4,31);
			} else {
				SetColor(15);
				printf("﹛%s",(char*)menuname[i].c_str());
				SetBGColor(lines+i+2,cols+strlen((char*)menuname[i].c_str())+4,lines+i+2,cols+(ccols*2)-4,15);
			}
		}
		while(1){
			k=getch();
			if(k==72)
			{
				if(stx>0) stx--;
				else if(stx==0) stx=count-1;
				break;
			}
			if(k==80)
			{
				if(stx<count-1) stx++;
				else if(stx==count-1) stx=0;
				break;
			}
			if(k==13) return menuport[stx];
		}
	}
}
void sysclear(){
	SetColor(7);
	system("cls");
}