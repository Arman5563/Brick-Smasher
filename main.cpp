#include <iostream>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL2_gfx.h>
#include <string>
#include <fstream>
#include <sstream>
#include <ctime>
#include <cmath>
#include <bits/stdc++.h>



using namespace std;

const int WINDOW_W = 1200;
const int WINDOW_H = 800;
const int LAYER_SIZE = 8;
const int BLOCK_W = 200;
const int BLOCK_H = 100;
const int BLOCK_D = 40;

const double PI = acos(-1);
double distance(int a, int b, int c, int d) {
    return sqrt((a - c) * (a - c) + (b - d) * (b - d));
}
double deg_to_rad(double degree) {
    return degree * PI / 180;
}

struct Brick {
    int x, y;
    short int R, G, B, A;
    int strength, width, height;

    Brick(int x, int y, int strength, int R = 0, int G = 255, int B = 0) {
        this->x = x, this->y = y, this->strength = strength;
        this->R = R, this->G = G, this->B = B;
        width = BLOCK_W, height = BLOCK_H;
    }
    int decrease(int damage) {
        damage = min(strength, damage);
        strength -= damage;
        return damage;
    }
    void go_down() {
        y += BLOCK_H + BLOCK_D;
    }
};

struct Player {
    int x, y;
    short int R, G, B, A;
    int width, height, dx;
    int level, score, lives;

    Player(int R = 255, int G = 0, int B = 0) {
        level = 1, score = 0, lives = 3;
        this->R = R, this->G = G, this->B = B;
        width = BLOCK_W, height = BLOCK_H, dx = 5;
        x = (WINDOW_W - width) / 2, y = WINDOW_H - height - BLOCK_D;
    }
    void add_score() {
        score += level;
    }
    void move_left() {
        x = max(x - dx, 0);
    }
    void move_right() {
        x = min(x + dx, WINDOW_W - width);
    }
};

struct Ball {
    float x, y;
    short int R, G, B, A;
    bool is_on_board;
    float dx, dy;
    int radius, damage;

    Ball(int x, int y, int R = 0, int G = 0, int B = 255) {
        this->R = R, this->G = G, this->B = B;
        dx = rand() % 399 + 1, dy = 400 - dx;
        dx = sqrt(dx), dy = sqrt(dy);
        this->x = x, this->y = y;
        is_on_board = true;
    }
    void decrease(int strength) {
        damage -= strength;
    }
    bool hit_to_brick(Player &player, Brick &brick) {
        bool is_hit = false;
        if (brick.x <= x && x <= brick.x + brick.width)
            is_hit |= (y == brick.y - radius || y == brick.y);
        if (brick.y <= y && y <= brick.y + brick.height)
            is_hit |= (x == brick.x - radius || x == brick.x);
        for (int dx: {0, brick.width})
            for (int dy: {0, brick.height})
                is_hit |= (distance(x, y, brick.x + dx, brick.y + dy) <= radius);
        if (!is_hit)
            return false;
        decrease(brick.decrease(damage));
        bool new_level = !damage;
        if (!brick.strength)
            player.add_score();
        if (new_level)
            damage = ++player.level;
        double temp = radius * cos(deg_to_rad(45));
        if (brick.x - temp <= x && x <= brick.x + brick.width + temp)
            dy *= -1;
        if (brick.y - temp <= y && y <= brick.y + brick.height + temp)
            dx *= -1;
        return new_level;
    }
    void hit_to_wall() {
        if (x == radius || x == WINDOW_W - radius)
            dx *= -1;
        if (y == radius || y == WINDOW_H - radius)
            dy *= -1;
    }
    void move_forward() {
        x += dx;
        x = max(x, float(radius));
        x = min(x, float(WINDOW_W - radius));
        y += dy;
        y = max(y, float(radius));
        y = min(y, float(WINDOW_W - radius));
    }
    void hit_to_buttom(Player &player) {
        if (y == player.y - radius && (player.x < x && x < player.x + player.width))
            dy *= -1;
        else if (y >= WINDOW_H - radius) {
            player.lives--;
            is_on_board = true;
        }
    }
};
Player player;
Ball ball(player.x, player.y);


