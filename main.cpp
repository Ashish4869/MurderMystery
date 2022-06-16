#include<GL/glut.h>
#include<stdio.h>
#include<iostream>
#include<string>
#include<ctype.h>
#include<vector>

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





vector<string> Scene1Dialouges = { "Hello this is some dummy text... ",
                          "In C++, std::substr() is a predefined function used for string handling. string.h is the header file required for string functions. This function takes two values pos and len as an argument and returns a newly constructed string object with its value initialized to a copy of a sub - string of this object. ",
                               "Write a JavaScript to design a simple calculator to perform the following operations: sum, product, difference and quotient. ",
                           "Write a JavaScript that calculates the squares and cubes of the numbers from 0 to 10 and outputs HTML text that displays the resulting values in an HTML table format. ",
                            "Mini Project Final demo will be on 30/06/22 and 1/07/22  time slot will be shared later. ",
                            "Completed Record and manual need to submit during internal time. " };

vector<string> Scene1Speakers = {
    "You ",
    "Computer ",
    "Melwyn Sir ",
    "Melwyn Sir ",
    "Ashwin Sir ",
    "Ashwin Sir "
};

vector<pair<string,string>> Scene1DialougesWithSpeakers;

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

    //Speaking Box
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
    else
    {
        glColor3f(0, 0, 0);
    }
    int len, i;
    glRasterPos2f(x, y);
    len = (int)strlen(string);

    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
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
    if (dialouge.length() > 100)
    {
        
        text = dialouge.substr(0, 100);
        dialouge.erase(0, 100); //erases first 100 characters
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
    //RecoverFrame();
    char name[400] = "";
    

    string text;
    bool OverflowFirstLine = false;
    bool OverflowSecondLine = false;
    char bufferText[400] = "";
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


        char FirstLine[101];
        char bufferSecondLine[400] = "";
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

            char SecondLine[101];
            char bufferThirdLine[400] = "";
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
        currentDialouge++;
    }
   
}

void init()
{
    glClearColor(0, 0, 0, 1);
    gluOrtho2D(0.0, 1280.0, 0.0, 720.0);
}

void ObtainDialougeAndSpeakers()
{
    for (int i = 0; i < Scene1Dialouges.size(); i++)
    {
        pair<string, string> dialouge;
        dialouge.first = Scene1Speakers[i];
        dialouge.second = Scene1Dialouges[i];

        Scene1DialougesWithSpeakers.push_back(dialouge);
    }
        
    /*
    for (auto x : Scene1DialougesWithSpeakers)
    {
        cout << x.first<<endl;
        cout << x.second<<endl;
        cout << "\n\n";
    }
    */
}

void display() //display function is called repeatedly by the main function so keep all rendering functions here
{
    glClear(GL_COLOR_BUFFER_BIT);


    if (State == 1)
    {
        ObtainDialougeAndSpeakers();
    }

    RecoverFrame();
    RenderSpeaker(Scene1DialougesWithSpeakers[currentDialouge].first);
    AnimateText(Scene1DialougesWithSpeakers[currentDialouge].second);
    
    
    
    glEnd();
    glFlush();
}

int main(int argc, char** argv)
{
    queue = createQueue(1000);
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