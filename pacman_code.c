#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<windows.h>
#include<time.h>
#include<locale.h>


#define LINE            30
#define COLUMN          101
#define PILL            111
#define SUPER_PILL      42
#define WALL            35
#define GHOST           87
#define PACMAN          67
#define MAX_GHOSTS      5

int difficulty;
char option;

void gotoxy1 (int x, int y){
	
	
	COORD CRD;
	CRD.X=x;
	CRD.Y=y;
	
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),CRD);
	
	
}

void displayMenu(){
	gotoxy1(39,25);
        printf("by Millenium");
	
		gotoxy1(42,12);
		printf("PAC-MAN");
		
        printf("\n                                Press 1 for level 1(easy)\n                                Press 2 for level 2(hard)\n                                Press 'P' for pausing the game\n\n                                ");
        
        scanf("%s", &option);
        

        if (option == '1') {
            difficulty=1;
        }
        if (option == '2') {
           difficulty=90;
        }
        system("cls");
} 

//PacMan Struct
typedef struct str_pacman
{
    int x,y;
    int start_x,start_y;
    int power;


} Pacman;

//Hayalet Struct
typedef struct str_ghost
{

    int x,y;
    int start_x,start_y;
    int alive;
    char last_move;

} Ghost;

//FONKS�YONLAR:

void debug(int x, char y)
{
    gotoxy(50,33);
    printf("DEBUG\t %d \t %c", x, y);
}

int Find_Ghost_Qtd(char map[][COLUMN])
{
    int i,j;
    int qnt = 0;
    for(i=0; i<LINE; i++)
        for(j=0; j<COLUMN; j++)
            if(map[i][j] == GHOST)
                qnt++;
    return qnt;
}

void gotoxy(int x, int y)
{
    //�mleci ekrandaki bir yere hareket ettirir
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),(COORD)
    {
        x-1,y-1
    });
}

void SetConsoleSize(unsigned width, unsigned height)
{
    SMALL_RECT r;
    COORD c;
    HANDLE stdoutHandle = GetStdHandle(STD_OUTPUT_HANDLE);

    c.X = width;
    c.Y = height;
    SetConsoleScreenBufferSize(stdoutHandle, c); //Ekran arabellek boyutunu ayarlar

    r.Left = 0;
    r.Top = 0;
    r.Right = width - 1;
    r.Bottom = height - 1;
    SetConsoleWindowInfo(stdoutHandle, 1, &r); //Ekran boyutunu ayarlar
}

void Get_Map(char map[][COLUMN])
{
    //Dosyadan matrisi al�r

    int i,j;
    FILE *arq;

    arq = fopen("labrint.txt","r");

    for(i=0; i<LINE; i++)
    {
        for(j=0; j<COLUMN; j++)
            fscanf(arq, "%c", &map[i][j]);
    }

    fclose(arq);
}

void Find_Pacman(char map[][COLUMN], int *posX, int *posY)
{
    //Pacman'in pozisyonunu bulun

    int i,j;
    for(i=0; i<LINE; i++)
        for(j=0; j<COLUMN; j++)
            if(map[i][j] == PACMAN)
            {
                *posX = j;
                *posY = i;
            }

}

void Find_Ghost(char map[][COLUMN],int *posX, int *posY,int current)
{
    //Hayaletin konumunu bul

    int i,j;
    int qnt = 0;
    for(i=0; i<LINE; i++)
        for(j=0; j<COLUMN; j++)
            if(map[i][j] == GHOST && current == qnt++)
            {
                *posX = j;
                *posY = i;
            }
}

void Print_Map(char map[][COLUMN])
{
    //Labirenti ekranda boyay�n


    HANDLE  hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);


    int i,j;
    for(i=0; i<LINE; i++)
        for(j=0; j<COLUMN; j++)
        {
            if(map[i][j] == WALL)
            {
                SetConsoleTextAttribute(hConsole,  9);
                printf("#");
            }
            else if(map[i][j] == PILL)
            {
                SetConsoleTextAttribute(hConsole, 15);
                printf("o");
            }
            else if(map[i][j] == SUPER_PILL)
            {
                SetConsoleTextAttribute(hConsole, 14);
                printf("*");
            }
            else if(map[i][j] == PACMAN)
            {
                SetConsoleTextAttribute(hConsole, 14);
                printf("C");
            }
            else if(map[i][j] == GHOST)
            {
                SetConsoleTextAttribute(hConsole, 12);
                printf("W");
            }
            else
                printf("%c",map[i][j]);
        }
}

