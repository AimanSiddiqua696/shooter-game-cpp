ww#include <iostream>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <ctime>
#include <algorithm>
using namespace std;
void SetCursorPosition(int x, int y){
    COORD coord;
    coord.X=x;
    coord.Y=y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
const int HEIGHT=10;
const int WIDTH=40;
const char PLAYER='@';
const char ZOMBIE='Z';
const char BULLET='-';
const char EMPTY=' ';
const char WALL='#';
//STRUCTURE FOR OBJECT POSITION
struct entity{
    int row,col;
    entity(int r,int c):row(r),col(c){}
};
//GAME STATE VARIABLES
int playerrow = HEIGHT/2;
int score=0;
int lives=3;
bool gameover=false;
vector<entity> bullets;
vector<entity> zombies;
void drawscreen(){
    SetCursorPosition(0,0);
    char screen[HEIGHT + 2][WIDTH + 2];
    //ADD WALLS AROUND
    //INITIALIZE SCREEN WITH WALLS
    for(int i=0; i<HEIGHT + 2; ++i){
        for(int j=0; j<WIDTH + 2; ++j){
            if(i==0 || i == HEIGHT +1 || j==0 ||j==WIDTH +1)
            screen[i][j]=WALL;
            else screen[i][j]= EMPTY;

        }
    }
    //PLACE PLAYER
    screen[playerrow +1][1]=PLAYER;
    //PLACE BULLETS
    for(auto& b : bullets)
    if(b.col>0 && b.col <WIDTH)
    screen[b.row +1][b.col +1] = BULLET;
    //PLACE ZOMBIE
    for(auto&& z :zombies)
    if(z.col > 0 && z.col < WIDTH)
    screen[z.row + 1][z.col + 1] = ZOMBIE;
    //PRINT SCREEN WITH WALLS
    for(int i=0; i<HEIGHT + 2; ++i){
         for(int j=0; j<WIDTH + 2; ++j)
         cout<< screen[i][j];
         cout<<endl;
    }
    //UI
    cout<<"----------------------------------------"<<endl;
    cout<<"score:"<<score<<" lives:" <<lives<<endl;
    cout<<"use W/S to move, space to shoot,Esc to quit"<<endl;
}
void updatebullets(){
    for(int i=0;i<bullets.size(); ++i)
    bullets[i].col++;
    //REMOVE BULLETS OUT OF BOUNDS
    bullets.erase(remove_if(bullets.begin(),bullets.end(), [](entity& b){
    return b.col>= WIDTH;
    }),bullets.end());
}
void updatezombies(){
    for(int i=0; i< zombies.size(); ++i)
    zombies[i].col--;
    //CHECK IF ANY ZOMBIE REACHED THE PLAYER SIDE 
    for(int i = 0; i < zombies.size(); ++i){
        if(zombies[i].col ==1 && zombies[i].row == playerrow){
            lives--;
            zombies.erase(zombies.begin() + i);
            i--;
            if(lives <=0)
            gameover = true;
        }
    }
}
void checkcollisions(){
    for(int i=0; i< bullets.size(); ++i){
        for(int j=0; j<zombies.size(); ++j){
            if(bullets[i].row == zombies[j].row && bullets[i].col == zombies[j].col){
                score +=10;
                bullets.erase(bullets.begin() + i);
                zombies.erase(zombies.begin() + j);
                i--;
                break;
            }
        }
    }
}
void spawnzombie(){
    int r=rand( )% HEIGHT;
    zombies.push_back(entity(r,WIDTH - 1));
}
void handleinput(){
    if(kbhit()){
        char ch=getch();
        if(ch=='w' || ch == 'W'){
            if(playerrow > 0)playerrow--;
        }
        else if(ch == 's' || ch == 'S'){
            if(playerrow<HEIGHT - 1)playerrow++;
        }
        else if (ch == ' '){
            bullets.push_back(entity(playerrow, 1));//START RIGHT NEXT TO THE PLAYER   
        }
        else if(ch == 27){
            gameover = true;//ESC
        }
    }
}
void gameoverscreen(){
    system("cls");
    cout<<"\n\n\t===== GAMEOVER =====\n";
    cout<<"\tFinal Score:"<<score<<"\n";
    cout<<"\tTHANKS FOR PLAYING AIMAN!\n";
    cout<<"\n\t#Cplusplus #gamedev #studentshowcase\n";
    cout<<"\npress any key to exit...";
    getch();
}
int main(){
    srand(time(0));
    int zombietimer = 0;
    while (!gameover){
        drawscreen();
        handleinput();
        updatebullets();
        updatezombies();
        checkcollisions();
        //SPAN ZOMBIES AFTER EVERY 10 CYCLES
        if(++zombietimer == 10){
            spawnzombie();
            zombietimer = 0;
        }
        Sleep(100);//GAME SPEED
    }
    gameoverscreen();
    return 0;
}