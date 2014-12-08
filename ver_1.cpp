#include "stdcnp.h"
#define GAME_CONNECT 100
#define GAME_LISTEN 101
#define GAME_TWOPLAY 102
#define GAME_EXIT 200
#define STONE_DARK 13
#define STONE_WHITE 14
#define ONEP 15
#define TWOP 16
#define SERVER 17
#define CLIENT 18
void ErrorMsg(char *message);
void NetworkGame(SOCKET,char*,int);
void ClearOhmok();
void SoloGame();
void DrawOhMok();
void SetNotice(char *txt);
int chk();
int suv=0;
unsigned short ohmok[13][13];
int main()
{
	string mainmenu[]={"Connect [ 들어가기 ]","Listen [ 기다리기 ]","2 P ( OffLine ) [ 2P 오프라인 ]","Exit [종 료]"};
	int mport[]={GAME_CONNECT,GAME_LISTEN,GAME_TWOPLAY,GAME_EXIT};
	int gt=0;
	WSADATA wsaData;
	SOCKADDR_IN addr;
	int port=36214;
	int caddrsize=0;
	char ip[25];
	SOCKET sServer,sClient;
	//메뉴 : 접속 , 기다리기 , 2인 플레이(오프라인) , 종료
MainMenu:
	title("CNOM 0.0.1 - By Qoo . http://cyworld.com/qoohk");
	sysclear();
	SetConsoleSize(17,68);
	gt=MenuSecond(0,4,10,5,20,4,"please select menu ~# [메뉴를 선택해주세요.]",mainmenu,mport,0);
	switch(gt)
	{
		case 0:
			ErrorMsg("Select Error");
			break;
		case GAME_EXIT:
			WSACleanup();
			exit(0);
			break;
		case GAME_TWOPLAY:
			SoloGame();
			break;
		case GAME_LISTEN:
			
			sysclear();
			SetConsoleSize(20,75);
			SetColor(15);
			printf("WSA Start Up ... ");
			if(WSAStartup(MAKEWORD(2,2),&wsaData)!=0)
			{
				ErrorMsg("WSA Start Error!");
			}
			printf("OK\nSocket Create ... ");
			sServer=socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
			if(sServer == INVALID_SOCKET) ErrorMsg("Socket Create Error");
			printf("OK\nBind.. ");
			memset(&addr,0,sizeof(addr));
			addr.sin_addr.s_addr = htonl(INADDR_ANY); addr.sin_family = AF_INET;
			addr.sin_port = htons(port);	
			if(bind(sServer,(SOCKADDR*)&addr,sizeof(addr))==SOCKET_ERROR)
			{
				ErrorMsg("Bind Error");	
			}
			if(listen(sServer,5)==SOCKET_ERROR)
			{
				ErrorMsg("Listen Error");
			}
			printf("OK\n");
			printf("... accept...");
			SOCKADDR_IN caddr;
			caddrsize=sizeof(caddr);
			sClient=accept(sServer,(SOCKADDR*)&caddr,&caddrsize);
			if(sClient==INVALID_SOCKET) ErrorMsg("ACCEPT ERROR");
			printf("OK\n");
			NetworkGame(sClient,inet_ntoa(caddr.sin_addr),SERVER);
			break;
		case GAME_CONNECT:
			sysclear();
			SetColor(15);
			SetConsoleSize(20,75);
			printf("Enter the IP address to access. (Up to 25 bytes) [IP주소를 입력해주세요.]\n->");
			SetColor(2);
			scanf("%s",ip);
			SetColor(15);
			printf("WSA Start Up ... ");
			if(WSAStartup(MAKEWORD(2,2),&wsaData)!=0)
			{
				ErrorMsg("WSA Start Error!");
			}
			printf("OK\nSocket Create ... ");
			sClient=socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
			if(sClient == INVALID_SOCKET) ErrorMsg("Socket Create Error");
			printf("OK\nConnect ... ");
			memset(&addr,0,sizeof(addr));
			addr.sin_addr.s_addr = inet_addr(ip); addr.sin_family = AF_INET;
			addr.sin_port = htons(port);	
			if(connect(sClient,(SOCKADDR*)&addr,sizeof(addr))==SOCKET_ERROR)
			{
				printf("\nSorry.. Connect Error.. \n");
				printf("Do you want to go back?( Y or N )\n    - select y or n");
				while(1)
				{
					char th=getch();
					if(th=='Y' || th=='y') goto MainMenu;
					else if(th=='N' || th=='n') exit(0);
				}
			}
			printf("OK\n");
			NetworkGame(sClient,ip,CLIENT);
			break;
	}
	WSACleanup();
	return EXIT_SUCCESS;
}
void NetworkGame(SOCKET sck,char *ip,int type)
{
	char input[255],output[255],*f;
	char key=0;
	int nall=0,swin=0,slose=0,cwin=0,close=0,ny,nx,tx,ty,kto=0,x,y,result;
	SetConsoleSize(27,90);
gamecl:
	ClearOhmok();
	sysclear();
	DrawOhMok();
	suv=SERVER;
	while(1)
	{
		gotoxy(50,11);
		if(type==SERVER) printf("○ : SERVER(ME) ● : CLIENT");
		else if(type==CLIENT) printf("○ : SERVER ● : CLIENT(ME)");
		gotoxy(50,12);
		printf("총 경기수:%d",nall);
		gotoxy(50,13);
		printf(" SERVER 전적 : %d 승 %d 패",swin,slose);
		gotoxy(50,14);
		printf(" CLIENT 전적 : %d 승 %d 패",cwin,close);
		gotoxy(50,15);
		printf(" 상대방 IP : %s",ip);
		if(type!=suv){
			char a[2];
			SetNotice("상대가 놓는중입니다. 기다려주십시오.");
			recv(sck,a,2,0);
			memset(input,0,255);
			recv(sck,input,atoi(a),0);
			sscanf(input,"%d %d",&y,&x);
			ohmok[y][x]=suv-4;
			gotoxy(x*4,y*2);
			SetColor(15);
			if(suv==SERVER) printf("○");
			else if(suv==CLIENT) printf("●");
		}
		else if(type==suv)
		{
			ny=nx=6;
			gotoxy(nx*4,ny*2);
			SetColor(11);
			printf("⊙");
			SetNotice("놓을곳을 선택해주세요.");
			while(1)
			{
				tx=nx; ty=ny;
				key=getch();
				if(key==32)
				{
					if(ohmok[ny][nx]==0){
						ohmok[ny][nx]=suv-4;
						gotoxy(nx*4,ny*2);
						SetColor(15);
						if(type==SERVER) printf("○");
						else if(type==CLIENT) printf("●");
						memset(output,0,255);
						sprintf(output,"%d %d",ny,nx);
						kto=strlen(output);
						f=new char[kto];
						strcpy(f,output);
						if(kto>=10) sprintf(output,"%d%s",kto,f);
						else sprintf(output,"0%d%s",kto,f);
						send(sck,output,strlen(output),0);
						break;}
					else SetNotice("놓을수 없는 자리입니다.");
				}
				switch(key)
				{
				case 72: //위로
					if(ny>0) ny--;
					break;
				case 75: //왼
					if(nx>0) nx--;
					break;
				case 77: //오른
					if(nx<12) nx++;
					break;
				case 80: //밑으로
					if(ny<12) ny++;
					break;
				default:
					continue;
				}
				gotoxy(tx*4,ty*2);
				SetColor(15);
				if(ohmok[ty][tx]==STONE_DARK) printf("○");
				else if(ohmok[ty][tx]==STONE_WHITE) printf("●");
				else if(ty==12 && tx==0) printf("┗");
				else if(ty==12 && tx==12) printf("┛");
				else if(tx==0 && ty==0) printf("┏");
				else if(tx==12 && ty==0) printf("┓");
				else if(ty==0) printf("┳");
				else if(tx==0) printf("┣");
				else if(tx==12) printf("┫");
				else if(ty==12) printf("┻");
				else printf("╋");
				gotoxy(nx*4,ny*2);
				SetColor(11);
				printf("⊙");
			}
		}
		suv=suv==SERVER ? CLIENT : SERVER;
		result=chk();
		if(result==STONE_DARK)
		{
			SetNotice("SERVER WIN");
			swin++;
			nall++;
			close++;
			suv=ONEP;
			gotoxy(50,15);
			printf(" 게임이 끝이 났습니다.");
			gotoxy(50,16);
			printf(" 한판 더하시겠습니까? ( y or n )");
			while(1)
			{
				key=getch();
				if(key=='Y' || key =='y')
				{
					memset(input,0,strlen(input));
					send(sck,"YES",strlen("YES"),0);
					recv(sck,input,255,0);
					if(!strcmp(input,"YES"))
					{
						sysclear();
						goto gamecl;
					} else if(!strcmp(input,"NO"))
					{
						sysclear();
						SetColor(15);
						printf("상대가 수락을 하지 않았습니다.");
						getch();
						main();
					}
					
				}
				else
				{
					send(sck,"NO",strlen("NO"),0);
					main();
				}
			}
		}
		else if(result == STONE_WHITE)
		{
			SetNotice("CLIENT WIN");
			cwin++;
			slose++;
			nall++;
			suv=TWOP;
			gotoxy(50,15);
			printf(" 게임이 끝이 났습니다.");
			gotoxy(50,16);
			printf(" 한판 더하시겠습니까? ( y or n )");
			while(1)
			{
				key=getch();
				if(key=='Y' || key =='y')
				{
					memset(input,0,strlen(input));
					recv(sck,input,255,0);
					if(!strcmp(input,"YES"))
					{
						goto gamecl;
					} else if(!strcmp(input,"NO"))
					{
						SetColor(15);
						printf("상대가 수락을 하지 않았습니다.");
						getch();
						main();
					}
					
				}
				else if(key=='N' || key == 'n')
				{
					send(sck,"NO",strlen("NO"),0);
					main();
				}
			}
		}
	}
}