void Update_Score(long *player_score,int score)
{
    //Puanla ilgili g�ncelleme

    HANDLE  hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    *player_score = *player_score + score;
    gotoxy(1,32);
    SetConsoleTextAttribute(hConsole, 2);
    printf("Score: %d", *player_score);
}

void Update_Time(int *gameplay_time)
{
    HANDLE  hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    gotoxy(1,33);
    SetConsoleTextAttribute(hConsole, 2);
    printf("Time:  %d", *gameplay_time);
}

void Update_Lives(int *lives)
{
    HANDLE  hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    gotoxy(1,34);
    SetConsoleTextAttribute(hConsole, 2);
    printf("Lives: %d", *lives);
}



int Verify_Win(char map[][COLUMN])
{
    int i, j;
    for(i=0; i<LINE; i++)
        for(j=0; j<COLUMN; j++)
            if(map[i][j] == PILL)
                return 0;
    return 1;
}

void Restart_Game(int *start_x, int *start_y, int *posX, int *posY,int *mov,int *lives)
{
    //Oyunu yeniden ba�lat�r

    int i;
    gotoxy(*posX+1,*posY+1);
    printf(" ");
    *posX = *start_x;
    *posY = *start_y;
    *mov = ' ';
    gotoxy(*start_x+1,*start_y+1);
    printf("C");
    if(*lives != 0)
    {
        for(i=5; i>=0; i--)
        {
            Sleep(250);
            gotoxy(45,33);
            printf("ARE YOU READY?");
            Sleep(250);
            gotoxy(45,33);
            printf("      %d       ",i);
        }
        gotoxy(51,33);
        printf("GO!");
        Sleep(250);
        gotoxy(51,33);
        printf("   ");
    }
}

void Print_Ghost(int *super)
{
    HANDLE  hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (*super)
    {
        SetConsoleTextAttribute(hConsole, 11);
        printf("w");
    }
    else
    {
        SetConsoleTextAttribute(hConsole, 12);
        printf("W");
    }
}

void Move_Pacman(int *posX, int *posY, char mov)
{
    //Pacman'i oyuncunun gitmek istedi�i yere g�re hareket ettirir

    HANDLE  hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(hConsole, 14);

    switch(mov)
    {
    case 'd':
        gotoxy(*posX+1,*posY+1);
        printf(" ");

        gotoxy(*posX+2,*posY+1);
        printf("C");
        *posX = *posX + 1;
        break;
    case 'w':
        gotoxy(*posX+1,*posY+1);
        printf(" ");

        gotoxy(*posX+1,*posY+0);
        printf("C");
        *posY = *posY - 1;
        break;
    case 'a':
        gotoxy(*posX+1,*posY+1);
        printf(" ");

        gotoxy(*posX+0,*posY+1);
        printf("C");
        *posX = *posX - 1;
        break;
    case 'x':
        gotoxy(*posX+1,*posY+1);
        printf(" ");

        gotoxy(*posX+1,*posY+2);
        printf("C");
        *posY = *posY + 1;
        break;
    }
}

int module(int x)
{
    if (x < 0)
        return -x;
    return x;
};

