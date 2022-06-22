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
const int TypingSpeed = 2; //20
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
enum Scenes { START, DESCRIPTION, SCENE1, SCENE2, TOBECONTINUED };
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

vector<vector<pair<int, int>>> GenericRoom =
{
    { {0,240},{187, 419},{187, 588}, {0, 720} },            //1.LeftWall - Upper - Polygon - color glColor3f(1, 0.99, 0.81) Cream Color
    { {1280, 240},{1093, 419},{1093, 588}, {1280, 720} },   //2.RightWall - Upper - Polygon 
    { {187, 588},{0, 720},{1280, 720}, {1093, 588} },       //3.//UpperWall - Polygon 
    { {187, 419},{187, 588},{1093, 588}, {1093, 419} },     //4. //FrontWall - Upper - Polygon 
    { {0, 0},{187, 245},{187, 420}, {0, 240} },             //5. //LeftWall - Lower - Polygon - color glColor3f(0.58, 0.43, 0.20) //WoodColor
    { {1280, 0},{1093, 245},{1093, 419}, {1280, 245} },     //6. //RightWall - Lower - Polygon
    { {187, 245},{187, 419},{1093, 419}, {1093, 245} },     //7. //FrontWall - Lower - Polygon
    { {0, 0},{187, 245},{1093, 245}, {1280, 0} },           //8.  //Ground - Polygon
    { {50, 330},{50, 430},{110, 480}, {110, 380} },         //9.  //Window Body - Polygon glColor3f(1, 1, 1) // white
    { {0, 0},{187, 250},{187, 588}, {0, 720} },             //10. //LeftWallOutline  - GL_LINE_STRIP glColor3f(0, 0, 0) // black; width = 2
    { {1280, 0},{1093, 250},{1093, 588}, {1280, 720} },     //11. //RightWallOutline  - GL_LINE_STRIP
    { {0, 241},{187, 419},{1093, 419}, {1280, 243} },       //12. //SplitLine  - GL_LINE_STRIP
    { {187, 588},{1093, 588} },                             //13. //UpperWallOutLine  - GL_LINES
    { {187, 250},{1093, 250} },                             //14. //LowerWallOutLine  - GL_LINES
    { {80, 355},{80, 455} },                                //15.  //WindowLine - Vertical  - GL_LINES
    { {50, 380},{110, 430} },                               //16.  //WindowLine - Horizontal  - GL_LINES
    { {50, 330},{50, 430} , {110, 480}, {110, 380} },       //17.  //Window Outline  - GL_LINE_LOOP width = 3
    
};

vector<vector<pair<int, int>>> Chair = 
{
    { {200, 250},{260, 230},{320, 260}, {260, 280} },       //0.Chair sitting - glColor3f(1, 0.64, 0); (chair brown color) - Polygon
    { {200, 252},{200, 350},{260, 370}, {260, 282} },       //1.Chair Top - glColor3f(1, 0.64, 0); (chair brown color) - Polygon
    { {200, 251},{260, 281} },                              //2.Chair Joiner - glColor3f(1, 1, 1); Lines - glLineWidth(3);
    { {200, 250},{200, 200} },                              //3.Chair Leg left - glColor3f(0, 0, 0); Lines - glLineWidth(2);
    { {260, 230},{260, 180} },                              //4.Chair Leg middle - glColor3f(0, 0, 0); Lines - glLineWidth(2);
    { {320, 260},{320, 210} },                              //5.Chair Leg right - glColor3f(0, 0, 0); Lines - glLineWidth(2);
};


