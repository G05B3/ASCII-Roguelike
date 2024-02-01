#include <stdio.h>
#include <stdlib.h>
#include "title.h"
#include "defs.h"
#include "player.h"
#include "enemy.h"
#include "dungeon.h"

#define SCREEN_WIDTH 158
#define SCREEN_HEIGHT 45
#define OPTIONSWIDTH 100
#define OPTIONSHEIGHT 30

#define CARRIAGERETURN 10

const char option_strings[4][20] = {"Explore", "Edit Character", "Lore / Controls", "Exit"};

void drawTitle(){

    int i;


gotoxy(OPTIONSWIDTH - 18, 14);                                                       
printf("@@@  @@@   @@@@@@   @@@  @@@@@@@    @@@@@@    @@@@@@");   
gotoxy(OPTIONSWIDTH - 18, 15);                                                       
printf("@@@  @@@  @@@@@@@@  @@@  @@@@@@@@  @@@@@@@@  @@@@@@@");   
gotoxy(OPTIONSWIDTH - 18, 16);                                                       
printf("@@!  !@@  @@!  @@@  @@!  @@!  @@@  @@!  @@@  !@@");       
gotoxy(OPTIONSWIDTH - 18, 17);                                                       
printf("!@!  @!!  !@!  @!@  !@!  !@!  @!@  !@!  @!@  !@!");       
gotoxy(OPTIONSWIDTH - 18, 18);                                                       
printf("@!@@!@!   @!@!@!@!  !!@  @!@!!@!   @!@  !@!  !!@@!!");    
gotoxy(OPTIONSWIDTH - 18, 19);                                                       
printf("!!@!!!    !!!@!!!!  !!!  !!@!@!    !@!  !!!   !!@!!!");   
gotoxy(OPTIONSWIDTH - 18, 20);                                                       
printf("!!: :!!   !!:  !!!  !!:  !!: :!!   !!:  !!!       !:!");  
gotoxy(OPTIONSWIDTH - 18, 21);                                                       
printf(":!:  !:!  :!:  !:!  :!:  :!:  !:!  :!:  !:!      !:!");   
gotoxy(OPTIONSWIDTH - 18, 22);                                                       
printf(" ::  :::  ::   :::   ::  ::   :::  ::::: ::  :::: ::");   
gotoxy(OPTIONSWIDTH - 18, 23);                                                       
printf(" :   :::   :   : :  :     :   : :   : :  :   :: : :");
gotoxy(OPTIONSWIDTH - 8, 25);                                                       
printf("The Dungeon at the End of Time");



    for (i = 0; i < 4; i++){
        if (i == 0){
                gotoxy(OPTIONSWIDTH, OPTIONSHEIGHT + 2 * i);
            printf("\033[0;32m%s\033[0m", option_strings[i]);
        }
        else{
            gotoxy(OPTIONSWIDTH, OPTIONSHEIGHT + 2 * i);
            printf("%s", option_strings[i]);        
        }
    }
    gotoxy(OPTIONSWIDTH - 4, OPTIONSHEIGHT + 8);
    printf("(use w/s and enter keys)");
    gotoxy(OPTIONSWIDTH, OPTIONSHEIGHT);

}

int optionSelect(){
    static int current = 0;
    int action;
    action = getch();
    if (action == 'w' || action == 'W'){
        if (current > 0){
            gotoxy(OPTIONSWIDTH, OPTIONSHEIGHT + 2 * current);
            printf("%s", option_strings[current]);
            current--;
            gotoxy(OPTIONSWIDTH, OPTIONSHEIGHT + 2 * current);
            printf("\033[0;32m%s\033[0m", option_strings[current]);
        }
    }    
    if (action == 's' || action == 'S'){
        if (current < 3){
            gotoxy(OPTIONSWIDTH, OPTIONSHEIGHT + 2 * current);
            printf("%s", option_strings[current]);
            current++;
            gotoxy(OPTIONSWIDTH, OPTIONSHEIGHT + 2 * current);
            printf("\033[0;32m%s\033[0m", option_strings[current]);            
        }
    }
    if (action == CARRIAGERETURN){
        return current;
    }
    return -1;
}

void editCharacter(player* p){

}

void execKairos(player* p, weaponLibrary* wl, enemyLibrary* el){
    int opt;

    clrscr();
    drawTitle();

    do{
        opt = optionSelect();    

        if (opt == 2){ // Lore / Controls

        }
        else if (opt == 1){ // Edit Character
            
        }
        else if (opt == 0){ // Main Game
            resetPlayer(p);
            setPlayerDispCoordinates(p, SCREEN_WIDTH + 4, 3);
            exploreDungeon(p, wl, el, SCREEN_WIDTH, SCREEN_HEIGHT, 11, 11, 20);
            getch();

            clrscr();
            drawTitle();
        }

    }while(opt != 3);

}