#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <fstream>
#include <math.h>

#define  BLACK   30
#define  RED     31
#define  GREEN   32
#define  YELLOW  33
#define  BLUE    34
#define  MAGENTA 35
#define  CYAN    36
#define  WHITE   37

#define  B_BLACK   40
#define  B_RED     41
#define  B_GREEN   42
#define  B_YELLOW  43
#define  B_BLUE    44
#define  B_MAGENTA 45
#define  B_CYAN    46
#define  B_WHITE   47

#define PRINT(color,format,...)\
do{\
  printf("\033[40;");\
  printf(color);\
  printf("m");\
  printf(format, ##__VA_ARGS__);\
  printf("\033[0m");\
}while(0)

#define SETBACKGROUD(color) \
do{\
    printf("\033[%dm \033[0m",(color));\
    printf("\033[1D");\
}while(0)

// 清除屏幕

#define CLEAR() printf("\033[2J")

// 上移光标

#define MOVEUP(x) printf("\033[%dA", (x))

// 下移光标

#define MOVEDOWN(x) printf("\033[%dB", (x))

// 左移光标

#define MOVELEFT(y) printf("\033[%dD", (y))

// 右移光标

#define MOVERIGHT(y) printf("\033[%dC",(y))

// 定位光标

#define MOVETO(x,y) printf("\033[%d;%dH", (x), (y))

// 光标复位

#define RESET_CURSOR() printf("\033[H")

#define SAVE_CURSOR() printf("\033[s");

#define BACK_CURSOR() printf("\033[u");

// 隐藏光标

#define HIDE_CURSOR() printf("\033[?25l")

// 显示光标

#define SHOW_CURSOR() printf("\033[?25h")

//反显

#define HIGHT_LIGHT() printf("\033[7m")

#define UN_HIGHT_LIGHT() printf("\033[27m")

struct current_cmd
{
    float x;
    float y;
    union
    {
        float height;
        float distance;
    };
    float theta;
    int res;
    int cmd;
};

void outInstruction(std::ofstream &out, const current_cmd &cmd, int line);

float getValue(void)
{
    std::string str = "0";
    char ch;
    while((ch = getchar ()) != EOF)
    {
        if(ch == '\n')
        {
            break;
        }
        else if (ch == 0x08 || ch == 127)
        {
            str.erase (str.length () - 1 );
        }
        else
        {
            str += ch;
        }
        printf ("%c", ch);
    }
    return atof (str.c_str ());
}

int main() {
    CLEAR();
    MOVETO(61,0);
    std::cout << "Enter initial (x y theta): ";
    struct current_cmd cmd ={};
    cmd.res = 1;
    std::cin >> cmd.x;
    std::cin >> cmd.y;
    std::cin >> cmd.theta;
    CLEAR();
    MOVETO(61,0);
    printf("Current Position: %6.1f %6.1f %7.4f", cmd.x, cmd.y, cmd.theta);
    SAVE_CURSOR();
    MOVETO(30,100);
    SETBACKGROUD(B_GREEN);

    int  ch;
    struct termios old,raw;
    tcgetattr (0, &raw);
    old = raw;
    raw.c_lflag &= ~(ICANON|ECHO);
    tcsetattr (0,TCSANOW, &raw);
   // SHOW_CURSOR();
    HIDE_CURSOR();

    std::ofstream out ("./cmd.txt");
    out.is_open ();
//    out<<"this is a test\n";


    getchar ();
    float action_theta = cmd.theta;
   while ((ch = getchar ()) != EOF)
    {
        switch (ch)
        {
            case 'W':
            case 'w':
                SETBACKGROUD(B_GREEN);
                MOVEUP(1);
                SETBACKGROUD(B_YELLOW);
                if(fabs (action_theta - 1.5707)>0.01f)
                {
                    cmd.cmd = 2;
                    action_theta = 1.5707;
                    if(fabs (cmd.theta - 3.1416) < 0.01 || fabs (cmd.theta) <0.01)
                    {
                        outInstruction (out, cmd, __LINE__);
                        cmd.theta = action_theta;
                    }
                    outInstruction (out, cmd, __LINE__);
                }
                cmd.y += 1;
                break;
            case 'a':
            case 'A':
                SETBACKGROUD(B_GREEN);
                MOVELEFT (1);
                SETBACKGROUD(B_YELLOW);
                if(fabs (action_theta - 3.1416) >0.01f)
                {
                    cmd.cmd = 2;
                    action_theta = 3.1416;
                    if(fabs (cmd.theta - 1.5707) < 0.01 || fabs (cmd.theta + 1.5707) <0.01)
                    {
                        outInstruction (out, cmd, __LINE__);
                        cmd.theta = action_theta;
                    }
                    outInstruction (out,cmd, __LINE__);
                }
                cmd.x -= 1;
                break;
            case 'S':
            case 's':
                SETBACKGROUD(B_GREEN);
                MOVEDOWN (1);
                SETBACKGROUD(B_YELLOW);
                if(fabs(action_theta + 1.5707) > 0.01f)
                {
                    cmd.cmd = 2;
                    action_theta = -1.5707;
                    if(fabs (cmd.theta - 3.1416) < 0.01 || fabs (cmd.theta) <0.01)
                    {
                        outInstruction (out, cmd, __LINE__);
                        cmd.theta = action_theta;
                    }
                    outInstruction (out, cmd, __LINE__);
                }
                cmd.y -= 1;
                break;
            case 'd':
            case 'D':
                SETBACKGROUD(B_GREEN);
                MOVERIGHT (1);
                SETBACKGROUD(B_YELLOW);
                if(fabs (action_theta) > 0.01f)
                {
                    cmd.cmd = 2;
                    action_theta = 0;
                    if(fabs (cmd.theta - 1.5707) < 0.01 || fabs (cmd.theta + 1.5707) <0.01)
                    {
                        outInstruction (out, cmd, __LINE__);
                        cmd.theta = action_theta;
                    }
                    outInstruction (out, cmd, __LINE__);
                    SAVE_CURSOR();
                }
                cmd.x += 1;
                break;
            case 'L':
            case 'l':
                SETBACKGROUD (B_CYAN);
                SAVE_CURSOR();
                MOVETO(61,41);
                std::cout<<"Height = ";
                cmd.height = getValue ();
                outInstruction (out, cmd, __LINE__);
                cmd.cmd = 3;
                outInstruction (out, cmd, __LINE__);
                cmd.cmd = 2;
                MOVETO(61,41);
                std::cout<<"              ";
                BACK_CURSOR ();
                break;
            case 'I':
            case 'i':
                SETBACKGROUD (B_WHITE);
                SAVE_CURSOR();
                MOVETO(61,41);
                std::cout<<"Distance = ";
                cmd.distance = getValue ();
                outInstruction (out, cmd, __LINE__);
                cmd.cmd = 5;
                outInstruction (out, cmd, __LINE__);
                cmd.cmd = 2;
                MOVETO(61,41);
                std::cout<<"                   ";
                BACK_CURSOR ();
                break;
            case 'O':
            case 'o':
                SETBACKGROUD (B_MAGENTA);
                SAVE_CURSOR();
                MOVETO(61,41);
                std::cout<<"Distance = ";
                cmd.distance = getValue ();
                outInstruction (out, cmd, __LINE__);
                cmd.cmd = 6;
                outInstruction (out, cmd, __LINE__);
                cmd.cmd = 2;
                MOVETO(61,41);
                std::cout<<"                   ";
                BACK_CURSOR ();
                break;
            case 'q':
                //CLEAR();
                MOVETO (61,0);
                printf ("                                                 \r");
                tcsetattr (0,TCSANOW, &old);
                outInstruction (out, cmd, __LINE__);
                out.close ();
                SHOW_CURSOR();
                return 0;
        }
        SAVE_CURSOR();
        MOVETO(61,0);
        printf("Current Position: %6.1f %6.1f %7.4f",cmd.x, cmd.y, cmd.theta);
        BACK_CURSOR();
    }
    tcsetattr (0,TCSANOW, &old);
    return 0;
}


void outInstruction(std::ofstream &out, const current_cmd &cmd, int line) {
    char buffer[100];
    sprintf (buffer,"%6.1f %6.1f %6.1f %7.4f %d %d",cmd.x,cmd.y,cmd.distance,cmd.theta,cmd.res,cmd.cmd);
    out << buffer <<"\n";
    SAVE_CURSOR();
    MOVETO(61,70);
    BACK_CURSOR ();
}