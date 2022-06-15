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
const int DialougeXOffset = 100;
const int FirstLineY = 150;
const int SecondLineY = 0;
const int ThirdLineY = 0;

enum LineNumber {FirstLine , SecondLine , ThirdLine};

LineNumber currentLineNumber = FirstLine;

vector<string> Scene1 = { "Hii!! ",
                          "You Exicted for the trip that we are gonna have this sunday at goa coming soon also , did u bring your essentials , we dont anyone having any issues??? ",
                               "You bet I am ",
                           "This is gonna be one of the best trips of our lives ",
                            "Lets just hope nothing bad happens for these few days ",
                            "AAAANNND you've jixed it .......Great " };

int currentDialouge = 0;




class QueueText {
public:
    int front, rear, size;
    unsigned capacity;
    char* array;
};

QueueText* queue;
int State = 1;

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


void drawText(char* string, int x, int y)
{
    int len, i;
    glRasterPos2f(x, y);
    len = (int)strlen(string);

    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
    }
}




void AnimateText(string dialouge)
{
    queue = createQueue(1000);
    string text;
    bool Overflow = false;
    char bufferText[400] = "";
    int i = 0;

   
    if (dialouge.length() > 100)
    {
        Overflow = true;
        text = dialouge.substr(0, 100);
        dialouge.erase(0, 100); //erases first 100 characters
    }
    else
    {
        text = dialouge;
    }
   

   // text = dialouge;

    glClear(GL_COLOR_BUFFER_BIT);
    glEnd();
    glFlush();

    for (auto x : text)
    {
        Enqueue(queue, x);
    }

    //cout << Dequeue(queue);

    bool moreDialouge = true;


    while(!isEmpty(queue))
    {
            glClear(GL_COLOR_BUFFER_BIT);
            bufferText[i] = Dequeue(queue);
            drawText(bufferText, DialougeXOffset, FirstLineY);
            Sleep(TypingSpeed);
            i++;
            glEnd();
            glFlush();
    }


    if (Overflow)
    {
        char FirstLine[101];
        char bufferSecondLine[400] = "";
        i = 0;

        for (int i = 0; i < 100 ; i ++)
        {
            FirstLine[i] = text[i];
        }

        text = dialouge;
        cout << text;

        for (auto x : text)
        {
            Enqueue(queue, x);
        }

        while (!isEmpty(queue))
        {
            
            glClear(GL_COLOR_BUFFER_BIT);
            bufferSecondLine[i] = Dequeue(queue);
            drawText(FirstLine, DialougeXOffset, FirstLineY);
            drawText(bufferSecondLine, 100, 100);
            Sleep(TypingSpeed);
            i++;
            glEnd();
            glFlush();
            
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

void display() //display function is called repeatedly by the main function so keep all rendering functions here
{
    glClear(GL_COLOR_BUFFER_BIT);
    AnimateText(Scene1[currentDialouge]);
    glEnd();
    glFlush();
}

int main(int argc, char** argv)
{
   
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