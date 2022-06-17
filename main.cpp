#include<GL/glut.h>
#include<stdio.h>
#include<iostream>
#include<string>
#include<ctype.h>
#include<vector>
#include<math.h>

using namespace std;

//CONSTANTS
const int TypingSpeed = 25;
const int LineCharacterLimit = 100;
const int DialougeXOffset = 150;
const int FirstLineY = 150;
const int SecondLineY = 120;
const int ThirdLineY = 90;
const int SpeakerXPos = 200;
const int SpeakerYPos = 195;

string PlayerName;

//States
enum Scenes {START , DESCRIPTION ,SCENE1 ,SCENE2};

Scenes Scene = START;
vector<string> Description; 
vector<pair<string, string>> Scene1DialougesWithSpeakers;

vector<vector<pair<int, int>>> HouseAndWindows = 
{ 
    { {100,200},{100,500},{200,500}, {200,200} }, //Left Pillar
    { {500,200},{500,500},{600,500}, {600,200} }, //Right Pillar
    { {200,180},{200,450},{500,450}, {500,180} }, //Middle House
    { {75,505},{125,570},{175,570},  {225,505} }, //RoofTop - Left
    { {475,505},{525,570},{575,570}, {625,505} }, //RoofTop - Right
    { {180,455},{300,570},{400,570}, {525,455} }, //RoofTop - Middle
    { {130,375},{130,450},{160,450}, {160,375} }, //Window Left Top
    { {130,250},{130,325},{160,325}, {160,250} }, //Window Left Bottom
    { {530,375},{530,450},{560,450}, {560,375} }, //Window Right Top
    { {530,250},{530,325},{560,325}, {560,250} }, //Window Right Bottom
    { {325,410},{325,485},{375,485}, {375,410} }, //Window Middle
    { {330,190},{330,300},{370,300}, {370,190} }, //Door
    { {145,375},{145,450} }, //WindowRails - left Top vertical
    { {130,425},{160,425} }, //WindowRails -  left Top horizontal
    { {145,250},{145,325} }, //WindowRails -  left Bottom vertical
    { {130,300},{160,300} }, //WindowRails - left Bottom horizontal
    { {545,375},{545,450} }, //WindowRails - Right Top vertical
    { {530,425},{560,425} }, //WindowRails - Right Top horizontal
    { {545,250},{545,325} }, //WindowRails - Right Bottom vertical
    { {530,300},{560,300} }, //WindowRails - Right Bottom horizontal
    { {325,460},{375,460} }, //WindowRails - Middle Horizontal
    { {350,410},{350,485} }, //WindowRails - Middle Vertical
};

int currentDialouge = 0;
int j;
int State = 1;


//------------------------QUEUE CLASS---------------//
class QueueText {
public:
    int front, rear, size;
    unsigned capacity;
    char* array;
};


//----------------QUEUE FUNCTIONS--------------------//
QueueText* queue;

QueueText* createQueue(unsigned capacity)
{
    QueueText* queueText = new QueueText();
    queueText->capacity = capacity;
    queueText->front = queueText->size = 0;

    //queueText->rear = capacity - 1;
    queueText->array = new char[queueText->capacity];
    return queueText;
}



bool isFull(QueueText* queueText)
{
    return (queueText->size == queueText->capacity);
}

bool isEmpty(QueueText* queueText)
{
    return (queueText->size == 0);
}

void Enqueue(QueueText* queueText, char s)
{
    if (isFull(queueText))
    {
        return;
    }

    queueText->rear++;
    queueText->array[queueText->rear] = s;
    queueText->size++;
}

char Dequeue(QueueText* queueText)
{
    if (isEmpty(queueText))
    {
        printf("QUEUE IS EMPTY");
        return INT_MIN;
    }

    char s = queueText->array[queueText->front];
    queueText->front = (queueText->front + 1) % queueText->capacity;
    queueText->size = queueText->size - 1;

    return s;
}

int front(QueueText* queueText)
{
    if (isEmpty(queueText))
    {
        return INT_MIN;
    }

    return queueText->array[queueText->front];
}

int rear(QueueText* queueText)
{
    if (isEmpty(queueText))
    {
        return INT_MIN;
    }

    return queueText->array[queueText->rear];
}

//----------------QUEUE FUNCTIONS END--------------------//

//-------------------DIALOUGE BOX-----------------------//