vector<vector<pair<int, int>>> Cupboard =
{
     { {900, 150},{900, 430},{1094, 430}, {1094, 150} },    //0.CupBoardBody - Polygon - glColor3f(1, 0, 0);
     { {900, 150},{900, 100},{920, 100}, {920, 150} },      //1.Cupboard left leg - Polygon - glColor3f(0, 0, 0);
     { {1074, 150},{1074, 100},{1094, 100}, {1094, 150} },  //2.Cupboard right leg - Polygon - glColor3f(0, 0, 0);
     { {997, 150},{997, 430} },                             //3.Cupboard Line - GL_LINES - glColor3f(0, 0, 0); - glLineWidth(3);
     { {985, 290} },                                        //4.Cupboard Knob left - GL_POINTS - glColor3f(0, 0, 0); - glPointSize(5);
     { {1005, 290} },                                       //5.Cupboard Knob left - GL_POINTS - glColor3f(0, 0, 0); - glPointSize(5);
     { {900, 430},{920, 500},{1074, 500}, {1094, 430} },    //6.Cupboard Knob left - GL_LINE_STRIP - glColor3f(0, 0, 0); - glLineWidth(2);
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

void DrawDialouge(char* string, int x, int y, int type)
{
    if (type == 1)
    {
        glColor3f(1, 1, 1); //white
    }
    else if (type == 0)
    {
        glColor3f(0, 0, 0); // black
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

//-------------Click to Continue Indicator-----------
void DrawClickToContinue()
{
    char clicktoContinue[100] = "Continue";
    DrawDialouge(clicktoContinue, 1060, 35, 1);
    glColor3f(1, 1, 1);
    glBegin(GL_POLYGON);
    glVertex2f(1150, 30);
    glVertex2f(1155, 30);
    glVertex2f(1165, 40);
    glVertex2f(1155, 50);
    glVertex2f(1150, 50);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(1170, 30);
    glVertex2f(1175, 30);
    glVertex2f(1185, 40);
    glVertex2f(1175, 50);
    glVertex2f(1170, 50);
    glEnd();
}

//-------------Click to Continue Indicator END-----------


//Drawing a Generic Room
void DrawRoomBG()
{
    for (int i = 0; i < GenericRoom.size(); i++)
    {
        //Sorting Colors
        if (i < 4) {glColor3f(1, 0.99, 0.81); } // Cream Color
        else if (i < 8) { glColor3f(0.58, 0.43, 0.20); } // Wood Color
        else if (i == 8) { glColor3f(1, 1, 1); } // white color
        else if (i >= 9) { glColor3f(0, 0, 0); } // Black color

        //LineWidth
        if (i >= 9 && i < 16) { glLineWidth(2); }
        if (i == 16) { glLineWidth(3); }

        //Shape
        if (i <= 8) { glBegin(GL_POLYGON); }
        if (i >= 9 && i <= 11) { glBegin(GL_LINE_STRIP); }
        if (i >= 12 && i <= 15) { glBegin(GL_LINES); }
        if (i == 16) { glBegin(GL_LINE_LOOP); }


        for (int j = 0; j < GenericRoom[i].size(); j++)
        {
            glVertex2f(GenericRoom[i][j].first, GenericRoom[i][j].second);
        }
        glEnd();
    }

}

void DrawChair()
{
    for (int i = 0; i < Chair.size(); i++)
    {
        //SortingColors
        if (i < 2) { glColor3f(1, 0.64, 0); } //chair wood color
        if (i == 2) { glColor3f(1, 1, 1); } //white color
        if (i > 2) { glColor3f(0, 0, 0); } //black color

        //LineWidth
        if (i == 2) { glLineWidth(3); }
        if (i > 2) { glLineWidth(2); }

        //Shape
        if (i < 2) { glBegin(GL_POLYGON); } 
        if (i >= 2) { glBegin(GL_LINES); }

        //Drawing Shape
        for (int j = 0; j < Chair[i].size(); j++)
        {
            glVertex2f(Chair[i][j].first, Chair[i][j].second);
        }

        glEnd();
    }
}

void DrawCupBoard()
{
    for (int i = 0; i < Cupboard.size(); i++)
    {
        //SortingColors
        if (i == 0) { glColor3f(1, 0, 0); } //red
        else { glColor3f(0, 0, 0); } //black

        //LineWidth and PointSize
        if (i == 3) { glLineWidth(3); }
        if (i == 4 || i == 5) { glPointSize(5); }
        if (i == 6) { glLineWidth(2); }

        //Shape
        if (i < 3) { glBegin(GL_POLYGON); }
        if (i == 3) { glBegin(GL_LINES); }
        if (i < 6) { glBegin(GL_POINTS); }
        if (i == 6) { glBegin(GL_LINE_STRIP); }

        //DrawingShape
        for (int j = 0; j < Cupboard[i].size(); j++)
        {
            glVertex2f(Cupboard[i][j].first, Cupboard[i][j].second);
        }

        glEnd();
    }
}

void DrawScene1BG()
{
    DrawRoomBG();
    DrawChair();
    DrawCupBoard();

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

   
    if (NewScene)
    {
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

    char characterS[100] = "";
    vector<pair<int, int>> position;

    if (Speaker == "Chris")
    {
        characterSprite = "Chris.png";

        for (int i = 0; i < characterSprite.length(); i++)
        {
            characterS[i] = characterSprite[i];
        }

        position =
        {
            {450, 172},
            {450, 410},
            {700, 410},
            {700, 172}
        };
    }
    else if (Speaker == "Andy")
    {
        characterSprite = "Andy.png";

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
    else  if (Speaker == "You")  //showing all characters
    {
        //later i have to clean this saav

        glColor3f(0.58, 0.43, 0.20); //woodColor
         //Loading texture
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        glGenTextures(1, &characterTexture);
        stbi_set_flip_vertically_on_load(true);
        int width, height, nrChannels;
        unsigned char* character = stbi_load("Chris.png", &width, &height, &nrChannels, 0);
        glBindTexture(GL_TEXTURE_2D, characterTexture);

        if (character != NULL)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, character);
            cout << "Character loaded" << endl;
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
        glTexCoord2f(0.0f, 0.0f);   glVertex2f(450, 172);
        glTexCoord2f(0.0f, 1.0f);   glVertex2f(450, 410);
        glTexCoord2f(1.0f, 1.0f);   glVertex2f(700, 410);
        glTexCoord2f(1.0f, 0.0f);   glVertex2f(700, 172);
        glEnd();


        glColor3f(0.58, 0.43, 0.20); //woodColor
   //Loading texture
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        glGenTextures(1, &characterTexture);
        stbi_set_flip_vertically_on_load(true);
        width, height, nrChannels;
        unsigned char* character2 = stbi_load("Andy.png", &width, &height, &nrChannels, 0);
        glBindTexture(GL_TEXTURE_2D, characterTexture);

        if (character2 != NULL)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, character2);
            cout << "Character loaded" << endl;
        }
        else
        {
            cout << "Failed to load character" << endl;
        }
        stbi_image_free(character2);

        //displaying

        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
        glEnable(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, characterTexture);

        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);   glVertex2f(700, 172);
        glTexCoord2f(0.0f, 1.0f);   glVertex2f(700, 418);
        glTexCoord2f(1.0f, 1.0f);   glVertex2f(900, 418);
        glTexCoord2f(1.0f, 0.0f);   glVertex2f(900, 172);
        glEnd();
        glDeleteTextures(1, &characterTexture);
        return;
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
        cout << "Character loaded"<<endl;
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


bool CheckOverFlow(string& dialouge, string& text)
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
        OverflowFirstLine = true;
    }

    FillQueue(text);

    while (!isEmpty(queue))
    {
        //glClear(GL_COLOR_BUFFER_BIT);
        FirstLine[i] = Dequeue(queue);
        DrawDialouge(FirstLine, DialougeXOffset, FirstLineY, 1);
        Sleep(TypingSpeed);
        i++;
        glEnd();
        glFlush();
    }

    if (OverflowFirstLine)
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
       

        switch (Scene)
        {
        case START:
           
            Scene = DESCRIPTION;
            break;

        case DESCRIPTION:
            Scene = SCENE1;
            break;

        case SCENE1:
            if (currentDialouge == Scene1DialougesWithSpeakers.size() - 1)
            {
                currentDialouge = 0;
                Scene = TOBECONTINUED;
                //NewScene = true;
            }
            else
            {
                currentDialouge++;
            }

        default:
            break;
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
            glColor3f(1, 1, 1);
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

    glColor3f(0, 0.22, 0.39);

    for (int i = 0; i < 360; i++)
    {
        float theta = i * 3.14 / 180;

        glVertex2f(x + r * cos(theta), y + r * sin(theta));
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

//------------------THROW AWAY CODE----------------
void DrawTOBECONTINUED()
{
    string tobeContinued = "TO BE CONTINUED...... ";
    string in = "IN ";
    string Phase2 = "PHASE 2 ";
   // string saav = "Yenchina Saav Ya";

    char buffer1[200] = "";
    char buffer2[200] = "";
    char buffer3[200] = "";
   // char buffer4[200] = "";

    
    int i = 0;

    FillQueue(tobeContinued);

    while (!isEmpty(queue))
    {
        buffer1[i] = Dequeue(queue);
        DrawDescriptionText(buffer1, 400, 400);
        Sleep(100);
        i++;
        glEnd();
        glFlush();
    }

    for (int i = 0; i < tobeContinued.length(); i++)
    {
        buffer1[i] = tobeContinued[i];
    }

    for (int i = 0; i < in.length(); i++)
    {
        buffer2[i] = in[i];
    }

    for (int i = 0; i < Phase2.length(); i++)
    {
        buffer3[i] = Phase2[i];
    }

    /*
    for (int i = 0; i < saav.length(); i++)
    {
        buffer4[i] = saav[i];
    }
    */

    glClear(GL_COLOR_BUFFER_BIT);
    DrawDescriptionText(buffer1, 400, 400);
    glEnd();
    glFlush();

    Sleep(500);

    DrawDescriptionText(buffer2, 500, 350);
    glEnd();
    glFlush();
    Sleep(500);

    DrawDescriptionText(buffer3, 480, 300);

    glEnd();
    glFlush();

    /*
    Sleep(1000);
    DrawDescriptionText(buffer4, 1000, 20);

    glEnd();
    glFlush();
    */

}

//I just hardCoded this crap , i will try to find a better solution
void AnimateDescription()
{
    

    string Line1 = Description[0];
    string Line2 = Description[1];
    string Line3 = Description[2];
    string Line4 = Description[3];
    string Line5 = Description[4];

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

    switch (Scene)
    {
    case START:
        LoadMainScreen();
        break;

    case DESCRIPTION:
        queue = createQueue(1000); //create a fresh queue
        AnimateDescription();
        DrawClickToContinue();
        

        break;

    case SCENE1:
        queue = createQueue(1000);//create a fresh queue
        DrawScene1BG();
        RecoverFrame();
        RenderSpeaker(Scene1DialougesWithSpeakers[currentDialouge].first);
        DrawCharacter(Scene1DialougesWithSpeakers[currentDialouge].first);
        AnimateText(Scene1DialougesWithSpeakers[currentDialouge].second);
        DrawClickToContinue();
        

        break;

    case TOBECONTINUED:
        DrawTOBECONTINUED();
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
    "One day John, Peter, Dan, Angela, Chris, Emily and "+ PlayerName +" , who were best friends since college, who work in ",
    "different MNCs got tired of their day to day life and wanted to go on a long vaccation. John suggested to go to the ",
    "countryside so that they can get rid of the city rush and all the friends agreed. They decided to book a hotel but later ",
    "Dan suggested that he knows a person who owns a mansion and is willing to rent it for a fair price, all of them agreed ",
    "and were very excited. They began their journey 3 days from then in John's car and reached there the next morning.  ",
    };

    Scene1DialougesWithSpeakers = {
    {"Narrator" , "As they entered , they were awestruck looking at the magnificent mansion and start exploring it and checked out the rooms they were going to stay in and started unpacking their luggages. "},
    {"Chris" , "Hey "+ PlayerName + " , Andy come check this room out..... its even bigger than my living room!! "},
    {"You" , "Wow.... my whole family can stay here. "},
    { "Andy" , "Ha ha ha... C'mon it isn't that big. But I have been wondering how did Dan get this mansion for such a low price. Something doesnt feel right. "},
    {"Chris" , "C'mon Andy.... why are you like this, overthinking about everything, we are here to relax... we dont get this opportunity everyday. "},
    {"You" , "Yes Chris is right... we are here to release our stress so dont increase it by overthinking. "},
    {"Andy" ,  "Yeah I think you guys are right.... if only I was a little more carefree like you guys. We are in this beautiful        mansion in this peaceful place and we are going to make the best of it. Besides, what can go wrong? "},
    {"????" ,  "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAHHHH!!!!! (Sudden scream from the other room....) "},
    {"You" ,  "I think I just heard Angela screaming...... we must hurry up and check what happened!! "},
    {"Narrator" ,  "They rush towards the scream.... "}
    };
}


int main(int argc, char** argv)
{
    cout << "Enter your name : " << endl;
    cin >> PlayerName;
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