//De�i�iklik!
void Move_Ghost(char map[][COLUMN], int *ghostX, int *ghostY, int *pacX, int *pacY, int *super, char* last_move)
{
    int flag_same_direction = 1, flag_stopped = 1;      //bayraklar y�n de�i�tirmeyecek veya hareketsiz duracak
    int difX = *ghostX - *pacX;                         //bir hayaletin X ekseninden pacman'a olan mesafe
    int difY = *ghostY - *pacY;                         //bir hayaletin Y ekseninden pacman'a olan mesafe
    if(*super == 1)
    {
        *last_move = 'p';
        difX *= -1;                         //bir hayaletin X ekseninden pacman'a olan mesafe
        difY *= -1;                         //bir hayaletin Y ekseninden pacman'a olan mesafe
    }
    HANDLE  hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    srand(time(NULL));
    if (rand()%100 < difficulty)                                  //Hayaletin pacman'� takip etme �ans� de�i�ken
    {
        if(module(difX) > module(difY))                 //X ekseninin mesafe mod�l� Y'ninkinden b�y�kse,
            //X'de takip etmeye �al���yor
        {
            if(difX > 0)                                //hayaletin X'deki mesafesi 0'dan b�y�kse
            {
                //sola gitmeyi dene
                if(map[*ghostY][*ghostX - 1] != WALL && *last_move != 'd')
                {
                    gotoxy(*ghostX + 1,*ghostY + 1);
                    if(map[*ghostY][*ghostX] == GHOST)
                        printf(" ");
                    else
                    {
                        if(map[*ghostY][*ghostX] == SUPER_PILL)
                            SetConsoleTextAttribute(hConsole, 14);
                        else
                            SetConsoleTextAttribute(hConsole, 15);
                        if(map[*ghostY][*ghostX] != 'C')
                            printf("%c",map[*ghostY][*ghostX]);
                        else
                            printf("%c", ' ');
                    }
                    gotoxy(*ghostX,*ghostY + 1);
                    SetConsoleTextAttribute(hConsole, 12);
                    Print_Ghost(super);
                    *ghostX = *ghostX - 1;
                    *last_move = 'a';
                    flag_stopped = 0;                   //bayrak hayaletin hareket etti�ini do�rular

                }

            }
            else                                        //hayaletin X'deki mesafesi 0'dan k���kse
            {
                //sa�a gitmeyi dene
                if(map[*ghostY][*ghostX + 1] != WALL && *last_move != 'a')
                {
                    gotoxy(*ghostX + 1,*ghostY + 1);
                    if(map[*ghostY][*ghostX] == GHOST)
                        printf(" ");
                    else
                    {
                        if(map[*ghostY][*ghostX] == SUPER_PILL)
                            SetConsoleTextAttribute(hConsole, 14);
                        else
                            SetConsoleTextAttribute(hConsole, 15);
                        if(map[*ghostY][*ghostX] != 'C')
                            printf("%c",map[*ghostY][*ghostX]);
                        else
                            printf("%c", ' ');
                    }
                    gotoxy(*ghostX + 2,*ghostY + 1);
                    SetConsoleTextAttribute(hConsole, 12);
                    Print_Ghost(super);
                    *ghostX = *ghostX + 1;
                    *last_move = 'd';
                    flag_stopped = 0;                   //bayrak hayaletin hareket etti�ini do�rular

                }

            }
        }
        else                                            //Y ekseninin mesafe mod�l� X'inkinden b�y�kse,
            //Y'yi takip etmeye �al���yor
        {
            if(difY > 0)                                //hayaletin Y'deki mesafesi 0'dan b�y�kse
                //yukar� ��kmay� dene
            {
                if(map[*ghostY - 1][*ghostX] != WALL && *last_move != 'x')
                {
                    gotoxy(*ghostX + 1,*ghostY + 1);
                    if(map[*ghostY][*ghostX] == GHOST)
                        printf(" ");
                    else
                    {
                        if(map[*ghostY][*ghostX] == SUPER_PILL)
                            SetConsoleTextAttribute(hConsole, 14);
                        else
                            SetConsoleTextAttribute(hConsole, 15);
                        if(map[*ghostY][*ghostX] != 'C')
                            printf("%c",map[*ghostY][*ghostX]);
                        else
                            printf("%c", ' ');
                    }
                    gotoxy(*ghostX + 1,*ghostY);
                    Print_Ghost(super);
                    *ghostY = *ghostY - 1;
                    *last_move = 'w';
                    flag_stopped = 0;                   //bayrak hayaletin hareket etti�ini do�rular

                }

            }
            else                                        //hayaletin Y'deki mesafesi 0'dan az ise
                //a�a�� inmeyi dene
            {
                if(map[*ghostY + 1][*ghostX] != WALL && *last_move != 'w')
                {
                    gotoxy(*ghostX + 1,*ghostY + 1);
                    if(map[*ghostY][*ghostX] == GHOST)
                        printf(" ");
                    else
                    {
                        if(map[*ghostY][*ghostX] == SUPER_PILL)
                            SetConsoleTextAttribute(hConsole, 14);
                        else
                            SetConsoleTextAttribute(hConsole, 15);
                        if(map[*ghostY][*ghostX] != 'C')
                            printf("%c",map[*ghostY][*ghostX]);
                        else
                            printf("%c", ' ');
                    }
                    gotoxy(*ghostX + 1,*ghostY + 2);
                    SetConsoleTextAttribute(hConsole, 12);
                    Print_Ghost(super);
                    *ghostY = *ghostY + 1;
                    *last_move = 'x';
                    flag_stopped = 0;                   //bayrak hayaletin hareket etti�ini do�rular

                }

            }
        }
    }
    //hayalet hareket etmediyse flag_stopped s�f�rlanacak,
    //bu, hayaletin pacman'� takip edemeyece�i anlam�na gelir ��nk� engelleyen bir duvar vard�r.
    //bu durumda hayalet rastgele bir karakter kazan�r ve m�mk�n olan her yere gider.
    //hareket edebilir.
    //RANDOM�K B�L�M:
    if (flag_stopped == 1)
    {
        do
        {
            switch(rand()%4)
            {
            case 0: //YUKARI gitmeyi dene
                //yukar� bir duvar de�ilse ve son hareket a�a�� de�ilse
                if(map[*ghostY - 1][*ghostX] != WALL && ((*last_move != 'x')||
                        (
                            (map[*ghostY + 1][*ghostX] == WALL) && //Hayalet �ld�yse, burada test edildi�i gibi
                            (map[*ghostY][*ghostX + 1] == WALL) && //program "geriye do�ru y�r�yemeyece�ini" yok say�yor
                            (map[*ghostY][*ghostX - 1] == WALL)    //bu �ekilde hayalet yoldan ��k�� yolu olmadan ka�ar
                        )                                          //ayn� �ey di�er 3 taraf i�in de yap�l�r
                                                        ))
                {
                    gotoxy(*ghostX + 1,*ghostY + 1);
                    if(map[*ghostY][*ghostX] == GHOST)
                        printf(" ");
                    else
                    {
                        if(map[*ghostY][*ghostX] == SUPER_PILL)
                            SetConsoleTextAttribute(hConsole, 14);
                        else
                            SetConsoleTextAttribute(hConsole, 15);
                        if(map[*ghostY][*ghostX] != 'C')
                            printf("%c",map[*ghostY][*ghostX]);
                        else
                            printf("%c", ' ');
                    }
                    gotoxy(*ghostX + 1,*ghostY);
                    Print_Ghost(super);
                    *ghostY = *ghostY - 1;
                    *last_move = 'w';
                    flag_same_direction = 0;                        //hayaletin rastgele hareket etti�ini do�rular
                    break;
                }
                else

                case 1: //SA�A gitmeyi dene
                if(map[*ghostY][*ghostX + 1] != WALL && ((*last_move != 'a')||
                        (
                            (map[*ghostY][*ghostX - 1] == WALL)&&
                            (map[*ghostY - 1][*ghostX] == WALL)&&
                            (map[*ghostY + 1][*ghostX] == WALL)
                        )))
                {
                    gotoxy(*ghostX + 1,*ghostY + 1);
                    if(map[*ghostY][*ghostX] == GHOST)
                        printf(" ");
                    else
                    {
                        if(map[*ghostY][*ghostX] == SUPER_PILL)
                            SetConsoleTextAttribute(hConsole, 14);
                        else
                            SetConsoleTextAttribute(hConsole, 15);
                        if(map[*ghostY][*ghostX] != 'C')
                            printf("%c",map[*ghostY][*ghostX]);
                        else
                            printf("%c", ' ');
                    }
                    gotoxy(*ghostX + 2,*ghostY + 1);
                    SetConsoleTextAttribute(hConsole, 12);
                    Print_Ghost(super);
                    *ghostX = *ghostX + 1;
                    *last_move = 'd';
                    flag_same_direction = 0;                        //hayaletin rastgele hareket etti�ini bulur
                    break;
                }
                else

                case 2: //A�a�� gitmeyi dene
                if(map[*ghostY + 1][*ghostX] != WALL && ((*last_move != 'w')||(
                            (map[*ghostY - 1][*ghostX] == WALL) &&
                            (map[*ghostY][*ghostX + 1] == WALL) &&
                            (map[*ghostY][*ghostX - 1] == WALL)
                        )
                                                        ))
                {
                    gotoxy(*ghostX + 1,*ghostY + 1);
                    if(map[*ghostY][*ghostX] == GHOST)
                        printf(" ");
                    else
                    {
                        if(map[*ghostY][*ghostX] == SUPER_PILL)
                            SetConsoleTextAttribute(hConsole, 14);
                        else
                            SetConsoleTextAttribute(hConsole, 15);
                        if(map[*ghostY][*ghostX] != 'C')
                            printf("%c",map[*ghostY][*ghostX]);
                        else
                            printf("%c", ' ');
                    }
                    gotoxy(*ghostX + 1,*ghostY + 2);
                    SetConsoleTextAttribute(hConsole, 12);
                    Print_Ghost(super);
                    *ghostY = *ghostY + 1;
                    *last_move = 'x';
                    flag_same_direction = 0;                        //hayaletin rastgele hareket etti�ini bulur
                    break;
                }
                else

                case 3: //Sola gitme te�ebb�s�
                if(map[*ghostY][*ghostX - 1] != WALL && ((*last_move != 'd')||(
                            (map[*ghostY][*ghostX + 1] == WALL)&&
                            (map[*ghostY - 1][*ghostX] == WALL)&&
                            (map[*ghostY + 1][*ghostX] == WALL)
                        )))
                {
                    gotoxy(*ghostX + 1,*ghostY + 1);
                    if(map[*ghostY][*ghostX] == GHOST)
                        printf(" ");
                    else
                    {
                        if(map[*ghostY][*ghostX] == SUPER_PILL)
                            SetConsoleTextAttribute(hConsole, 14);
                        else
                            SetConsoleTextAttribute(hConsole, 15);
                        if(map[*ghostY][*ghostX] != 'C')
                            printf("%c",map[*ghostY][*ghostX]);
                        else
                            printf("%c", ' ');
                    }
                    gotoxy(*ghostX,*ghostY + 1);
                    SetConsoleTextAttribute(hConsole, 12);
                    Print_Ghost(super);
                    *ghostX = *ghostX - 1;
                    *last_move = 'a';
                    flag_same_direction = 0;                        //hayaletin rastgele hareket etti�ini bulur
                    break;
                }
            }
        }
        while (flag_same_direction == 1);
    }
}