void DrawDialougeBox()
{
    //OutLine of the DialougeBox
    glLineWidth(2);
    glColor3f(1, 1, 1);
    glBegin(GL_LINE_LOOP);
    glVertex2f(75, 50);
    glVertex2f(75, 200);
    glVertex2f(1200, 200);
    glVertex2f(1200, 50);
    glEnd();

    //Speaker's Box
    glBegin(GL_POLYGON);
    glColor3f(1, 0, 0);
    glVertex2f(120, 200);
    glVertex2f(150, 220);
    glVertex2f(400, 220);
    glVertex2f(430, 200);
    glVertex2f(400, 180);
    glVertex2f(150, 180);
    glEnd();
}


//----------------RENDERING TEXT ON SCREEN--------------//

void DrawDialouge(char* string, int x, int y , int type)
{
    if (type == 1)
    {
        glColor3f(1, 1, 1);
    }
    else if(type == 0)
    {
        glColor3f(0, 0, 0);
    }
    else
    {
        //do nothing
    }

    int len, i;
    glRasterPos2f(x, y);
    len = (int)strlen(string);

    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
    }
}

//----------------RENDERING TEXT ON SCREEN END--------------//

void ClearFrame()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glEnd();
    glFlush();
}

void RecoverFrame()
{
    DrawDialougeBox();
}

bool CheckOverFlow(string &dialouge , string &text)
{
    if (dialouge.length() > 120)
    {
        text = dialouge.substr(0, 120);
        dialouge.erase(0, 120); //erases first 100 characters
        return true;
    }
    else
    {
        text = dialouge;
        return false;
    }
}

void FillQueue(string text)
{
    for (auto x : text)
    {
        Enqueue(queue, x);
    }
}

void RenderSpeaker(string speaker)
{
    char speakerinChar[100] = "";

    for (int i = 0; i < speaker.length(); i++)
    {
        speakerinChar[i] = speaker[i];
    }

    DrawDialouge(speakerinChar, SpeakerXPos, SpeakerYPos, 0);
}

void AnimateText(string dialouge)
{
    char name[400] = "";  
    string text;
    bool OverflowFirstLine = false;
    bool OverflowSecondLine = false;
    char bufferText[400] = "";
    char FirstLine[201];
    char bufferSecondLine[400] = "";
    char SecondLine[201];
    char bufferThirdLine[400] = "";
    int i = 0;

    if (CheckOverFlow(dialouge, text))
    {
        OverflowFirstLine =true;
    }

    FillQueue(text);

    while(!isEmpty(queue))
    {
            //glClear(GL_COLOR_BUFFER_BIT);
            bufferText[i] = Dequeue(queue);
            DrawDialouge(bufferText, DialougeXOffset, FirstLineY ,1);
            Sleep(TypingSpeed);
            i++;
            glEnd();
            glFlush();
    }

    if(OverflowFirstLine)
    {
        //clear old text
        ClearFrame();
        RecoverFrame();
        RenderSpeaker(Scene1DialougesWithSpeakers[currentDialouge].first);

        i = 0;
 
        for (int i = 0; i <= text.length() ; i ++)
        {
            FirstLine[i] = text[i];
        }
      
        if (CheckOverFlow(dialouge, text))
        {
            OverflowSecondLine = true;
        }
       
        FillQueue(text);

        int j = 0;

        Dequeue(queue); //remove extra character

        while (!isEmpty(queue))
        {
            bufferSecondLine[j] = Dequeue(queue);
            DrawDialouge(FirstLine, DialougeXOffset, FirstLineY, 1);
            DrawDialouge(bufferSecondLine, DialougeXOffset, SecondLineY, 1);
            Sleep(TypingSpeed);
            j++;
            glEnd();
            glFlush();
            
        }

        if (OverflowSecondLine)
        {
            //clear old frame
            ClearFrame();
            RecoverFrame();
            RenderSpeaker(Scene1DialougesWithSpeakers[currentDialouge].first);

            i = 0;

            for (int i = 0; i <= text.length(); i++)
            {
                SecondLine[i] = text[i];
            }

            text = dialouge;

            FillQueue(text);

            int j = 0;

            Dequeue(queue); //remove extra character

            while (!isEmpty(queue))
            {
                //glClear(GL_COLOR_BUFFER_BIT);
                bufferThirdLine[j] = Dequeue(queue);
                DrawDialouge(FirstLine, DialougeXOffset, FirstLineY, 1);
                DrawDialouge(SecondLine, DialougeXOffset, SecondLineY, 1);
                DrawDialouge(bufferThirdLine, DialougeXOffset, ThirdLineY, 1);
                Sleep(TypingSpeed);
                j++;
                glEnd();
                glFlush();
            }
        }
    }
}