void SoloGame()
{
	int nx,ny,tx,ty;
	int owin=0,olose=0;
	int twin=0,tlose=0;
	int sall=0,result=0;
	char key=0;
	sysclear();
	SetConsoleSize(26,90);
	suv=ONEP;
game:
	ClearOhmok();
	sysclear();
	DrawOhMok();
	while(1)
	{
		nx=ny=6;
		if(suv==ONEP) SetNotice("1P 노실 차례입니다. ( 흑돌 )");
		else if(suv==TWOP) SetNotice("2P 노실 차례입니다. ( 백돌 )");
		gotoxy(nx*4,ny*2);
		SetColor(11);
		printf("⊙");
		gotoxy(50,11);
		SetColor(15);
		printf("○ : 1P ● : 2P");
		gotoxy(50,12);
		printf("총 경기수:%d",sall);
		gotoxy(50,13);
		printf(" 1P 전적 : %d 승 %d 패",owin,olose);
		gotoxy(50,14);
		printf(" 2P 전적 : %d 승 %d 패",twin,tlose);
		while(1)
		{
			tx=nx; ty=ny;
 			key=getch();
			if(key==32)
			{
				if(ohmok[ny][nx]==0){
					ohmok[ny][nx]=suv-2;
					gotoxy(nx*4,ny*2);
					SetColor(15);
					if(suv==ONEP) printf("○");
					else if(suv==TWOP) printf("●");
					break;}
				else SetNotice("놓을수 없는 자리입니다.");
			}
			switch(key)
			{
			case 72: //위로
				if(ny>0) ny--;
				break;
			case 75: //왼
				if(nx>0) nx--;
				break;
			case 77: //오른
				if(nx<12) nx++;
				break;
			case 80: //밑으로
				if(ny<12) ny++;
				break;
			default:
				continue;
			}
			gotoxy(tx*4,ty*2);
			SetColor(15);
			if(ohmok[ty][tx]==STONE_DARK) printf("○");
			else if(ohmok[ty][tx]==STONE_WHITE) printf("●");
			else if(ty==12 && tx==0) printf("┗");
			else if(ty==12 && tx==12) printf("┛");
			else if(tx==0 && ty==0) printf("┏");
			else if(tx==12 && ty==0) printf("┓");
			else if(ty==0) printf("┳");
			else if(tx==0) printf("┣");
			else if(tx==12) printf("┫");
			else if(ty==12) printf("┻");
			else printf("╋");
			gotoxy(nx*4,ny*2);
			SetColor(11);
			printf("⊙");
		}
		suv= suv==ONEP ? TWOP : ONEP;
		result=chk();
		if(result==STONE_DARK)
		{
		//	ClearOhmok();
			SetNotice("1P WIN");
			owin++;
			sall++;
			tlose++;
			suv=ONEP;
			gotoxy(50,15);
			printf(" 게임이 끝이 났습니다.");
			gotoxy(50,16);
			printf(" 한판 더하시겠습니까? ( y or n )");
			while(1)
			{
				key=getch();
				if(key=='Y' || key =='y') goto game;
				else if(key=='N' || key=='n') main();
			}
		}
		else if(result == STONE_WHITE)
		{
		//	ClearOhmok();
			SetNotice("2P WIN");
			twin++;
			olose++;
			sall++;
			suv=TWOP;
			gotoxy(50,15);
			printf(" 게임이 끝이 났습니다.");
			gotoxy(50,16);
			printf(" 한판 더하시겠습니까? ( y or n )");
			while(1)
			{
				key=getch();
				if(key=='Y' || key =='y') goto game;
				else if(key=='N'||key=='n') main();
			}
		}
	}
}
void ClearOhmok()
{
	int i,j;
	for(i=1;i<=13;i++)
		for(j=1;j<=13;j++) ohmok[i][j]=0;
}
int chk()
{
	int i,j,z,t1,t2,t3,t4,t5,t6,t7,t8;
	for(i=1;i<=13;i++)
	{
		for(j=1;j<=13;j++)
		{
			if(ohmok[i][j]!=0)
			{
				t1=t2=t3=t4=t5=t6=t7=t8=0;
				for(z=0;z<5;z++)
				{
					if(ohmok[i][j]==ohmok[i-z][j]) t1++;
					if(ohmok[i][j]==ohmok[i][j+z]) t2++;
					if(ohmok[i][j]==ohmok[i-z][j-z]) t3++;
					if(ohmok[i][j]==ohmok[i+z][j]) t4++;
					if(ohmok[i][j]==ohmok[i][j+z]) t5++;
					if(ohmok[i][j]==ohmok[i+z][j+z]) t6++;
					if(ohmok[i][j]==ohmok[i-z][j+z]) t7++;
					if(ohmok[i][j]==ohmok[i+z][j-z]) t8++;
				}
				if(t1>=5 || t2>=5 || t3>=5 || t4>=5 || t5>=5 || t6>=5 || t7>=5 || t8>=5) return ohmok[i][j];
			}
		}
	}
	return 0;
}
void SetNotice(char *txt)
{
	gotoxy(0,25);
	SetColor(15);
	printf("Notice:");
	SetColor(12);
	printf("%s",txt);
}
void DrawOhMok()
{
	int x,y,i,j,z;
	x=y=0;
	SetColor(15);
	for(j=0;j<13;j++)
	{
		gotoxy(x,0);
		if(j>0 && j<12) printf("┳");
		else if(j==0) printf("┏");
		else if(j==12) printf("┓");
		//if(j<12){
		gotoxy(0,y);
		if(j>0 && j<12) printf("┣");
		else if(j==0) printf("┏");
		else if(j==12) printf("┗");
		//}
		for(z=1;z<23;z++)
		{
			if(j==12 && z%2==0)
			{
				gotoxy(x,z);
				printf("┫");
			}
			else
			{
				gotoxy(x,z);
				printf("┃");
			}
			if(z%2==0 && j>0 && j<12)
			{
				gotoxy(x,z);
				if(ohmok[z/2][x/4]==STONE_DARK) printf("○");
				else if(ohmok[z/2][x/4]==STONE_WHITE) printf("●");
				else printf("╋");
			} else
			{
				if(j==12 && z%2==0)
				{
					gotoxy(z*2,y);
					printf("┻");
				} else
				{
					gotoxy(z*2,y);
					printf("━");
				}
			}

		}
		gotoxy(z*2,y);
		printf("━");
		gotoxy(x,23);
		printf("┃");
		gotoxy(x,24);
		if(j>0 && j<12) printf("┻");
		else if(j==0) printf("┗");
		else if(j==12) printf("┛");
		x+=4;
		gotoxy(48,y);
		if(j>0 && j<12) printf("┫");
		else if(j==0) printf("┓");
		else if(j==12) printf("┛");
		y+=2;
	}
	SetColor(15);
	gotoxy(50,0);
	printf(" ♧★ OhMok ☆♣");
	MakeSq(50,1,20,9,12);
	gotoxy(52,1);
	printf("설명서");
	gotoxy(52,2);
	printf("Space Bar : 놓기");
	gotoxy(52,3);
	printf("  ↑");
	gotoxy(52,4);
	printf("←  → :  이동");
	gotoxy(52,5);
	printf("  ↓");
	gotoxy(52,6);
	printf(" ○ : 흑돌 ");
	gotoxy(52,7);
	printf(" ● : 백돌 ");
	gotoxy(52,8);
	printf(" ⊙ : 놓으려고 하는 위치");
	gotoxy(50,10);
	printf(" 차례 : ");
	
	if(suv == ONEP) printf("1 P");
	else if(suv == TWOP) printf("2 P");
	else if(suv == SERVER) printf(" 서버");
	else if(suv == CLIENT) printf(" 클라이언트");
	
}
void ErrorMsg(char *message)
{
	MessageBox(FindWindow("ConsoleWindowClass",NULL),(LPCTSTR)message,(LPCTSTR)"Error",MB_OK|MB_ICONWARNING);
	WSACleanup();
	exit(0);
}