//void new_brick_layer() {
//    for (Bricks &brick: bricks)
//        brick.go_down();
//    for (int i = 0; i < LAYER_SIZE; i++)
//        bricks.push_back(Brick(i * (BLOCK_W + BLOCK_D), 0, player.level));
//}

void launch_game() {
    while (true) {
        if (ball.is_on_board) {
            ball.x = player.x + player.width / 2;
            ball.y = player.y - ball.radius;
        }
        else {
            ball.move_forward();
            ball.hit_to_wall();
            ball.hit_to_buttom(player);
        }
        // Draw the everything
        SDL_Event event;
        if (SDL_PollEvent(&event))
            if (event.type == SDLK_RIGHT || event.type == SDLK_d)
                player.move_right();
            else if (event.type == SDLK_LEFT || event.type == SDLK_a)
                player.move_left();
            else if (event.type == SDLK_SPACE && ball.is_on_board)
                ball.is_on_board = false;
            else if (event.type == SDLK_ESCAPE)
                break;
            else if (event.type == SDLK_p)
                while (true) {
                    SDL_Event e;
                    if (SDL_PollEvent(&e) && e.type == SDLK_p)
                        break;
                }
    }
}
struct Balls
{
    float x;
    float car_y;
    float vx;
    float vy;
    short int R;
    short int G;
    short int B;
    short int A;
    int radius;
    int damage;
};
struct Bricks
{
    int x;
    int cartez_y;
    int strength;
    char type;
    int width;
    int height;
    short int R;
    short int G;
    short int B;
    short int A;
};

void rect1(SDL_Renderer *m_renderer, SDL_Texture *Texture, Brick &brick, SDL_Texture *Image );
void rect2(SDL_Renderer *m_renderer, Brick &brick, SDL_Texture *Image );
void random(int ini, int fin, Ball &ball);
void Level(SDL_Renderer *m_renderer, SDL_Texture *Texture, Brick brick[][8]);
void newlevel(SDL_Renderer *m_renderer, SDL_Texture *Texture, Brick brick[][8]);
void downshift(Brick brick[9][8]);
void alamrect(SDL_Renderer *Renderer, int x,int y,int w,int h,int R, int G, int B, int fill_boolian );
void clearrect(SDL_Renderer *m_renderer, SDL_Texture *Texture, Brick &brick, SDL_Texture *backg);
void save_to_file(Brick my_bricks[][8], Brick plate, Ball ball, string filename);
void everybrick(SDL_Rect &backg, SDL_Renderer* m_renderer, SDL_Texture* m_brick, SDL_Texture* m_back_picture, Brick my_bricks[][9]);
int H=790,W=1350,level=1, cnt=0,score, Dx, lx, Dy, ly, I, J, corner_l=3;
short int diff;

//vector<Brick> bricks;