void AnimateNextDialouge(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        if (Scene == START)
        {
            queue = createQueue(1000); //create a fresh queue
            Scene = DESCRIPTION;
        }
        else if (Scene == DESCRIPTION)
        {
            queue = createQueue(1000);//create a fresh queue
            Scene = SCENE1;
        }
        else
        {
            currentDialouge++;
        }
    }
}

void  DrawStartBox()
{
    char Start[20] = "Start";

    glBegin(GL_POLYGON);
    glColor3f(1, 0, 0);
    glVertex2f(550, 10);
    glVertex2f(550, 50);
    glVertex2f(700, 50);
    glVertex2f(700, 10);
    glEnd();

    DrawDialouge(Start, 600, 25, 0);
    glFlush();

}


void DrawHouseAndWindow()
{
    for (int i = 0; i < HouseAndWindows.size(); i++)
    {
        if (i < 6)
        {
            glColor3f(0, 0.22, 0.39);
        }
        else
        {
            glColor3f(1,1,1);
        }

        if (i < 12)
        {
            glBegin(GL_POLYGON);
        }
        else
        {
            glColor3f(0, 0, 0);
            glLineWidth(3);
            glBegin(GL_LINES);
        }
        

        for (int j = 0; j < HouseAndWindows[i].size(); j++)
        {
            glVertex2f(HouseAndWindows[i][j].first, HouseAndWindows[i][j].second);
        }

        glEnd();
    }
}


void DrawMansion()
{
    DrawHouseAndWindow();
    glFlush();
}

void  DrawMoon()
{
    char Title1[20] = "Murder";
    char Title2[20] = "Mystery";

    glBegin(GL_POLYGON);
    int x = 1100, y = 550, r = 100;

    glColor3f(0,0.22,0.39);

    for (int i = 0; i < 360; i++)
    {
        float theta = i * 3.14 / 180;

        glVertex2f(x + r * cos(theta) , y + r * sin(theta));
    }

    glEnd();
    
    glColor3f(1, 0, 0);
    DrawDialouge(Title1, 1040, 560, 2);
    DrawDialouge(Title2, 1080, 535, 2);

    glFlush();
}

void DrawGreenLine()
{
    glLineWidth(1);
    glBegin(GL_LINES);
    glColor3f(0, 1, 0);
    glVertex2f(100, 70);
    glVertex2f(1100, 70);
    glEnd();
    glFlush();
}

void DrawDescriptionText(char* string, int x, int y)
{
    int len, i;
    glRasterPos2f(x, y);
    len = (int)strlen(string);
    glColor3f(1, 1, 1);

    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
    }
}

//I just hardCoded this crap , i will try to find a better solution
void AnimateDescription()
{  
    string Line1 = Description[0];
    string Line2 = Description[1];
    string Line3 = Description[2];
    string Line4 = Description[3];
    string Line5 = Description[4];
    string Line6 = Description[5];

    char line1Buffer[300] = "";
    char line2Buffer[300] = "";
    char line3Buffer[300] = "";
    char line4Buffer[300] = "";
    char line5Buffer[300] = "";
    char line6Buffer[300] = "";


    FillQueue(Line1);
    int i = 0;
    Dequeue(queue); //remove extra character

    while (!isEmpty(queue))
    {
        line1Buffer[i] = Dequeue(queue);
        DrawDescriptionText(line1Buffer, 100, 500);
        Sleep(TypingSpeed);
        i++;
        glEnd();
        glFlush();
    }

    FillQueue(Line2);
    i = 0;
    Dequeue(queue); //remove extra character

    while (!isEmpty(queue))
    {
        line2Buffer[i] = Dequeue(queue);
        DrawDescriptionText(line1Buffer, 100, 500);
        DrawDescriptionText(line2Buffer, 100, 450);
        Sleep(TypingSpeed);
        i++;
        glEnd();
        glFlush();
    }

    FillQueue(Line3);
    i = 0;
    Dequeue(queue); //remove extra character

    while (!isEmpty(queue))
    {
        line3Buffer[i] = Dequeue(queue);
        DrawDescriptionText(line1Buffer, 100, 500);
        DrawDescriptionText(line2Buffer, 100, 450);
        DrawDescriptionText(line3Buffer, 100, 400);
        Sleep(TypingSpeed);
        i++;
        glEnd();
        glFlush();
    }

    FillQueue(Line4);
    i = 0;
    Dequeue(queue); //remove extra character

    while (!isEmpty(queue))
    {
        line4Buffer[i] = Dequeue(queue);
        DrawDescriptionText(line1Buffer, 100, 500);
        DrawDescriptionText(line2Buffer, 100, 450);
        DrawDescriptionText(line3Buffer, 100, 400);
        DrawDescriptionText(line4Buffer, 100, 350);
        Sleep(TypingSpeed);
        i++;
        glEnd();
        glFlush();
    }

    FillQueue(Line5);
    i = 0;
    Dequeue(queue); //remove extra character

    while (!isEmpty(queue))
    {
        line5Buffer[i] = Dequeue(queue);
        DrawDescriptionText(line1Buffer, 100, 500);
        DrawDescriptionText(line2Buffer, 100, 450);
        DrawDescriptionText(line3Buffer, 100, 400);
        DrawDescriptionText(line4Buffer, 100, 350);
        DrawDescriptionText(line5Buffer, 100, 300);
        Sleep(TypingSpeed);
        i++;
        glEnd();
        glFlush();
    }

    FillQueue(Line6);
    i = 0;
    Dequeue(queue); //remove extra character

    while (!isEmpty(queue))
    {
        line6Buffer[i] = Dequeue(queue);
        DrawDescriptionText(line1Buffer, 100, 500);
        DrawDescriptionText(line2Buffer, 100, 450);
        DrawDescriptionText(line3Buffer, 100, 400);
        DrawDescriptionText(line4Buffer, 100, 350);
        DrawDescriptionText(line5Buffer, 100, 300);
        DrawDescriptionText(line6Buffer, 100, 250);
        Sleep(TypingSpeed);
        i++;
        glEnd();
        glFlush();
    }

}

