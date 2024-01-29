#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

/* reads from keypress, doesn't echo */
int getch(void)
{
    struct termios oldattr, newattr;
    int ch;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    return ch;
}

/* reads from keypress, echoes */
int getche(void)
{
    struct termios oldattr, newattr;
    int ch;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    return ch;
}

void gotoxy(int x,int y)
{
    printf("%c[%d;%df",0x1B,y,x);
}

void clrscr(){
    system("clear");
}

int powi(int a, int b){
    int i, p = 1;
    for (i = 0; i < b; i++)
        p*=a;
    return p;
}

int abs(int a){
    if (a < 0)
        return -a;
    return a;
}
