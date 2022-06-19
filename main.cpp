#include<GL/glut.h>
#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<string>
#include<ctype.h>
#include<vector>
#include<math.h>
#define STB_IMAGE_IMPLEMENTATION    
#include "stb_image.h"

using namespace std;

//CONSTANTS
const int TypingSpeed = 20; //20
const int LineCharacterLimit = 120;
const int DialougeXOffset = 150;
const int FirstLineY = 110;
const int SecondLineY = 80;
const int ThirdLineY = 50;
const int SpeakerXPos = 200;
const int SpeakerYPos = 165;

string PlayerName;
GLuint characterTexture;
GLuint characterTexture2;

bool NewScene = true;

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
    //BlackBackground
    glColor3f(0, 0, 0);
    glBegin(GL_POLYGON);
    glVertex2f(75, 20);
    glVertex2f(75, 170);
    glVertex2f(1200, 170);
    glVertex2f(1200, 20);
    glEnd();


    //OutLine of the DialougeBox
    glLineWidth(2);
    glColor3f(1, 1, 1);
    glBegin(GL_LINE_LOOP);
    glVertex2f(75, 20);
    glVertex2f(75, 170);
    glVertex2f(1200, 170);
    glVertex2f(1200, 20);
    glEnd();

    //Speaker's Box
    glBegin(GL_POLYGON);
    glColor3f(1, 0, 0);
    glVertex2f(120, 170);
    glVertex2f(150, 190);
    glVertex2f(400, 190);
    glVertex2f(430, 170);
    glVertex2f(400, 150);
    glVertex2f(150, 150);
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