void LoadMainScreen()
{
    glClear(GL_COLOR_BUFFER_BIT);
    DrawMoon();
    DrawMansion();
    DrawGreenLine();
    DrawStartBox();
    glEnd();
    glFlush();
}

void init()
{
    glClearColor(0, 0, 0, 1);
    gluOrtho2D(0.0, 1280.0, 0.0, 720.0);
}

void display() //display function is called repeatedly by the main function so keep all rendering functions here
{
    glClear(GL_COLOR_BUFFER_BIT);

   switch(Scene)
   {
   case START:
        LoadMainScreen();
        break;

   case DESCRIPTION:
       AnimateDescription();
       break;

   case SCENE1:
       RecoverFrame();
       RenderSpeaker(Scene1DialougesWithSpeakers[currentDialouge].first);
       AnimateText(Scene1DialougesWithSpeakers[currentDialouge].second);
       break;

   default:
       cout << "Something went wrong";
       break;

   }
    glEnd();
    glFlush();
}

void InitializeVariables()
{
    Description = {
    "One day John, Peter, Dan, Angela, Chris, Emily and " + PlayerName + " ,the seven friends, who work in different MNCs ",
    "got tired of their day to day life, wanted to go on a long vaccation. John suggested to go to the countryside so that " ,
    "they can get rid of the city rush, and all the friends agreed.They decided to book a hotel but later Dan suggested that ",
    "he knows a person who owns a mansion and is willing to rent it for a fair price, all of them agreed and were very  ",
    "excited. They began their journey 3 days from then in John’s car and reached there the next morning. All of them ",
    "were very excited to see the magnificent mansion and began to explore the mansion....  "
    };

    Scene1DialougesWithSpeakers = {
    {"You " , "Hello this is some dummy text... "},
    {"Computer " , "In C++, std::substr() is a predefined function used for string handling. string.h is the header file required for string functions. This function takes two values pos and len as an argument and returns a newly constructed string object with its value initialized to a copy of a sub - string of this object. "},
    { "Melwyn Sir " , "Write a JavaScript to design a simple calculator to perform the following operations: sum, product, difference and quotient. "},
    {"Melwyn Sir " , "Write a JavaScript that calculates the squares and cubes of the numbers from 0 to 10 and outputs HTML text that displays the resulting values in an HTML table format. "},
    {"Ashwin Sir " , "Mini Project Final demo will be on 30/06/22 and 1/07/22  time slot will be shared later. "},
    {"Ashwin Sir " ,  "Completed Record and manual need to submit during internal time. "}
    };
}

int main(int argc, char** argv)
{
    cout << "Enter your name : " << endl;
    cin>>PlayerName;
    queue = createQueue(1000);
    InitializeVariables();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(1280, 720);
    glutInitWindowPosition(300, 200);
    glutCreateWindow("MurderMystery");
    glutMouseFunc(AnimateNextDialouge);
    init();
    glutDisplayFunc(display);
    glutMainLoop();
}