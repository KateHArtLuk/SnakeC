#include <ctime> 
#include <iostream> 
#include <list>
#include <windows.h> 
using namespace std;
const int width=50; 
const int height=25; 
const char* DOT="*";
const char* SPACE=" ";
const COORD run_up ={0, -1}; 
const COORD run_down ={0, 1};
const COORD run_left ={-1, 0};
const COORD run_right ={1, 0};
const DWORD DELAY_TIME=166; 

typedef COORD cell; 
typedef list<cell> tSnake; 

tSnake snake; 
HANDLE hStdout;
COORD dir=run_right;
cell apple;
COORD pos={0,0};
void put_cell(cell place, const char* symbol){
    DWORD written;
    SetConsoleCursorPosition(hStdout, place);
    WriteConsole(hStdout, symbol, 1, &written, NULL);
}
void init_window(){
    system("mode con cols=50 lines=25");
    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
}
void init_snake(){
    for (pos.X=pos.Y=0; pos.X<3; pos.X++)
    snake.push_front(pos);
}
void redraw(){
    for(tSnake::iterator it=snake.begin(); it!=snake.end(); it++)
    put_cell(*it, DOT);
}
void resetApple(){
    bool run=true;
    while(run){
        run = false;
        apple.X=rand()%(width-3)+1;
        apple.Y=rand()%(height-3)+1;
        for(tSnake::iterator it=snake.begin(); it!=snake.end() && (!run); it++)
        run=(it->X==apple.X && it->Y==apple.Y);
    }
}
bool step_and_die(){
    bool hit, eat;
    pos=snake.back();
    snake.pop_back();
    if(snake.back().X!=pos.X || snake.back().Y!=pos.Y ) put_cell(pos, SPACE);
    pos=snake.front();
    pos.X+=dir.X;
    pos.Y+=dir.Y;
    if (pos.X==-1) pos.X=width-1;
    if (pos.X==width) pos.X=0;
    if (pos.Y==-1) pos.Y=height-1;
    if (pos.Y==height) pos.Y=0;
    hit=eat=false;
    for (tSnake::iterator it=snake.begin(); it!=snake.end() && (!hit) && (!eat); it++){
        hit=(it->X==pos.X && it->Y==pos.Y);
        eat=(it->X==apple.X && it->Y==apple.Y);
    }
    snake.push_front(pos);
    put_cell(pos,DOT);
    if (eat){
        resetApple();
        snake.push_front(pos);
    }
    return hit;
}
void finish(){
    pos.X=width/2-5;
    pos.Y=height/2;
    put_cell(pos, SPACE);
    cout<<"GAME OVER!";
    system("pause");
}
void HandleKeyboard(){
    SHORT keys[4];
    int i, key;
    for (i=0, key=VK_LEFT; i<4; i++, key++)
    keys[i]=GetAsyncKeyState(key);

    if (dir.Y==0){
    if (keys[VK_UP -VK_LEFT]) dir=run_up;
    if (keys[VK_DOWN-VK_LEFT]) dir=run_down;}
    else{
       if (keys[VK_LEFT -VK_LEFT]) dir=run_left;
       if (keys[VK_RIGHT-VK_LEFT]) dir=run_right; 
    }
}
int main(){
    srand(time(NULL));
    init_window();
    init_snake();
    resetApple();
    redraw();
    const char* anim[]={DOT, SPACE, DOT};
    while(1){
        for (int j=0; j<3; j++){
            put_cell(apple, anim[j&1]);
            Sleep(DELAY_TIME);
        }

        HandleKeyboard();
        if (step_and_die()||GetAsyncKeyState(VK_ESCAPE))break;//step_and_die()
    }
    finish();
    return 0;
}