void DrawScene1BG()
{
    //LeftWall - Upper
    glColor3f(1, 0.99, 0.81);
    glBegin(GL_POLYGON);
    glVertex2f(0, 240);
    glVertex2f(187, 419);
    glVertex2f(187, 588);
    glVertex2f(0, 720);
    glEnd();



    //RightWall - Upper
    glBegin(GL_POLYGON);
    glVertex2f(1280, 240);
    glVertex2f(1093, 419);
    glVertex2f(1093, 588);
    glVertex2f(1280, 720);
    glEnd();



    //UpperWall
    glBegin(GL_POLYGON);
    glVertex2f(187, 588);
    glVertex2f(0, 720);
    glVertex2f(1280, 720);
    glVertex2f(1093, 588);
    glEnd();

    //FrontWall - Upper
    glBegin(GL_POLYGON);
    glVertex2f(187, 419);
    glVertex2f(187, 588);
    glVertex2f(1093, 588);
    glVertex2f(1093, 419);
    glEnd();

    //LeftWall - Lower
    glColor3f(0.58, 0.43, 0.20); //WoodColor
    glBegin(GL_POLYGON);
    glVertex2f(0, 0);
    glVertex2f(187, 245);
    glVertex2f(187, 420);
    glVertex2f(0, 240);
    glEnd();

    //RightWall - Lower
    glBegin(GL_POLYGON);
    glVertex2f(1280, 0);
    glVertex2f(1093, 245);
    glVertex2f(1093, 419);
    glVertex2f(1280, 245);
    glEnd();

    //FrontWall - lower
    glBegin(GL_POLYGON);
    glVertex2f(187, 245);
    glVertex2f(187, 419);
    glVertex2f(1093, 419);
    glVertex2f(1093, 245);
    glEnd();

    //Ground
    glBegin(GL_POLYGON);
    glVertex2f(0, 0);
    glVertex2f(187, 245);
    glVertex2f(1093, 245);
    glVertex2f(1280, 0);
    glEnd();


    //LeftWallOutline 
    glColor3f(0, 0, 0);
    glLineWidth(2);
    glBegin(GL_LINE_STRIP);
    glVertex2f(0, 0);
    glVertex2f(187, 250);
    glVertex2f(187, 588);
    glVertex2f(0, 720);
    glEnd();

    //RightWallOutline
    glBegin(GL_LINE_STRIP);
    glVertex2f(1280, 0);
    glVertex2f(1093, 250);
    glVertex2f(1093, 588);
    glVertex2f(1280, 720);
    glEnd();

    //UpperWallOutLine
    glBegin(GL_LINES);
    glVertex2f(187, 588);
    glVertex2f(1093, 588);
    glEnd();

    //LowerWallOutLine
    glBegin(GL_LINES);
    glVertex2f(187, 250);
    glVertex2f(1093, 250);
    glEnd();

    //SplitLine
    glBegin(GL_LINE_STRIP);
    glVertex2f(0, 241);
    glVertex2f(187, 419);
    glVertex2f(1093, 419);
    glVertex2f(1280, 243);
    glEnd();

    //DrawWindow

    //Window Outline
    glLineWidth(4);
    glBegin(GL_LINE_LOOP);
    glColor3f(0, 0, 0);
    glVertex2f(50, 330);
    glVertex2f(50, 430);
    glVertex2f(110, 480);
    glVertex2f(110, 380);
    glEnd();

    //Window Body
    glBegin(GL_POLYGON);
    glColor3f(1, 1, 1);
    glVertex2f(50, 330);
    glVertex2f(50, 430);
    glVertex2f(110, 480);
    glVertex2f(110, 380);
    glEnd();

    //WindowLine - Vertical
    glLineWidth(2);
    glColor3f(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(80, 355);
    glVertex2f(80, 455);
    glEnd();

    //WindowLine - Horizontal
    glBegin(GL_LINES);
    glVertex2f(50, 380);
    glVertex2f(110, 430);
    glEnd();

    //DrawClock
    //Outline
    glBegin(GL_POLYGON);
    int x = 700, y = 520, r = 50;

    glColor3f(0, 0, 0);

    for (int i = 0; i < 360; i++)
    {
        float theta = i * 3.14 / 180;

        glVertex2f(x + r * cos(theta), y + r * sin(theta));
    }

    glEnd();

    //Face outline
    glBegin(GL_POLYGON);
    x = 700, y = 520, r = 45;

    glColor3f(1, 1, 1);

    for (int i = 0; i < 360; i++)
    {
        float theta = i * 3.14 / 180;

        glVertex2f(x + r * cos(theta), y + r * sin(theta));
    }

    glEnd();

    //Clock Marks
    //LeftMark
    glColor3f(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(655, 520);
    glVertex2f(675, 520);
    glEnd();

    //RightMark
    glColor3f(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(745, 520);
    glVertex2f(725, 520);
    glEnd();

    //TopMark
    glColor3f(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(700, 565);
    glVertex2f(700, 545);
    glEnd();

    //Bottom Mark
    glColor3f(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(700, 475);
    glVertex2f(700, 495);
    glEnd();

    //ClockHourhand
    glColor3f(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(700, 520);
    glVertex2f(680, 530);
    glEnd();

    //ClockMinutehand
    glColor3f(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(700, 520);
    glVertex2f(720, 550);
    glEnd();

    //Clock Center
    glPointSize(4);
    glColor3f(0, 0, 0);
    glBegin(GL_POINTS);
    glVertex2f(699, 521);
    glEnd();

    //--------CHAIR----------

    //Chair Bottom
    glColor3f(1, 0.64, 0); //Chair color
    glBegin(GL_POLYGON);
    glVertex2f(200, 250);
    glVertex2f(260, 230);
    glVertex2f(320, 260);
    glVertex2f(260, 280);
    glEnd();

    //Chair Joiner
    glLineWidth(3);
    glColor3f(1, 1, 1);
    glBegin(GL_LINES);
    glVertex2f(200, 251);
    glVertex2f(260, 281);
    glEnd();

    //Chair Top
    glColor3f(1, 0.64, 0); //Chair color
    glBegin(GL_POLYGON);
    glVertex2f(200, 252);
    glVertex2f(200, 350);
    glVertex2f(260, 370);
    glVertex2f(260, 282);
    glEnd();


    //Chair Legs

    //Chair Leg left
    glLineWidth(2);
    glColor3f(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(200, 250);
    glVertex2f(200, 200);
    glEnd();

    //Chair Leg middle
    glLineWidth(2);
    glColor3f(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(260, 230);
    glVertex2f(260, 180);
    glEnd();

    //Chair Leg right
    glLineWidth(2);
    glColor3f(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(320, 260);
    glVertex2f(320, 210);
    glEnd();

    //------CUPBOARD-------

    //CupBoardBody
    glColor3f(1, 0, 0); 
    glBegin(GL_POLYGON);
    glVertex2f(900, 150);
    glVertex2f(900, 430);
    glVertex2f(1094, 430);
    glVertex2f(1094, 150);
    glEnd();

    //cupboardlegs

    //cupboard left leg
    glColor3f(0, 0, 0); 
    glBegin(GL_POLYGON);
    glVertex2f(900, 150);
    glVertex2f(900, 100);
    glVertex2f(920, 100);
    glVertex2f(920, 150);
    glEnd();

    //cupboard right leg
    glColor3f(0, 0, 0); 
    glBegin(GL_POLYGON);
    glVertex2f(1074, 150);
    glVertex2f(1074, 100);
    glVertex2f(1094, 100);
    glVertex2f(1094, 150);
    glEnd();

    //Cupboard Line
    glLineWidth(3);
    glColor3f(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(997, 150);
    glVertex2f(997, 430);
    glEnd();

    //Cupboard Knob left
    glPointSize(5);
    glBegin(GL_POINTS);
    glVertex2f(985, 290);
    glEnd();

    //Cupboard Knob right
    glPointSize(5);
    glBegin(GL_POINTS);
    glVertex2f(1005, 290);
    glEnd();

    //Cupboard upper Design
    glLineWidth(2);
    glBegin(GL_LINE_STRIP);
    glVertex2f(900, 430);
    glVertex2f(920, 500);
    glVertex2f(1074, 500);
    glVertex2f(1094, 430);
    glEnd();


    if (NewScene)
    {
        cout << "hello";
        NewScene = false;
        glFlush();
        Sleep(1000);
    }
}

void ClearFrame()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glEnd();
    glFlush();
}

void RecoverFrame()
{
    DrawScene1BG();
    DrawDialougeBox();
}



void DrawCharacter(string Speaker)
{
    string characterSprite;

    char characterS[100]  = "";
    vector<pair<int, int>> position;

    if (Speaker == "Dan ")
    {
        characterSprite = "character.png";

        for (int i = 0; i < characterSprite.length(); i++)
        {
            characterS[i] = characterSprite[i];
        }

        cout << characterS;
        position =
        {
            {500, 172},
            {500, 410},
            {700, 410},
            {700, 172}
        };
    }
    else if(Speaker == "John ")
    {
        characterSprite = "character2.png";

        for (int i = 0; i < characterSprite.length(); i++)
        {
            characterS[i] = characterSprite[i];
        }

        position =
        {
            {700, 172},
            {700, 418},
            {900, 418},
            {900, 172}
        };
    }
    else
    {
        return;
    }

    glColor3f(0.58, 0.43, 0.20); //woodColor
    //Loading texture
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glGenTextures(1, &characterTexture);
    stbi_set_flip_vertically_on_load(true);
    int width, height, nrChannels;
    unsigned char* character = stbi_load(characterS, &width, &height, &nrChannels, 0);
    glBindTexture(GL_TEXTURE_2D, characterTexture);

    if (character != NULL)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, character);
        cout << "Character loaded";
    }
    else
    {
        cout << "Failed to load character" << endl;
    }
    stbi_image_free(character);

    //displaying

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glEnable(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, characterTexture);
    
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);   glVertex2f(position[0].first, position[0].second);
    glTexCoord2f(0.0f, 1.0f);   glVertex2f(position[1].first, position[1].second);
    glTexCoord2f(1.0f, 1.0f);   glVertex2f(position[2].first, position[2].second);
    glTexCoord2f(1.0f, 0.0f);   glVertex2f(position[3].first, position[3].second);
    glEnd();
  
 

    glDeleteTextures(1, &characterTexture);
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
    char FirstLine[201] = "";
    char SecondLine[201] = "";
    char ThirdLine[201] = "";
    int i = 0;

    if (CheckOverFlow(dialouge, text))
    {
        OverflowFirstLine =true;
    }

    FillQueue(text);

    while(!isEmpty(queue))
    {
            //glClear(GL_COLOR_BUFFER_BIT);
            FirstLine[i] = Dequeue(queue);
            DrawDialouge(FirstLine, DialougeXOffset, FirstLineY ,1);
            Sleep(TypingSpeed);
            i++;
            glEnd();
            glFlush();
    }

    if(OverflowFirstLine)
    {
        i = 0;
      
        if (CheckOverFlow(dialouge, text))
        {
            OverflowSecondLine = true;
        }
       
        FillQueue(text);

        int j = 0;

        while (!isEmpty(queue))
        {
            SecondLine[j] = Dequeue(queue);
            DrawDialouge(FirstLine, DialougeXOffset, FirstLineY, 1);
            DrawDialouge(SecondLine, DialougeXOffset, SecondLineY, 1);
            Sleep(TypingSpeed);
            j++;
            glEnd();
            glFlush();
            
        }

        if (OverflowSecondLine)
        {
            i = 0;

            text = dialouge;

            FillQueue(text);

            int j = 0;

            while (!isEmpty(queue))
            {
                //glClear(GL_COLOR_BUFFER_BIT);
                ThirdLine[j] = Dequeue(queue);
                DrawDialouge(FirstLine, DialougeXOffset, FirstLineY, 1);
                DrawDialouge(SecondLine, DialougeXOffset, SecondLineY, 1);
                DrawDialouge(ThirdLine, DialougeXOffset, ThirdLineY, 1);
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

    //Door Knob
    glColor3f(0, 0, 0);
    glPointSize(5.0);
    glBegin(GL_POINTS);
    glVertex2f(360, 250);
    glEnd();
    

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
       DrawScene1BG();
       RecoverFrame();
       RenderSpeaker(Scene1DialougesWithSpeakers[currentDialouge].first);
       DrawCharacter(Scene1DialougesWithSpeakers[currentDialouge].first);
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
    {"Scene 1" , "After a Little Exploration, You , Dan and John got a room with a nice view and decided to take the room for the night and started unpacking their luggage "},
    {"John " , "Hello this is some dummy text... "},
    {"John " , "In C++, std::substr() is a predefined function used for string handling. string.h is the header file required for string functions. This function takes two values pos and len as an argument and returns a newly constructed string object with its value initialized to a copy of a sub - string of this object. "},
    { "Dan " , "Write a JavaScript to design a simple calculator to perform the following operations: sum, product, difference and quotient. "},
    {"John " , "Write a JavaScript that calculates the squares and cubes of the numbers from 0 to 10 and outputs HTML text that displays the resulting values in an HTML table format. "},
    {"Dan " , "Mini Project Final demo will be on 30/06/22 and 1/07/22  time slot will be shared later. "},
    {"John " ,  "Completed Record and manual need to submit during internal time. "}
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
    glutInitWindowPosition(100, 50);
    glutCreateWindow("MurderMystery");
    glutMouseFunc(AnimateNextDialouge);
    init();
    glutDisplayFunc(display);
    glutMainLoop();
}