int main( int argc, char * argv[] )
{
    srand(time(0));
    Bricks plate;
    Balls ball;
    Brick my_bricks[9][8];

    const char* filename="list";
    bool quit=0,Q=0;
    plate.strength=1;
    //file and loading begins
    string linename="",Name;
    fstream names_list;
    names_list.open(filename, ios::in | ios::out | ios::app);
    cout<<"Enter existing name"<<endl;
    getline(cin,Name);
    while ((getline(names_list, linename))&&(!quit))
    {
        if (linename==Name)
            quit=1;
    }
    short int lives,score,plevel,load_num,v;
    bool canload=false;
    if (quit)
    {
        short int cnt;
        fstream scores;
        scores.open(Name+"_scores", ios::in);
        scores>>cnt;
        bool noend[cnt]={};
        for (v=0;v<cnt;v++)
        {
            scores>>lives>>score>>plevel;
            if (lives)
            {
                noend[v]=true;
                cout<<"Lives: "<<lives<<" ";
                canload=true;
                //Player.level=plevel;
            }
            cout<<"Score: "<<score<<" "<<"Level: "<<plevel<<endl;
        }
        cout<<"You can only load these games"<<" ";
        for (v=0;v<cnt;v++)
        {
            if (noend[v])
                cout<<v+1<<" ";
        }
        cout<<endl<<"Enter game number for loading. For creating a new game, enter 0"<<endl;
        cin>>load_num;
        if (load_num==0)
            quit=true;
        else if (canload)
        {
            while (!noend[load_num-1])
                cout<<"Invalid number. Enter the game number again";
        }
        else
        {
            cout<<"No available game"<<endl;
            quit=false;
        }
    }
    else
    {
        cout<<"Name not found"<<endl<<"Enter a name"<<endl;
        getline(cin,Name);
        names_list<<Name<<endl;
    }
    names_list.close();
    if (quit)
    {
        Name=Name+char(v+1+int('0'));
        ifstream data;
        data.open(Name+".txt");
        for (int i=0;i<9;i++)
        {
            for (int j=0;j<8;j++)
            {
                data>>my_bricks[i][j].x>>my_bricks[i][j].y>>my_bricks[i][j].width>>my_bricks[i][j].height>>my_bricks[i][j].strength>>my_bricks[i][j].type>>my_bricks[i][j].R>>my_bricks[i][j].G>>my_bricks[i][j].B>>my_bricks[i][j].A;
            }
        }
        data>>ball.x>>ball.y>>ball.dx>>ball.dy>>ball.radius>>ball.damage>>ball.R>>ball.G>>ball.B>>ball.A;
        data>>plate.x>>plate.R>>plate.G>>plate.B>>plate.A;
        data>>diff;
        data.close();
    }
    if (!quit)
    {

        cout<<"choose difficulty number: Easy:1 Medium:2 Hard:3"<<endl;
        cin>>diff;
        cout<<"choose ball color (R G B A)"<<endl;
        cin>>ball.R>>ball.G>>ball.B>>ball.A;
        while ( (ball.R>255) || (ball.R<1) || (ball.G>255) || (ball.G<1) || (ball.B>255) || (ball.B<1) ||(ball.A>255) || (ball.A<1) )
        {
            cout<<"numbers are out of range. please reenter"<<endl;
            cin>>ball.R>>ball.G>>ball.B>>ball.A;
        }
        for (int i=0;i<3;i++)
        {
            for (int j=0;j<8;j++)
            {
                my_bricks[i][j].x=(j+1)*Dx+j*lx;
                my_bricks[i][j].y=(i+1)*Dy+i*ly;
                my_bricks[i][j].height=ly;
                my_bricks[i][j].width=lx;
                my_bricks[i][j].strength=(rand()%level+1)*diff;
                my_bricks[i][rand()%8].strength=0;
                my_bricks[i][rand()%8].strength=0;
            }
        }
        for (int i=3;i<9;i++)
        {
            for (int j=0;j<8;j++)
            {
                my_bricks[i][j].x=(i+1)*Dx+i*lx;
                my_bricks[i][j].cartez_y=(i+1)*Dy+i*ly;
                my_bricks[i][j].height=ly;
                my_bricks[i][j].width=lx;
                my_bricks[i][j].strength=0;
            }
        }

    }
    //Initialization of SDL windows
    Uint32 SDL_flags = SDL_INIT_VIDEO | SDL_INIT_TIMER ;
    Uint32 WND_flags = SDL_WINDOW_SHOWN ;//SDL_WINDOW_BORDERLESS ;
    SDL_Window * m_window;
    SDL_Renderer * m_renderer;
    SDL_Texture *m_back_picture;
    SDL_Texture *m_background;
    SDL_Texture *temp;
    SDL_Texture *m_brick;
    SDL_Texture *m_platform;
    m_back_picture=SDL_CreateTexture( m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET , H, W );
    m_background=SDL_CreateTexture( m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET , H, W );
    short int backnum=0;
    const char *backname_1="game_background_1.png";
    const char *backname_2="game_background_2.png";
    const char *backname_3="game_background_3.png";
    const char *backname_4="game_background_4.png";
    const char *brickim="brick.png";
    const char *backname;
    cout<<"Enter a number from 1-4 to choose a background"<<endl;
    cin>>backnum;
    while ((backnum!=1)&&(backnum!=2)&&(backnum!=3)&&(backnum!=4))
    {
        cout<<"Invalid number. Enter background number again"<<endl;
        cin>>backnum;
    }
    switch (backnum)
    {
    case 1:
        backname=backname_1;
        break;
    case 2:
        backname=backname_2;
        break;
    case 3:
        backname=backname_3;
        break;
    case 4:
        backname=backname_4;
        break;
    }
    cout<<backname<<endl;

    //Texture for loading image
    int pict_w, pict_h;
    SDL_Rect img_rect;
    SDL_Init( SDL_flags );
    SDL_CreateWindowAndRenderer( W, H, WND_flags, &m_window, &m_renderer );
    //Pass the focus to the drawing window
    SDL_RaiseWindow(m_window);
    //Get screen resolution
    temp=IMG_LoadTexture(m_renderer,backname);
    //m_brick=IMG_LoadTexture(m_renderer,"" );
    SDL_Rect backg;
    backg.x=0;
    backg.y=0;
    backg.h=H;
    backg.w=W;
    //

    //

    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    cout<<W<<" "<<H;
    while (!quit)
    {
        I=int(H-ball.car_y/(Dy+ly));
        J=int(ball.x/(Dx+lx));
        if (ball.x-I*(Dx+lx)>=Dx)
        {
            if (I==0)
            {
                if ((my_bricks[0][J].strength)&&(ball.y+ball.radius==my_bricks[0][J].y))
                {
                    if (ball.x>=my_bricks[0][J].x+lx-corner_l)
                        random(-90,180, ball);
                    else if (ball.x-my_bricks[0][J].x<=corner_l)
                        random(0,270, ball);
                    else
                        ball.dy*=-1;
                }
            }
            else if (I==9)
            {
                if ((my_bricks[8][J].strength)&&(ball.y-ball.radius==my_bricks[8][J].y))
                {
                    if (ball.x>=my_bricks[8][J].x+lx-corner_l)
                        random(-180,90,ball);
                    else if (ball.x-my_bricks[8][J].x<=corner_l)
                        random(-270,0, ball);
                    else
                        ball.dy*=-1;
                }
            }
            else
            {
                if ((my_bricks[I-1][J].strength)&&(ball.y-ball.radius==my_bricks[I-1][J].y))
                {
                    if (ball.x>=my_bricks[I-1][J].x+lx-corner_l)
                        random(-180,90,ball);
                    else if (ball.x-my_bricks[I-1][J].x<=corner_l)
                        random(-270,0, ball);
                    else
                        ball.dy*=-1;
                }
                if ((my_bricks[I][J].strength)&&(ball.y+ball.radius==my_bricks[I][J].y))
                {
                    if (ball.x>=my_bricks[I][J].x+lx-corner_l)
                        random(-90,180,ball);
                    else if (ball.x-my_bricks[I][J].x<=corner_l)
                        random(0,270,ball);
                    else
                        ball.dy*=-1;
                }
            }
        }
        else if (ball.y-I*(Dy+ly)>=Dy)
        {
            if (J==0)
            {
                if ((my_bricks[I][0].strength)&&(ball.x+ball.radius==my_bricks[I][0].x))
                {
                    if (ball.y>=my_bricks[I][J].cartez_y+ly-corner_l)
                        random(-270,0,ball);
                    else if (ball.y-my_bricks[I][0].y<=corner_l)
                        random(0,270,ball);

                    else
                        ball.dx*=-1;
                }
            }
            else if (J==8)
            {
                if ((my_bricks[I][7].strength)&&(ball.x-ball.radius==my_bricks[I][7].x))
                {
                    if (ball.y>=my_bricks[I][7].y+ly-corner_l)
                        random(-180,90,ball);
                    else if (ball.y-(H-my_bricks[I][7].y)<=corner_l)
                        random(-90,180,ball);
                    else
                        ball.dx*=-1;
                }
            }
            else
            {
                if ((my_bricks[I][J-1].strength)&&(ball.x-ball.radius==my_bricks[I][J-1].x))
                {
                    if (ball.y>=my_bricks[I][J-1].y+ly-corner_l)
                         random(-180,90,ball);
                    else if (ball.y-(H-my_bricks[I][J-1].y)<=corner_l)
                        random(-90,180,ball);
                    else
                        ball.dx*=-1;
                }
                if ((my_bricks[I][J].strength)&&(ball.x-ball.radius==my_bricks[I][J].x))
                {
                    if (ball.y>=my_bricks[I][J].y+ly-corner_l)
                        random(-270,0,ball);
                    else if (ball.y-(my_bricks[I][J].y)<=corner_l)
                        random(0,270,ball);
                    else
                        ball.dx*=-1;
                }
            }
        }
        else if ((ball.x>I*(Dx+lx))&&(ball.x<Dx+I*(Dx+lx)))
        {
            if ( (ball.x-J*(Dx+lx))*(ball.x-J*(Dx+lx)) + (ball.car_y-I*(Dy+ly) )*(ball.car_y-I*(Dy+ly) ) == ball.radius * ball.radius ) //alpha
            {
                if ((I)&&(J))
                {
                    if (my_bricks[I-1][J-1].strength)
                        random(-180,90,ball);
                }
            }
            if ( (ball.x-J*(Dx+lx)-Dx)*(ball.x-J*(Dx+lx)-Dx) + (ball.car_y-I*(Dy+ly) )*(ball.car_y-I*(Dy+ly) ) == ball.radius * ball.radius ) //beta
            {
                if ((J!=8)&&(I))
                {
                    if (my_bricks[I-1][J].strength)
                        random(-270,0,ball);
                }
            }

            if ( (ball.x-I*(Dx+lx))*(ball.x-I*(Dx+lx)) + (ball.car_y-I*(Dy+ly)-Dy )*(ball.car_y-I*(Dy+ly)-Dy ) == ball.radius * ball.radius ) //gamma
            {
                if ((I!=9)&&(J))
                {
                    if (my_bricks[I][J-1].strength)
                        random(-90,180,ball);
                }
            }
            if ( (ball.x-J*(Dx+lx)-Dx)*(ball.x-J*(Dx+lx)-Dx) + (ball.car_y-I*(Dy+ly)-Dy )*(ball.car_y-I*(Dy+ly)-Dy ) == ball.radius * ball.radius ) //phi
            {
                if ((I!=9)&&(J!=8))
                {
                    if (my_bricks[I][J].strength)
                        random(-270,0,ball);
                }
            }
        }

    }

//    for (int i=0;i<9;i++)
//    {
//        for (int j=0;j<)
//    }

    //Finalize and free resources
    SDL_DestroyRenderer( m_renderer );
	IMG_Quit();
	SDL_Quit();
    return 0;

}
void alamrect(SDL_Renderer *Renderer, int x,int y,int w,int h,int R, int G, int B, int fill_boolian )
{
    SDL_Rect rectangle ;
    rectangle.x = x;
    rectangle.y = y;
    rectangle.w = w;
    rectangle.h = h;


    SDL_SetRenderDrawColor(Renderer, R, G, B, 255);
    if (fill_boolian==1)
        SDL_RenderFillRect(Renderer, &rectangle);
    SDL_RenderDrawRect(Renderer, &rectangle);
    //SDL_RenderPresent(Renderer);

}
void rect1(SDL_Renderer *m_renderer, SDL_Texture *Texture, Brick &brick, SDL_Texture *Image )
{
    SDL_Rect *rectangle=new SDL_Rect;
    rectangle->x = brick.x;
    rectangle->y = H-brick.cartez_y;
    rectangle->w = brick.width;
    rectangle->h = brick.height;
    SDL_SetRenderTarget(m_renderer, Texture);

    SDL_SetRenderDrawColor(m_renderer, int(brick.R), int(brick.G), int(brick.B), int (brick.A));
    if (brick.strength)
    {
        SDL_RenderCopy(m_renderer, Image, NULL, rectangle);
        //SDL_RenderFillRect(m_renderer, rectangle);
    }
    //SDL_RenderDrawRect(Texture, rectangle);
    delete rectangle;
    SDL_SetRenderTarget(m_renderer, NULL);
}
void rect2(SDL_Renderer *m_renderer, Brick &brick, SDL_Texture *Image )
{
    SDL_Rect *rectangle=new SDL_Rect;
    rectangle->x = brick.x;
    rectangle->y = H-brick.cartez_y;
    rectangle->w = brick.width;
    rectangle->h = brick.height;

    SDL_SetRenderDrawColor(m_renderer, int(brick.R), int(brick.G), int(brick.B), int (brick.A));
    if (brick.strength)
    {
        SDL_RenderCopy(m_renderer, Image, NULL, rectangle);
        //SDL_RenderFillRect(m_renderer, rectangle);
    }
    //SDL_RenderDrawRect(Texture, rectangle);
    delete rectangle;
}
void clearrect(SDL_Renderer* m_renderer, SDL_Texture* Texture, Brick &brick, SDL_Texture* backg)
{
    SDL_Rect *rectangle=new SDL_Rect;
    rectangle->x = brick.x;
    rectangle->y = brick.y;
    rectangle->w = brick.width;
    rectangle->h = brick.height;
    SDL_SetRenderTarget(m_renderer, Texture);
    SDL_RenderCopy(m_renderer, backg, rectangle, rectangle);
    delete rectangle;
    SDL_SetRenderTarget(m_renderer, NULL);
}
void random(int ini, int fin, Ball &ball)
{
    float *abs_v=new float;
    int* theta=new int;
    *theta=rand()%(fin-ini)+ini;
    *abs_v=sqrt(ball.dx*ball.dx+ball.dy*ball.dy);
    ball.dx=*abs_v*cos(M_PI/180**theta);
    ball.dy=*abs_v*sin(M_PI/180**theta)*(-1);
    delete abs_v, theta;
}
void downshift(Brick brick[9][8])
{
    int *tempx= new int;
    int *tempy=new int;
    for (int i=1;i<9;i++)
    {
        for (int j=0;j<8;j++)
        {
            *tempx=brick[i][j].x;
            *tempy=brick[i][j].cartez_y;
            brick[i][j]=brick[i-1][j];
            brick[i][j].x=*tempx;
            brick[i][j].cartez_y=*tempy;
        }
    }
    delete tempx, tempy;
}
void newlevel(SDL_Renderer* m_renderer, SDL_Texture* Texture, Brick brick[9][8] )
{
    int *temp1=new int;
    int *temp2=new int;
    *temp1=rand()%8;
    *temp2=rand()%8;
    score+=cnt*level; cnt=0; level++;
    downshift(brick);
    for (int j=0;j<8;j++)
        brick[0][j].strength=(rand()%level+1)*diff;
    brick[0][rand()%8].strength=0;
    brick[0][rand()%8].strength=0;
    delete temp1, temp2;
}
void save_to_file(Bricks my_bricks[][8], Brick plate, Ball ball, string filename)
{
    ofstream save;
    save.open(filename+".txt", ios::trunc);
        for (int i=0;i<9;i++)
        {
            for (int j=0;j<8;j++)
            {
                save<<my_bricks[i][j].x<<" "<<my_bricks[i][j].y<<" "<<my_bricks[i][j].width<<" "<<my_bricks[i][j].height<<" "<<my_bricks[i][j].strength<<" "<<my_bricks[i][j].type<<" "<<my_bricks[i][j].R<<" "<<my_bricks[i][j].G<<" "<<my_bricks[i][j].B<<" "<<my_bricks[i][j].A<<endl;
            }
        }
        save<<" "<<ball.x<<" "<<ball.y<<" "<<ball.dx<<" "<<ball.dy<<" "<<ball.radius<<" "<<ball.damage<<" "<<ball.R<<" "<<ball.G<<" "<<ball.B<<" "<<ball.A<<endl;
        save<<" "<<plate.x<<" "<<plate.R<<" "<<plate.G<<" "<<plate.B<<" "<<plate.A<<endl;
        save<<" "<<diff<<endl;
}
void save_score(string name, int cnt, int score, int lives, int level)
{
    string filename=name+"_scores.txt";
    fstream save;
    if (cnt)
    {
        string a,b="";
        save.open(filename, ios::in);
        save>>cnt;//just to read the first line
        for (int i=0;i<cnt;i++)
        {
            getline(save,a);
            b=b+a;
        }
        save.close();
        save.open(filename, ios::out | ios::trunc);
        save<<cnt+1<<endl<<b;
    }
    else
    {
        save.open(filename, ios::out);
        save<<cnt+1<<endl<<lives<<" "<<score<<" "<<level;
    }
}
void everybrick(SDL_Rect &backg, SDL_Renderer* m_renderer, SDL_Texture* m_brick, SDL_Texture* m_back_picture, Brick my_bricks[][9])
{
    SDL_RenderCopy(m_renderer, m_back_picture, NULL,&backg);
    for (int i=0;i<9;i++)
    {
        for (int j=0;j<8;j++)
        {
            if (my_bricks[i][j].strength)
                rect2(m_renderer, my_bricks[i][j], m_brick);
        }
    }
}
