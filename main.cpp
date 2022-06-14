#include<GL/glut.h>
#include<stdio.h>
#include<iostream>
#include<string>
#include<ctype.h>

using namespace std;


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

void AnimateText()
{
    queue = createQueue(1000);
    string text = "A wiki  is a hypertext publication collaboratively edited and managed by its own audience directly. ";
    char bufferText[400] = "";
    int i = 0;

    glClear(GL_COLOR_BUFFER_BIT);
    glEnd();
    glFlush();

    for (auto x : text)
    {
        Enqueue(queue, x);
    }

    cout << Dequeue(queue);

    while(!isEmpty(queue) && State == 1)
    {
        glClear(GL_COLOR_BUFFER_BIT);
        bufferText[i] = Dequeue(queue);
        drawText(bufferText, 10, 80);
        Sleep(25);
        i++;
        glEnd();
        glFlush();
    }
}

void CallMouseButton(int button, int state, int x, int y)
{
    cout << button << state;
    State = 0;
   
}

void init()
{
    glClearColor(0, 0, 0, 1);
    gluOrtho2D(0.0, 1280.0, 0.0, 720.0);
}

void display()
{
    AnimateText();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(1280, 720);
    glutInitWindowPosition(300, 200);
    glutCreateWindow("Bresenham's Lune Drawing");
    glutMouseFunc(CallMouseButton);
    init();
    glutDisplayFunc(display);
    glutMainLoop();
}