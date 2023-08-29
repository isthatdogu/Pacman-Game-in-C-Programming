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

//FONKSÝYONLAR:

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
    //Ýmleci ekrandaki bir yere hareket ettirir
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
    //Dosyadan matrisi alýr

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
    //Labirenti ekranda boyayýn


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
    //Puanla ilgili güncelleme

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
    //Oyunu yeniden baþlatýr

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
    //Pacman'i oyuncunun gitmek istediði yere göre hareket ettirir

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

//Deðiþiklik!
void Move_Ghost(char map[][COLUMN], int *ghostX, int *ghostY, int *pacX, int *pacY, int *super, char* last_move)
{
    int flag_same_direction = 1, flag_stopped = 1;      //bayraklar yön deðiþtirmeyecek veya hareketsiz duracak
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
    if (rand()%100 < difficulty)                                  //Hayaletin pacman'ý takip etme þansý deðiþken
    {
        if(module(difX) > module(difY))                 //X ekseninin mesafe modülü Y'ninkinden büyükse,
            //X'de takip etmeye çalýþýyor
        {
            if(difX > 0)                                //hayaletin X'deki mesafesi 0'dan büyükse
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
                    flag_stopped = 0;                   //bayrak hayaletin hareket ettiðini doðrular

                }

            }
            else                                        //hayaletin X'deki mesafesi 0'dan küçükse
            {
                //saða gitmeyi dene
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
                    flag_stopped = 0;                   //bayrak hayaletin hareket ettiðini doðrular

                }

            }
        }
        else                                            //Y ekseninin mesafe modülü X'inkinden büyükse,
            //Y'yi takip etmeye çalýþýyor
        {
            if(difY > 0)                                //hayaletin Y'deki mesafesi 0'dan büyükse
                //yukarý çýkmayý dene
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
                    flag_stopped = 0;                   //bayrak hayaletin hareket ettiðini doðrular

                }

            }
            else                                        //hayaletin Y'deki mesafesi 0'dan az ise
                //aþaðý inmeyi dene
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
                    flag_stopped = 0;                   //bayrak hayaletin hareket ettiðini doðrular

                }

            }
        }
    }
    //hayalet hareket etmediyse flag_stopped sýfýrlanacak,
    //bu, hayaletin pacman'ý takip edemeyeceði anlamýna gelir çünkü engelleyen bir duvar vardýr.
    //bu durumda hayalet rastgele bir karakter kazanýr ve mümkün olan her yere gider.
    //hareket edebilir.
    //RANDOMÝK BÖLÜM:
    if (flag_stopped == 1)
    {
        do
        {
            switch(rand()%4)
            {
            case 0: //YUKARI gitmeyi dene
                //yukarý bir duvar deðilse ve son hareket aþaðý deðilse
                if(map[*ghostY - 1][*ghostX] != WALL && ((*last_move != 'x')||
                        (
                            (map[*ghostY + 1][*ghostX] == WALL) && //Hayalet öldüyse, burada test edildiði gibi
                            (map[*ghostY][*ghostX + 1] == WALL) && //program "geriye doðru yürüyemeyeceðini" yok sayýyor
                            (map[*ghostY][*ghostX - 1] == WALL)    //bu þekilde hayalet yoldan çýkýþ yolu olmadan kaçar
                        )                                          //ayný þey diðer 3 taraf için de yapýlýr
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
                    flag_same_direction = 0;                        //hayaletin rastgele hareket ettiðini doðrular
                    break;
                }
                else

                case 1: //SAÐA gitmeyi dene
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
                    flag_same_direction = 0;                        //hayaletin rastgele hareket ettiðini bulur
                    break;
                }
                else

                case 2: //Aþaðý gitmeyi dene
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
                    flag_same_direction = 0;                        //hayaletin rastgele hareket ettiðini bulur
                    break;
                }
                else

                case 3: //Sola gitme teþebbüsü
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
                    flag_same_direction = 0;                        //hayaletin rastgele hareket ettiðini bulur
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
	
    //Deðiþkenler:
    char map[LINE][COLUMN];
    int i,j;
    int win =               0;
    int lives =             3;
    int qtGhosts =          Find_Ghost_Qtd(map);

    //hareket deðiþkenleri
    int mov, last_mov;
    int pacman_speed =      800;
    int ghosts_speed;
    int is_paused =        0;
    int is_powerful =       0;

    //kullanýcýya veri gösteren deðiþkenler
    int gameplay_time =     0;
    long player_score =     0;

    //hareket zamanlayýcý deðiþkenleri
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

    //Labirent gösteriliyor:
    Print_Map(map);

    //Skor gösteriliyor:
    Update_Score(&player_score, 0);

    //Gösterilen zaman:
    Update_Time(&gameplay_time);

    //Hayatlarý gösteren:
    Update_Lives(&lives);

    //Oyuna baþla:
    while(!win && lives > 0)
    {

        //Pacman çalýþtýrýldýðýnda veya olmadýðýnda hayaletler hýzýný ayarlar
        if(is_powerful)ghosts_speed=pacman_speed*0.7;
        else ghosts_speed = pacman_speed;

        Update_Time(&gameplay_time);
        //Oyuncu giriþi bekleniyor:
        if(kbhit())
        {
            while(kbhit())
            {
                last_mov = mov;
                mov = getch();
            }
            mov = tolower(mov);

            //Pacman'ý durdurmamak için oyuncunun giriþini "doðrulama"
            //oyuncu harekete paralel yönü seçerse
            //duvarýn olduðu yerde
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

        //Pacman'ý hareket ettirmek:
        if(can_move_pacman && !is_paused)           //zamanlayýcý harekete izin veriyorsa
        {
            can_move_pacman = 0;                    //izni geri çekmek
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


        for(i=0; i<qtGhosts; i++)                   //Pacman'in bir hayalete dokunup dokunmadýðýný test etmek
        {
            if(player.x == ghosts[i].x && player.y == ghosts[i].y)
            {
                if(!is_powerful)
                {
                    for(j=0; j<qtGhosts; j++)                   //Pacman'in bir hayalete dokunup dokunmadýðýný test etmek
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
		
        
        if(can_move_ghost && !is_paused)            //zamanlayýcý harekete izin veriyorsa
        {
            can_move_ghost = 0;                     //izni geri çekmek
            for(i=0; i<qtGhosts; i++)
            {
                Move_Ghost(map,&ghosts[i].x,&ghosts[i].y, &player.x, &player.y, &is_powerful, &ghosts[i].last_move);
            }
        }
	}

        for(i=0; i<qtGhosts; i++)                   //Pacman'in bir hayalete dokunup dokunmadýðýný test etmek
        {
            if(player.x == ghosts[i].x && player.y == ghosts[i].y)
            {
                if(!is_powerful)
                {

                    for(j=0; j<qtGhosts; j++)                   //Pacman'in bir hayalete dokunup dokunmadýðýný test etmek
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

        if(map[player.y][player.x] == PILL)         //Pacman'in bir tablet alýp almadýðýný test etmek
        {
            map[player.y][player.x] = 32;
            Update_Score(&player_score, 1);
        }
        if(map[player.y][player.x] == SUPER_PILL)   //Pacman'in herhangi bir süper tablet yakalayýp yakalamadýðýný test etme
        {
            map[player.y][player.x] = 32;
            flag_start_power = 1;                   //yakalanýrsa güç kazanýr
        }

        win = Verify_Win(map);                      //Haritada hala tablet olup olmadýðýný kontrol etmek:


        time_dif = clock() - start;                 //program baþlangýcýndan mevcut duruma kadar zaman farký
        msec = time_dif * 1000 / CLOCKS_PER_SEC;    //milesimos sayacý

        can_move_pacman = 0;                        //pacman izin bayraðý
        timer_pacman = msec%(1000-pacman_speed);    //pacman zamanlayýcý her x milisaniyede bir sýfýrlanýyor
        if(last_timer_pacman > timer_pacman)        //zamanlayýcý sýfýrlanýrsa
            can_move_pacman = 1;                    //taþýnmasýna izin ver
        last_timer_pacman = timer_pacman;           //son zamanlayýcýyý kaydet

        can_move_ghost = 0;                         //hayalet izin bayraðý
        timer_ghost = msec%(1000-ghosts_speed);     //her x milisaniyede bir sýfýrlanan hayalet zamanlayýcý
        if(last_timer_ghost> timer_ghost)           //zamanlayýcý sýfýrlanýrsa
            can_move_ghost = 1;                     //taþýnmasýna izin ver
        last_timer_ghost = timer_ghost;             //son zamanlayýcýyý kaydet

        game_timer = msec%1000;                     //saniye oynatma süresi
        if(last_game_timer > game_timer)            //sýfýr çýktý,
            gameplay_time++;                        //artýrmak
        last_game_timer = msec%1000;                //son zamanlayýcýyý kaydet

        //maksimum güç süresini tanýmlar
        if (flag_start_power==1)
        {
            max_time_power = msec + 10000;           //þimdiki zaman artý 10 saniye
            is_powerful = 1;                        //güç vermek
            flag_start_power = 0;                   //bayraðý sýfýrla
        }

        if (msec > max_time_power)                  //oyun süresi gücün maksimumunu geçerse,
            is_powerful = 0;                        // gücü kaybeder
        
    	

		
    }


    //oyunu bitirmek
    End_Game(&player_score, &lives);
}