void Pause_Game(int *is_paused)
{
    if(*is_paused)
    {
        gotoxy(50,33);
        printf("      ");
        *is_paused = *is_paused - 1;
    }
    else
    {
        gotoxy(50,33);
        printf("PAUSED");
        *is_paused = *is_paused + 1;
    }
}

void End_Game(int *player_score, int *lives)
{
    char end;
    system("cls");
    gotoxy(45,15);
    if(*lives == 0)
    {
        printf("GAME OVER");
        gotoxy(45,16);
        printf("SCORE: %d", *player_score);
    }
    else
    {
        printf("YOU WIN!");
        gotoxy(45,16);
        printf("SCORE: %d", *player_score);
    }
    gotoxy(45,18);
    printf("Press 'E' to exit");
    gotoxy(45,30);
    while (tolower(end) != 'e')
        end = getch();
}


int main()
{
	displayMenu();
	
    //De�i�kenler:
    char map[LINE][COLUMN];
    int i,j;
    int win =               0;
    int lives =             3;
    int qtGhosts =          Find_Ghost_Qtd(map);

    //hareket de�i�kenleri
    int mov, last_mov;
    int pacman_speed =      800;
    int ghosts_speed;
    int is_paused =        0;
    int is_powerful =       0;

    //kullan�c�ya veri g�steren de�i�kenler
    int gameplay_time =     0;
    long player_score =     0;

    //hareket zamanlay�c� de�i�kenleri
    int msec, timer_pacman, last_timer_pacman, can_move_pacman, timer_ghost, last_timer_ghost, can_move_ghost;
    float last_game_timer, game_timer;

    int flag_start_power =0, max_time_power=0;

    Pacman player;
    Ghost ghosts[MAX_GHOSTS];
    HANDLE  hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    clock_t start = clock(), time_dif;
    clock_t start_power = clock();

    //Ekran boyutu:
    SetConsoleSize(101,36);

    //Matrisi almak:
    Get_Map(map);

    //Pac-man'i bulmak:
    Find_Pacman(map,&player.start_x, &player.start_y);
    player.x = player.start_x;
    player.y = player.start_y;

    //Hayalet aramak:
    qtGhosts = Find_Ghost_Qtd(map);
    for(i=0; i<qtGhosts; i++)
    {
        Find_Ghost(map,&ghosts[i].start_x,&ghosts[i].start_y,i);
        ghosts[i].x = ghosts[i].start_x;
        ghosts[i].y = ghosts[i].start_y;
    }

    //Labirent g�steriliyor:
    Print_Map(map);

    //Skor g�steriliyor:
    Update_Score(&player_score, 0);

    //G�sterilen zaman:
    Update_Time(&gameplay_time);

    //Hayatlar� g�steren:
    Update_Lives(&lives);

    //Oyuna ba�la:
    while(!win && lives > 0)
    {

        //Pacman �al��t�r�ld���nda veya olmad���nda hayaletler h�z�n� ayarlar
        if(is_powerful)ghosts_speed=pacman_speed*0.7;
        else ghosts_speed = pacman_speed;

        Update_Time(&gameplay_time);
        //Oyuncu giri�i bekleniyor:
        if(kbhit())
        {
            while(kbhit())
            {
                last_mov = mov;
                mov = getch();
            }
            mov = tolower(mov);

            //Pacman'� durdurmamak i�in oyuncunun giri�ini "do�rulama"
            //oyuncu harekete paralel y�n� se�erse
            //duvar�n oldu�u yerde
            switch(mov)
            {
            case 'd':
                if(map[player.y][player.x + 1] == WALL)
                    mov = last_mov;
                break;
            case 'w':
                if(map[player.y - 1][player.x ] == WALL)
                    mov = last_mov;
                break;
            case 'a':
                if(map[player.y][player.x - 1] == WALL)
                    mov = last_mov;
                break;
            case 'x':
                if(map[player.y + 1][player.x ] == WALL)
                    mov = last_mov;
                break;
            case 's':
                break;
            case 'p':
                Pause_Game(&is_paused);
                mov = last_mov;
                break;
            default:
                mov = last_mov;
                break;
            }
        }

        //Pacman'� hareket ettirmek:
        if(can_move_pacman && !is_paused)           //zamanlay�c� harekete izin veriyorsa
        {
            can_move_pacman = 0;                    //izni geri �ekmek
            switch(mov)
            {
            case 'd':
                if(map[player.y][player.x + 1] != WALL)
                    Move_Pacman(&player.x,&player.y,mov);
                break;
            case 'w':
                if(map[player.y - 1][player.x] != WALL)
                    Move_Pacman(&player.x,&player.y,mov);
                break;
            case 'a':
                if(map[player.y][player.x - 1] != WALL)
                    Move_Pacman(&player.x,&player.y,mov);
                break;
            case 'x':
                if(map[player.y + 1][player.x] != WALL)
                    Move_Pacman(&player.x,&player.y,mov);
                break;
            case 's':
                mov = ' ';
                last_mov = ' ';
            }
        }


        for(i=0; i<qtGhosts; i++)                   //Pacman'in bir hayalete dokunup dokunmad���n� test etmek
        {
            if(player.x == ghosts[i].x && player.y == ghosts[i].y)
            {
                if(!is_powerful)
                {
                    for(j=0; j<qtGhosts; j++)                   //Pacman'in bir hayalete dokunup dokunmad���n� test etmek
                    {
                        gotoxy((ghosts[j].x+1),(ghosts[j].y+1));
                        printf("%c", ' ');
                        ghosts[j].x = ghosts[j].start_x;
                        ghosts[j].y = ghosts[j].start_y;
                    }
                    lives--;
                    Update_Lives(&lives);

                    if(lives > 0)
                    Restart_Game(&player.start_x,&player.start_y,&player.x,&player.y,&mov,&lives);
                }
                else
                {
                    ghosts[i].x = ghosts[i].start_x;
                    ghosts[i].y = ghosts[i].start_y;
                    map[player.y][player.x] = 32;
                    Update_Score(&player_score, 5);
                }
            }
        }

        //Hareket eden hayaletler:
        ;
        if(gameplay_time>10){
		
        
        if(can_move_ghost && !is_paused)            //zamanlay�c� harekete izin veriyorsa
        {
            can_move_ghost = 0;                     //izni geri �ekmek
            for(i=0; i<qtGhosts; i++)
            {
                Move_Ghost(map,&ghosts[i].x,&ghosts[i].y, &player.x, &player.y, &is_powerful, &ghosts[i].last_move);
            }
        }
	}

        for(i=0; i<qtGhosts; i++)                   //Pacman'in bir hayalete dokunup dokunmad���n� test etmek
        {
            if(player.x == ghosts[i].x && player.y == ghosts[i].y)
            {
                if(!is_powerful)
                {

                    for(j=0; j<qtGhosts; j++)                   //Pacman'in bir hayalete dokunup dokunmad���n� test etmek
                    {
                        gotoxy((ghosts[j].x+1),(ghosts[j].y+1));
                        printf("%c", ' ');
                        ghosts[j].x = ghosts[j].start_x;
                        ghosts[j].y = ghosts[j].start_y;
                    }
                    lives--;
                    Update_Lives(&lives);
                    if(lives > 0)
                    Restart_Game(&player.start_x,&player.start_y,&player.x,&player.y,&mov,&lives);
                }
                else
                {
                    ghosts[i].x = ghosts[i].start_x;
                    ghosts[i].y = ghosts[i].start_y;
                    map[player.y][player.x] = 32;
                    Update_Score(&player_score, 200);
                }
            }
        }

        if(map[player.y][player.x] == PILL)         //Pacman'in bir tablet al�p almad���n� test etmek
        {
            map[player.y][player.x] = 32;
            Update_Score(&player_score, 1);
        }
        if(map[player.y][player.x] == SUPER_PILL)   //Pacman'in herhangi bir s�per tablet yakalay�p yakalamad���n� test etme
        {
            map[player.y][player.x] = 32;
            flag_start_power = 1;                   //yakalan�rsa g�� kazan�r
        }

        win = Verify_Win(map);                      //Haritada hala tablet olup olmad���n� kontrol etmek:


        time_dif = clock() - start;                 //program ba�lang�c�ndan mevcut duruma kadar zaman fark�
        msec = time_dif * 1000 / CLOCKS_PER_SEC;    //milesimos sayac�

        can_move_pacman = 0;                        //pacman izin bayra��
        timer_pacman = msec%(1000-pacman_speed);    //pacman zamanlay�c� her x milisaniyede bir s�f�rlan�yor
        if(last_timer_pacman > timer_pacman)        //zamanlay�c� s�f�rlan�rsa
            can_move_pacman = 1;                    //ta��nmas�na izin ver
        last_timer_pacman = timer_pacman;           //son zamanlay�c�y� kaydet

        can_move_ghost = 0;                         //hayalet izin bayra��
        timer_ghost = msec%(1000-ghosts_speed);     //her x milisaniyede bir s�f�rlanan hayalet zamanlay�c�
        if(last_timer_ghost> timer_ghost)           //zamanlay�c� s�f�rlan�rsa
            can_move_ghost = 1;                     //ta��nmas�na izin ver
        last_timer_ghost = timer_ghost;             //son zamanlay�c�y� kaydet

        game_timer = msec%1000;                     //saniye oynatma s�resi
        if(last_game_timer > game_timer)            //s�f�r ��kt�,
            gameplay_time++;                        //art�rmak
        last_game_timer = msec%1000;                //son zamanlay�c�y� kaydet

        //maksimum g�� s�resini tan�mlar
        if (flag_start_power==1)
        {
            max_time_power = msec + 10000;           //�imdiki zaman art� 10 saniye
            is_powerful = 1;                        //g�� vermek
            flag_start_power = 0;                   //bayra�� s�f�rla
        }

        if (msec > max_time_power)                  //oyun s�resi g�c�n maksimumunu ge�erse,
            is_powerful = 0;                        // g�c� kaybeder
        
    	

		
    }


    //oyunu bitirmek
    End_Game(&player_score, &lives);
}
