#include<GL/glut.h>
#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<string>
#include<ctype.h>
#include <map>
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

string PlayerName; //name of the player inputed at the start
GLuint characterTexture;
GLuint characterTexture2;

bool NewScene = true; //bool to give a pause when a new scene is loaded

//States
enum Scenes { START, DESCRIPTION, SCENE, TOBECONTINUED , CHOOSING , SCENEA , SCENEB};
Scenes Scene = START;

//SceneVairables
int currentScene = 0; //Stores the scene number 
int branchCounter = 0; //Stores the no branches encountered
int currentDialouge = 0;
int j;

// The Dialouge arrays for main branch , Branch A and Branch B are defined here
#pragma region SceneData
//An Array of dialouges and speakers for each scene present in the game
vector<string> Description;

//MAIN BRANCH
vector<pair<string, string>> Scene1DialougesWithSpeakers;
vector<pair<string, string>> Scene2DialougesWithSpeakers;
vector<pair<string, string>> Scene3DialougesWithSpeakers;
vector<pair<string, string>> Scene4DialougesWithSpeakers;
vector<pair<string, string>> Scene5DialougesWithSpeakers;
vector<pair<string, string>> Scene6DialougesWithSpeakers;
vector<pair<string, string>> Scene7DialougesWithSpeakers;
vector<pair<string, string>> Scene8DialougesWithSpeakers;

//BRANCH A
vector<pair<string, string>> Branch1ADialougesWithSpeakers;

//BRANCH B
vector<pair<string, string>> Branch1BDialougesWithSpeakers;

//Initialising the dialouges in a vector 
vector<vector<pair<string, string>>> SceneDialouges =
{
    Scene1DialougesWithSpeakers,
    Scene2DialougesWithSpeakers,
    Scene3DialougesWithSpeakers,
    Scene4DialougesWithSpeakers,
    Scene5DialougesWithSpeakers,
    Scene6DialougesWithSpeakers,
    Scene7DialougesWithSpeakers,
    Scene8DialougesWithSpeakers
};

//Same as Scene Dialouges but for Branch A
vector<vector<pair<string, string>>> BranchADialouges =
{
    Branch1ADialougesWithSpeakers,
};

//Same as Scene Dialouges but for Branch B
vector<vector<pair<string, string>>> BranchBDialouges =
{
    Branch1BDialougesWithSpeakers,
};


#pragma endregion

// The Coordinates of the position of each character in each scene is specified below
#pragma region CharacterData

//Characters and their position on each scene
map <string, vector<pair<int, int>> > scene1CharacterPos =
{
    {"Chris", { {450, 172}, {450,  400}, {700, 400}, {700, 172} } },
    {"Andy",  { {700, 172}, {700, 418}, {900, 418}, {900, 172} } },
};

map <string, vector<pair<int, int>> > scene2CharacterPos =
{
    {"Andy",    { {420, 172}, {420, 400}, {620, 400}, {620, 172} } },
    {"Angella", { {650, 172}, {650, 390}, {800, 390}, {800, 172} } },
    {"Emily" ,  { {800, 172}, {800, 390}, {940, 390}, {940, 172} } },
    {"roach" ,  { {70, 500},  {70, 550},  {120, 550}, {120, 500} } },
};

map <string, vector<pair<int, int>> > scene3CharacterPos =
{
    {"Chris",   { {160, 172}, {160, 400}, {410, 400}, {410, 172} } },
    {"Andy",    { {400, 172}, {400, 400}, {600, 400}, {600, 172} } },
    {"John",    { {600, 172}, {600, 400}, {790, 400}, {790, 172} } },
    {"Emily",   { {800, 172}, {800, 400}, {940, 400}, {940, 172} } },
    {"Dan",     { {930, 172}, {930, 400}, {1115, 400}, {1115, 172} } },
};

map <string, vector<pair<int, int>> > scene4CharacterPos =
{
    {"Chris",   { {160, 172}, {160, 400}, {410, 400}, {410, 172} } },
    {"Andy",    { {400, 172}, {400, 400}, {600, 400}, {600, 172} } },
    {"John",    { {600, 172}, {600, 400}, {790, 400}, {790, 172} } },
    {"Emily",   { {800, 172}, {800, 400}, {940, 400}, {940, 172} } },
    {"Dan",     { {930, 172}, {930, 400}, {1115, 400}, {1115, 172} } },
};

map <string, vector<pair<int, int>> > scene6CharacterPos =
{
    {"Dan",     { {530, 172}, {530, 400}, {715, 400}, {715, 172} } },
};

//Array of characters and their postion in the scene
vector<map <string, vector<pair<int, int>> >> SceneCharacterPos
{
    scene1CharacterPos,
    scene2CharacterPos,
    scene3CharacterPos,
    scene3CharacterPos,
    scene3CharacterPos,
    scene6CharacterPos,
    scene3CharacterPos,
    scene3CharacterPos
};

#pragma endregion

// The Coordinates of each and every point to draw the object are stored in a 2D Matrix with each element being a pair
#pragma region CoordinateData


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
    { {145,375},{145,450} },                      //WindowRails - left Top vertical
    { {130,425},{160,425} },                      //WindowRails -  left Top horizontal
    { {145,250},{145,325} },                      //WindowRails -  left Bottom vertical
    { {130,300},{160,300} },                      //WindowRails - left Bottom horizontal
    { {545,375},{545,450} },                      //WindowRails - Right Top vertical
    { {530,425},{560,425} },                      //WindowRails - Right Top horizontal
    { {545,250},{545,325} },                      //WindowRails - Right Bottom vertical
    { {530,300},{560,300} },                      //WindowRails - Right Bottom horizontal
    { {325,460},{375,460} },                      //WindowRails - Middle Horizontal
    { {350,410},{350,485} },                      //WindowRails - Middle Vertical
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

vector<vector<pair<int, int>>> GenericRoomScene2 =
{
    { {0,240},{187, 419},{187, 588}, {0, 720} },            //0.LeftWall - Upper - Polygon - color grey
    { {1280, 240},{1093, 419},{1093, 588}, {1280, 720} },   //1.RightWall - Upper - Polygon 
    { {187, 588},{0, 720},{1280, 720}, {1093, 588} },       //2.//UpperWall - Upper - Polygon 
    { {187, 419},{187, 588},{1093, 588}, {1093, 419} },     //3. //FrontWall - Upper - Polygon 
    { {0,0}, {0, 240},{187, 419},{1093, 419}, {1280, 240} ,{1280,0}},        //4.  //Ground - Polygon
    { {50, 430},{50, 530},{110, 580}, {110, 480} },         //5.  //Window Body - Polygon glColor3f(1, 1, 1) // white
    { {80, 455},{80, 555} },                                //6.  //WindowLine - Vertical  - GL_LINES
    { {50, 480},{110, 530} },                               //7.  //WindowLine - Horizontal  - GL_LINES
    { {187, 588},{1093, 588} },                            //8. //UpperWallOutLine  - GL_LINES
    { {0, 241},{187, 419},{1093, 419}, {1280, 240} },     //9. //SplitLine  - GL_LINE_STRIP
    { {0, 241},{187, 419},{187, 588}, {0, 720} },             //10. //LeftWallOutline  - GL_LINE_STRIP glColor3f(0, 0, 0) // black; width = 2
    { {1280, 241},{1093, 419},{1093, 588}, {1280, 720} },     //11. //RightWallOutline  - GL_LINE_STRIP
    { {50, 430},{50, 530} , {110, 580}, {110, 480} },       //12.  //Window Outline  - GL_LINE_LOOP width = 3

};

vector<vector<pair<int, int>>> HallRoom =
{
    //RoomWalls
    { {0,-60}, {0,200} ,{179, 350} ,{1100, 350}, {1280, 200} ,{1280,-60}}, //0.Ground - POLYGON - color (0.35,0.27,0.22)
    { {0,200}, {179, 350} ,{179, 800} ,{0, 800} }, //1.Left Wall - POLYGON - color (0.35,0.27,0.22)
    { {1280, 200}, {1100, 350} ,{1100, 800} ,{1280, 800} }, //2.Right Wall - POLYGON - color (0.35,0.27,0.22)
    { {1100, 350}, {1100, 800} ,{179, 800} ,{179, 350} }, //3.Front Wall - POLYGON - color (0.35,0.27,0.22)

    //RoomOutline
    { {0,200} ,{179, 350} ,{1100, 350}, {1280, 200} },  //4.SplitLine - GL LINESTRIP - linewidth(2)
    { {179, 350} ,{179, 720} },                         //5.LeftVerticalLine - GL LINES - linewidth(2)
    { {1100, 350} ,{1100, 720} },                       //6.RightVerticalLine - GL LINES - linewidth(2)
    { {179 , 610} , {600 , 610}},                       //7.TopRailing Left - GL LINES - line width(2)
    { {179 ,560} , {600, 560}},                         //8.Bottom Railing Left - GL LINES - line width(2)
    { {1100 ,560} , {680 , 560} },                      //9.Top Railing right - GL LINES - line width(2)
    { {1100 ,610} , {680 , 610} },                      //10.Bottom Railing left - GL LINES - line width(2) 
};

vector<vector<pair<int, int>>> ColumnDesign =
{
    //ColumnDesign
    { {200 ,610} , {200 , 598} },                     //7.LineTop - GL LINES - LineWidht(2)
    { {200 ,598} , {188 , 585} },                     //8.LineLeftdiagTop - GL LINES - LineWidht(2)
    { {200 ,598} , {212 , 585} },                     //9.LineRightdiagTOp - GL LINES - LineWidht(2)
    { {188, 585} , {200 , 572} },                     //10.LineRightdiagDown - GL LINES - LineWidht(2)
    { {212, 585} , {200 , 572} },                     //11.LineleftdiagDown - GL LINES - LineWidht(2)
    { {200, 572} , {200 , 560} },                     //12.Middle LineDown - GL LINES - LineWidht(2)
    { {188 , 585} , {212 , 585} }                     //12.Middle Line Bisecting- GL LINES - LineWidht(2)
};

vector<vector<pair<int, int>>> Columns =
{
    //ColumnDesign
    { {180 , 610} , {220 , 610} , {220 , 560} , {180 , 560} }                               //ColumnOutlineCoordinates
};

vector<vector<pair<int, int>>> UnderColumnDesign =
{
    //UnderColumnDesign
    { {179 ,500} , {321 , 500} },                     //0.MidLineRight - GL LINES - LineWidht(2)
    { {580 ,500} , {438 , 500} },                     //1.MidLineLeft - GL LINES - LineWidht(2)
    { {321 , 500} , {379 , 550} , {438 , 500}, {379 , 450} },                     //2.InnerSquare - GL LINELOOP - LineWidht(2)
    { {276 , 500} , {379 , 595} , {483 , 500}, {379 , 395} },                     //3.OutterSquare - GL LINELOOP - LineWidht(2)
    
};

vector<vector<pair<int, int>>> Stairs =
{
    //BackGround
    { {530, 350}, {600, 560} ,{680, 560} ,{750, 350} }, //0. Stairs BG gl lines Polygon

    //Stairs
    { {530, 350}, {550, 370} ,{730, 370} ,{750, 350} }, //1. First Step Bottom
    { {550, 370}, {550, 400} ,{730, 400} ,{730, 370} }, //2. First Step Front

     { {550, 400}, {570, 420} ,{710, 420} ,{730, 400} }, //3. Second Step Bottom
     { {570, 420}, {570, 450} ,{710, 450} ,{710, 420} }, //4. Second Step Front

    { {570, 450}, {590, 470} ,{690, 470} ,{710, 450} }, //5. third Step bottom
    { {590, 470}, {590, 500} ,{690, 500} ,{690, 470} }, //6. third Step Front

    { {590, 500}, {610, 520} ,{670, 520} ,{690, 500} }, //7. fourth step bottom
    { {610, 520}, {610, 550} ,{670, 550} ,{670, 520} }, //8. fourth step Front

    { {610, 550}, {630, 560} ,{650, 560} ,{670, 550} }, //9. fifth step reach

    { {530, 350}, {600, 560} ,{680, 560} ,{750, 350} }, //10. Boudanry


    //RailingColumn
    { {530, 410} , {530, 350} }, //11. left railing column 1
    { {600, 550} , {600, 610} }, //12. left railing column 2

    { {750, 410} , {750, 350} }, //13. right railing column 1
    { {680, 550} , {680, 610} }, //14. right railing column 2


    //Railings
    { {600, 610} , {530, 410} }, //15. left railing
    { {680, 610} , {750, 410} }, //16. right railing

    


    


};

vector<vector<pair<int, int>>> Chair =
{
    { {200, 250},{260, 230},{320, 260}, {260, 280} },       //0.Chair sitting - glColor3f(1, 0.64, 0); (chair brown color) - Polygon
    { {200, 252},{200, 350},{260, 370}, {260, 282} },       //1.Chair Top - glColor3f(1, 0.64, 0); (chair brown color) - Polygon
    { {200, 251},{260, 281} },                              //2.Chair Joiner - glColor3f(1, 1, 1); Lines - glLineWidth(3);
    { {200, 250},{200, 200} },                              //3.Chair Leg left - glColor3f(0, 0, 0); Lines - glLineWidth(2);
    { {260, 230},{260, 180} },                              //4.Chair Leg middle - glColor3f(0, 0, 0); Lines - glLineWidth(2);
    { {320, 260},{320, 210} },                              //5.Chair Leg right - glColor3f(0, 0, 0); Lines - glLineWidth(2);
    { {200, 250},{260, 230},{320, 260}, {260, 280},{200, 250} },       //6.Chair sitting outline - GLline strip
    { {200, 252},{200, 350},{260, 370}, {260, 282},  {200, 252} },       //7.Chair top outline - GLline strip
};

vector<vector<pair<int, int>>> Lamp =
{
    { {1000, 200},{1050, 200},{1050 , 260}, {1000, 260} },       //0.LampBase - POLYGON
    { {1020, 260},{1020, 450},{1030 , 450}, {1030, 260} },       //1.LampSuport - POLYGON
    { {950, 450},{975, 500},{1075 , 500}, {1100, 450} },       //2.LampLight - POLYGON
};

vector<vector<pair<int, int>>> Sofa =
{
    { {100, 200},{100, 380},{300, 390}, {300, 210} },       //0.sofa back rest - polygon - glcolor3f(0.05,0.32,0.59)
    { {100, 200},{100, 290},{150, 260}, {150, 175} },       //1.sofa leftsize rest - polygon - glcolor3f(0,0,1)
    { {300, 270},{300, 320},{350, 300}, {350, 225} },       //2.sofa rightsize rest - polygon - glcolor3f(0,0,1)
    { {150, 260},{300, 270},{350, 235}, {150, 215} },       //3.sofa sitting  - polygon - glcolor3f(0,0,1),
    { {150, 215},{350, 235},{350, 200}, {150, 175} },       //4.sofa bottom  - polygon - glcolor3f(0.05,0.32,0.59)
    { {150, 195},{350, 217} },                              //5.LineDivider  - Line 
    { {100, 200},{100, 380}, {300, 390}, {300, 270} },       //6.UpperOutline  - LineStrip
    { {150, 260},{300, 270},{350, 235}, {150, 215} ,{150, 260}  } ,      //7.SofaSittingOutline  - LineStrip
    { {100, 200},{100, 290},{150, 260}, {150, 175}, {100, 200}  },       //8.SofaLeftOutline  - LineStrip
    { {300, 270},{300, 320},{350, 300}, {350, 225}},       //9.SofaRightOutline  - LineStrip
    { {150, 215},{350, 235},{350, 200}, {150, 175}},       //10.SofaBottomOutline  - LineStrip
                             
};

vector<vector<pair<int, int>>> Cupboard =
{
     { {900, 150},{900, 430},{1094, 430}, {1094, 150} },    //0.CupBoardBody - Polygon - glColor3f(1, 0, 0);
     { {900, 150},{900, 100},{920, 100}, {920, 150} },      //1.Cupboard left leg - Polygon - glColor3f(0, 0, 0);
     { {1074, 150},{1074, 100},{1094, 100}, {1094, 150} },  //2.Cupboard right leg - Polygon - glColor3f(0, 0, 0);
     { {997, 150},{997, 430} },                             //3.Cupboard Line - GL_LINES - glColor3f(0, 0, 0); - glLineWidth(3);
     { {985, 290} },                                        //4.Cupboard Knob left - GL_POINTS - glColor3f(0, 0, 0); - glPointSize(5);
     { {1005, 290} },                                       //5.Cupboard Knob left - GL_POINTS - glColor3f(0, 0, 0); - glPointSize(5);
     { {900, 430},{920, 500},{1074, 500}, {1094, 430} },    //6.Top design - GL_LINE_STRIP - glColor3f(0, 0, 0); - glLineWidth(2);
     { {900, 150},{900, 430},{1094, 430}, {1094, 150} , {900, 150}},    //7.CupboardBody outline - GL_LINE_STRIP - glColor3f(0, 0, 0); - glLineWidth(2);
};

vector<vector<pair<int, int>>> ClockLines =
{
     { {655, 520},{675, 520} },                             //0.LeftMark
     { {745, 520},{725, 520} },                             //1.RightMark
     { {700, 565},{700, 545} },                             //2.TopMark
     { {700, 475},{700, 495} },                             //3.Bottom Mark
     { {700, 520},{680, 530} },                             //4.ClockHourhand
     { {700, 520},{720, 550} },                             //5.ClockMinutehand
     { {699, 521} },                                        //6.Clock Center - Point
};

#pragma endregion

// Objects are draw on to the frame with value present in COORDINATE DATA region
//The way this is done is , by iterating the 2D matrix values for the respective object present in the COORDINATE DATA.
//The outer for loop iterates through each shape (line , polygon , etc) and sets its colors depending on the index values
//The inner for loop iterates through each coordinate in the shape and draw the shape
//This is done for as many shapes that constitute the Object
#pragma region DrawObjects

#pragma region DrawChair
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
        if (i >= 2 && i <= 5) { glBegin(GL_LINES); }
        if (i >= 5 && i <= 7) { glBegin(GL_LINE_STRIP); }

        //Drawing Shape
        for (int j = 0; j < Chair[i].size(); j++)
        {
            glVertex2f(Chair[i][j].first, Chair[i][j].second);
        }

        glEnd();
    }
}
#pragma endregion

#pragma region DrawCupboard
void DrawCupBoard()
{
    for (int i = 0; i < Cupboard.size(); i++)
    {
        //SortingColors
        if (i == 0)
        {
           glColor3f(1, 0, 0); //red  
        }
        else { glColor3f(0, 0, 0); } //black

        //LineWidth and PointSize
        if (i == 3) { glLineWidth(3); }
        if (i == 4 || i == 5) { glPointSize(5); }
        if (i == 6) { glLineWidth(2); }

        //Shape
        if (i < 3) { glBegin(GL_POLYGON); }
        if (i == 3) { glBegin(GL_LINES); }
        if (i < 6) { glBegin(GL_POINTS); }
        if (i >= 6) { glBegin(GL_LINE_STRIP); }

        //DrawingShape
        for (int j = 0; j < Cupboard[i].size(); j++)
        {
            glVertex2f(Cupboard[i][j].first, Cupboard[i][j].second);
        }

        glEnd();
    }
}

#pragma endregion

#pragma region DrawClock
void DrawClock()
{
    //DrawClock
    //Outline
    int x = 700, y = 520, r = 50;
    glBegin(GL_POLYGON);

    glColor3f(0, 0, 0); // black
    for (int i = 0; i < 360; i++) { float theta = i * 3.14 / 180;  glVertex2f(x + r * cos(theta), y + r * sin(theta)); } //drawCircle
    glEnd();

    //Face outline
    glBegin(GL_POLYGON);
    x = 700, y = 520, r = 45;

    glColor3f(1, 1, 1); //white
    for (int i = 0; i < 360; i++) { float theta = i * 3.14 / 180;  glVertex2f(x + r * cos(theta), y + r * sin(theta)); }  //drawCircle
    glEnd();

    glColor3f(0, 0, 0); //black
    for (int i = 0; i < ClockLines.size(); i++)
    {
        //Shape
        if (i != 6) { glBegin(GL_LINES); }
        if (i == 6) { glBegin(GL_POINTS); }

        //DrawingShape
        for (int j = 0; j < ClockLines[i].size(); j++)
        {
            glVertex2f(ClockLines[i][j].first, ClockLines[i][j].second);
        }
        glEnd();
    }
}

#pragma endregion

#pragma region DrawSofa
void DrawSofa()
{
    for (int i = 0; i < Sofa.size(); i++)
    {
        //Sorting Colors
        if (i == 0 || i == 4) { glColor3f(0.20, 0.23, 0.30); }
        else{ glColor3f(1, 0.64, 0); }

        //Sorting shapes and colors
        if (i == 5)
        {
            glColor3f(0, 0, 0);
            glBegin(GL_LINES);
        }
        if (i >= 6 && i <= 10)
        {
            glColor3f(0, 0, 0);
            glBegin(GL_LINE_STRIP);
        }
        else { glBegin(GL_POLYGON); }

        //Drawing part of the object
        for (int j = 0; j < Sofa[i].size(); j++)
        {
            glVertex2f(Sofa[i][j].first + 60, Sofa[i][j].second + 10);
        }

        glEnd();
    }
}
#pragma endregion

#pragma region DrawLamp
void DrawLamp()
{
    for (int i = 0; i < Lamp.size(); i++)
    {
        //Sorting colors
        if (i == 2){ glColor3f(1, 1, 0); }
        else{ glColor3f(0, 0, 0); }

        glBegin(GL_POLYGON);

        for (int j = 0; j < Lamp[i].size(); j++)
        {
            glVertex2f(Lamp[i][j].first, Lamp[i][j].second);
        }

        glEnd();
    }
}
#pragma endregion

#pragma endregion

#pragma region Queue Implementation

//------------------------QUEUE CLASS---------------//
//Class that holds blueprint for the queue
class QueueText {
public:
    int front, rear, size;
    unsigned capacity;
    char* array;
};


//----------------QUEUE FUNCTIONS--------------------//
QueueText* queue;

//Creates fresh queue with all values initialized to zero
QueueText* createQueue(unsigned capacity)
{
    QueueText* queueText = new QueueText();
    queueText->capacity = capacity;
    queueText->front = queueText->size = 0;

    //queueText->rear = capacity - 1;
    queueText->array = new char[queueText->capacity];
    return queueText;
}


//Checks if the queue is full
bool isFull(QueueText* queueText)
{
    return (queueText->size == queueText->capacity);
}

//Checks if the queue is empty
bool isEmpty(QueueText* queueText)
{
    return (queueText->size == 0);
}

//Adds a character into the queue
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

//Removes an element from queue
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

//Retrivies the element present at the front of the queue
int front(QueueText* queueText)
{
    if (isEmpty(queueText))
    {
        return INT_MIN;
    }

    return queueText->array[queueText->front];
}

//Retrivies the element present at the rear of the queue
int rear(QueueText* queueText)
{
    if (isEmpty(queueText))
    {
        return INT_MIN;
    }

    return queueText->array[queueText->rear];
}

//----------------QUEUE FUNCTIONS END--------------------//
#pragma endregion

//Draws the dialouge Box
#pragma region DialougeBoxRendering
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

#pragma endregion

//renders the text with a smooth animation
#pragma region RenderTextOnScreen
//----------------RENDERING TEXT ON SCREEN--------------//

//Draws text on screen using the glutBitmapCharacter function
//it takes character arry , x position , y poisition and color for rending text on screen
void DrawDialouge(char* string, int x, int y, int color)
{
    //sorting colors
    if (color == 1) { glColor3f(1, 1, 1); } //white
    else if (color == 0) { glColor3f(0, 0, 0); }  // black

    int len, i;
    glRasterPos2f(x, y);
    len = (int)strlen(string);

    //Drawing text
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
    }
}

//----------------RENDERING TEXT ON SCREEN END--------------//
#pragma endregion

//Draw a simple Click to Continue text and 2 polygons as arrows
#pragma region ClickToContinue
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
#pragma endregion

//Draws backgroung for the scenes
#pragma region DrawScenes
//Draws the basic room
//takes scene as input cause we have 2 room but diff colors
void DrawRoomBG(int scene)
{
    for (int i = 0; i < GenericRoom.size(); i++)
    {
        //Sorting Colors
        if (i < 4) 
        {
            if (scene == 0)
            {
                glColor3f(1, 0.99, 0.81); // cream color
            }
            else
            {
                glColor3f(0.34, 0.33, 0.33); // grey
            }
           
        } // Cream Color
        else if (i < 8)
        {
            if (scene == 0)
            {
                glColor3f(0.58, 0.43, 0.20); // wood
            }
            else
            {
                glColor3f(0.83, 0.01, 0.16); // red tint
            }
            
        } // Wood Color

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

void DrawHall()
{
    for (int i = 0; i < HallRoom.size(); i++)
    {
        if (i == 0)
        {
            glColor3f(0.28, 0.25, 0.23); //Dark Chocolate
        }
        else if( i >= 0)
        {
            glColor3f(0.96, 0.96, 0.86);
        }

      
        if (i <= 3)
        {
            glBegin(GL_POLYGON);
        }
        else if(i >= 4)
        {
            glLineWidth(4);
            glColor3f(0, 0, 0);
            glBegin(GL_LINE_STRIP);
        }
        else
        {
            glLineWidth(2);
            glColor3f(0, 0, 0);
            glBegin(GL_LINES);
        }
       

        for (int j = 0; j < HallRoom[i].size(); j++)
        {
            glVertex2f(HallRoom[i][j].first, HallRoom[i][j].second+60);
        }
        glEnd();
    }
}

void DrawColumnDesign()
{
    int increment = 0;

    //Left Columns
    for (int i = 0; i < 4; i++)
    {
        for (int i = 0; i < ColumnDesign.size(); i++)
        {
            glLineWidth(3);
            glColor3f(0, 0, 0);
            glBegin(GL_LINES);

            for (int j = 0; j < ColumnDesign[i].size(); j++)
            {
                glVertex2f(ColumnDesign[i][j].first+increment, ColumnDesign[i][j].second + 60);
            }
            glEnd();
        }
        increment += 100;
    }

    //Right Columns
    increment = 570;
    for (int i = 0; i < 4; i++)
    {
        for (int i = 0; i < ColumnDesign.size(); i++)
        {
            glLineWidth(3);
            glColor3f(0, 0, 0);
            glBegin(GL_LINES);

            for (int j = 0; j < ColumnDesign[i].size(); j++)
            {
                glVertex2f(ColumnDesign[i][j].first + increment, ColumnDesign[i][j].second + 60);
            }
            glEnd();
        }
        increment += 100;
    }
}

void DrawColumn()
{
    int increment = 0;

    //Left Columns
    for (int i = 0; i < 4; i++)
    {
        for (int i = 0; i < Columns.size(); i++)
        {
            glColor3f(0.28, 0.25, 0.23);
            glBegin(GL_POLYGON);

            for (int j = 0; j < Columns[i].size(); j++)
            {
                glVertex2f(Columns[i][j].first + increment, Columns[i][j].second + 60);
            }
            glEnd();
        }
        increment += 100;
    }

    //Right Columns
    increment = 570;
    for (int i = 0; i < 4; i++)
    {
        for (int i = 0; i < Columns.size(); i++)
        {
            glColor3f(0.28, 0.25, 0.23);
            glBegin(GL_POLYGON);

            for (int j = 0; j < Columns[i].size(); j++)
            {
                glVertex2f(Columns[i][j].first + increment, Columns[i][j].second + 60);
            }
            glEnd();
        }
        increment += 100;
    }
}

void DrawUnderColumnDesign()
{
    //Left Columns
    for (int i = 0; i < UnderColumnDesign.size(); i++)
    {
        glColor3f(1, 0.84, 0);

        if (i < 2)
        {
            glLineWidth(2);
            glBegin(GL_LINES);
        }
        else
        {
            glLineWidth(2);
            glBegin(GL_LINE_LOOP);
        }

        for (int j = 0; j < UnderColumnDesign[i].size(); j++)
        {
            glVertex2f(UnderColumnDesign[i][j].first, UnderColumnDesign[i][j].second + 20);
        }
        glEnd();
    }
   

    
    //Right Columns
    for (int i = 0; i < UnderColumnDesign.size(); i++)
    {
        glColor3f(1, 0.84, 0);

        if (i < 2)
        {
            glBegin(GL_LINES);
        }
        else
        {
            glBegin(GL_LINE_LOOP);
        }

        for (int j = 0; j < UnderColumnDesign[i].size(); j++)
        {
            glVertex2f(UnderColumnDesign[i][j].first+520, UnderColumnDesign[i][j].second + 20);
        }
        glEnd();
    }
   
    
}

void DrawStairs()
{
    for (int i = 0; i < Stairs.size(); i++)
    {
        glColor3f(0.28, 0.25, 0.23);
        glLineWidth(2);

        if (i >= 11)
        {
            
            glLineWidth(10);
        }

        if (i == 0)
        {
            glBegin(GL_POLYGON);
        }
        else
        {
            if (i >= 11 && i <= 14)
            {
                glColor3f(1, 0.84, 0);
            }
            else if( i >= 15 && i<= 18)
            {
                glColor3f(0.18, 0.15, 0.13);
            }
            else
            {
                glColor3f(0, 0, 0);
            }
             
            glBegin(GL_LINE_LOOP);
        }

        for (int j = 0; j < Stairs[i].size(); j++)
        {
            glVertex2f(Stairs[i][j].first, Stairs[i][j].second + 60);
        }
        glEnd();
    }
}

#pragma endregion

//We set the color for the character BG so that it matches with the scene BG
void DrawCharacterBG(int scene)
{
    switch (scene)
    {
    case 0:
        glColor3f(0.58, 0.43, 0.20); // cream color
        break;
    case 1:
        glColor3f(0.83, 0.01, 0.16); // red tint
        break;
    case 2:
    case 4:
    case 6:
    case 7:
        glColor3f(0.28, 0.25, 0.23); //dark chocolate
        break;
    case 5:
        glColor3f(0, 0, 0); //Black
        break;
    }
}

#pragma region CharacterRendering
//We load the character in to the scene
//We take 3 parameters here - The coordinates where the image should be drawn , The character who image we are rendering and optional color 
void LoadCharacter(vector<pair<int, int>> characterImageCoordinates, string characterImageName, int color)
{
    //if no color set , draw the color as that of scene
    if (color == 0)
    {
        DrawCharacterBG(currentScene);
    }
    else
    {
        glColor3f(0.34, 0.33, 0.33);//grey color
    }

    //Storing the name of the character in char array
    char characterS[100] = "";
    for (int i = 0; i < characterImageName.length(); i++) { characterS[i] = characterImageName[i]; }
    strcat_s(characterS, ".png"); //appending .png so that we get the name of the image file to load


    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //Clears the pixel image data so that we can draw new images

    glGenTextures(1, &characterTexture); //Returns a texture , we pass 1 , cause we want only 1 texture and pointer to the reference variable
    stbi_set_flip_vertically_on_load(true); //PNG images on loading are upside down for some reason so we flip it
    int width, height, nrChannels;

    //the stbi_load function is used to convert image dat into a contiguous array of char blocks , which can be mapped to the texture and draw as a image
    //It takes 4 parameters - char array which holds the path of the file with respect to the main cpp file , the width of the image , the height of the image
    // and the number of color channels and the number of components per image
    unsigned char* character = stbi_load(characterS, &width, &height, &nrChannels, 0); 

    glBindTexture(GL_TEXTURE_2D, characterTexture); //It binds a texture to second variable passed

    if (character != NULL)
    {
        //this function maps the texture image with the given char array block
        //We set GL_RGBA because we are working with PNG files
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, character);
        cout << "Character loaded" << endl;
    }
    else
    {
        cout << "Failed to load character" << endl;
    }
    //freeing the image data in ram
    stbi_image_free(character);

    //displaying

    //Setting the texture environment parameters
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

    //Enabling the texture
    glEnable(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //we set the texture to repeat on x
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //we set the texture to repeat on y
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //we set the filter on GL_LINEAR so that when we scale down the image, it does it by blurring some parts
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //we set the filter on GL_LINEAR so that when we scale up the image, it does it by blurring some parts
    glBindTexture(GL_TEXTURE_2D, characterTexture); //It binds a texture to second variable passed

    //Drawing the shape on which the texture should be mapped on
    //We draw a quad and the texture is mapped on that quad
    //The glTexCoord2f specfies if the texture should repeated or streached , by giving the points to 1 we are doing none
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);   glVertex2f(characterImageCoordinates[0].first, characterImageCoordinates[0].second);
    glTexCoord2f(0.0f, 1.0f);   glVertex2f(characterImageCoordinates[1].first, characterImageCoordinates[1].second);
    glTexCoord2f(1.0f, 1.0f);   glVertex2f(characterImageCoordinates[2].first, characterImageCoordinates[2].second);
    glTexCoord2f(1.0f, 0.0f);   glVertex2f(characterImageCoordinates[3].first, characterImageCoordinates[3].second);
    glEnd();

    glDeleteTextures(1, &characterTexture); // we delete the texture for the reference variables so that we can draw new ones
}

//Loading all the character if the player is speaking
//it takes the following parameters - Character and optional color
void LoadAllCharacters(map <string, vector<pair<int, int>> > Characters, int color)
{
    //Loop through all characters in the array and display then one by one
    for (int i = 0; i < Characters.size(); i++)
    {
        char characterS[100] = "";
        for (const auto& character : Characters) { //foreach loop to loop through each element , we use const auto& so that map is not modified
            if (character.first == "roach")
            {
                LoadCharacter(character.second, character.first, 1);
            }
            else
            {
                LoadCharacter(character.second, character.first, color);
            }
        }
    }
}

//Draws the character based on who is speaking
void DrawCharacter(string Speaker)
{
    if (Speaker == "Narrator" || Speaker == "????") { return; }

    if (Speaker == "You")
    {
        LoadAllCharacters(SceneCharacterPos[currentScene], 0);
    }
    else
    {
        LoadCharacter(SceneCharacterPos[currentScene][Speaker], Speaker, 0);
    }
}
#pragma endregion

//Draws the background for each scene
#pragma region DrawScenes

void DrawScene1BG()
{
    DrawRoomBG(currentScene);
    DrawSofa();
    DrawCupBoard();
    DrawClock();

    if (NewScene) //We give a 1 second pause when we see a first for the first time so players can see the room without characters
    {
        NewScene = false;
        glFlush();
        Sleep(1000);
    }
}

void DrawScene2BG()
{
    DrawRoomBG(currentScene);
    DrawChair();
    DrawLamp();
    DrawClock();

    if (currentDialouge < 3) //the roach is present only for the first 3 dialouges
    {
        LoadCharacter(scene2CharacterPos["roach"], "roach", 1);
    }


    if (NewScene) //We give a 1 second pause when we see a first for the first time so players can see the room without characters
    {
        NewScene = false;
        glFlush();
        Sleep(1000);
    }
}

void DrawScene3BG()
{
    DrawHall();
    DrawColumn();
    DrawColumnDesign();
    DrawUnderColumnDesign();
    DrawStairs();

    if (NewScene)  //We give a 1 second pause when we see a first for the first time so players can see the room without characters
    {
        NewScene = false;
        glFlush();
        Sleep(1000);
    }
}

void DrawScene4BG()
{
    //do nothing , this scene is for flash backs
}

//Here we draw BG's based on the scene at we are in
void DrawCurrentSceneBG(int scene)
{
    if (scene > 3)
    {
        NewScene = false;
    }

    switch (scene)
    {
    case 0:
        DrawScene1BG();
        break;

    case 1:
        DrawScene2BG();
        break;
    case 2:
    case 4:
    case 6:
    case 7:
        DrawScene3BG();
        break;

    case 3:
    case 5:
        DrawScene4BG();
        break;
    }
}

#pragma endregion

#pragma region FrameFunctions
void ClearFrame()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glEnd();
    glFlush();
}

void RecoverFrame()
{
    DrawCurrentSceneBG(currentScene); 
}
#pragma endregion

#pragma region DialougeSystem
bool CheckOverFlow(string& dialouge, string& text)
{
    if (dialouge.length() > 120)
    {
        text = dialouge.substr(0, 120);
        dialouge.erase(0, 120); //erases first 120 characters
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

#pragma endregion

#pragma region BranchingLogic

void RenderOptions()
{
    string fc = BranchADialouges[branchCounter][0].second;
    string sc = BranchBDialouges[branchCounter][0].second;
    char FirstChoice[200] ="";
    char SecondChoice[200] = "";

    for (int i = 0; i < fc.length(); i++)
    {
        FirstChoice[i] = fc[i];
    }

    for (int i = 0; i < sc.length(); i++)
    {
        SecondChoice[i] = sc[i];
    }

    //DrawingOptionsbox1
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0, 0, 0 , 0.75);
    glBegin(GL_POLYGON);
    glVertex2f(400, 500);
    glVertex2f(400, 700);
    glVertex2f(1000, 700);
    glVertex2f(1000, 500);
    glEnd();
    DrawDialouge(FirstChoice, 420, 600 , 1);

    glColor4f(0, 0, 0 , 0.75);
    glBegin(GL_POLYGON);
    glVertex2f(400, 200);
    glVertex2f(400, 400);
    glVertex2f(1000, 400);
    glVertex2f(1000, 200);
    glEnd();
    DrawDialouge(SecondChoice, 420, 300, 1);
    glDisable(GL_BLEND);

    glFlush();

}

bool CheckIfBranching()
{
    switch (currentScene)
    {
    case 6:
        return true;

    default:
        return false;
    }
}
#pragma endregion

#pragma region MouseCallback
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
                Scene = SCENE;
                break;

            case SCENE:
                if (currentDialouge == SceneDialouges[currentScene].size() - 1)
                {
                    if (CheckIfBranching())
                    {
                        Scene = CHOOSING;
                    }
                    else
                    {
                        currentScene++;
                        currentDialouge = 0;
                        NewScene = true;
                        
                        //Scene = TOBECONTINUED;
                    }
                    
                }
                else
                {
                    currentDialouge++;
                }
                break;

            case SCENEA:
                if (currentDialouge == BranchADialouges[branchCounter].size() - 1)
                {
                        currentScene++;
                        branchCounter++;
                        Scene = SCENE;
                        currentDialouge = 0;
                        //Scene = TOBECONTINUED;
                }
                else
                {
                    currentDialouge++;
                }
                break;

            case SCENEB:
                if (currentDialouge == BranchBDialouges[branchCounter].size() - 1)
                {
                    currentScene++;
                    branchCounter++;
                    Scene = SCENE;
                    currentDialouge = 0;
                    //Scene = TOBECONTINUED;
                }
                else
                {
                    currentDialouge++;
                }
                break;

            case CHOOSING:  
                if (x > 400 && x < 1000 && y > 40 && y < 220) { Scene = SCENEA; currentDialouge = 1;}
                if (x > 400 && x < 1000 && y > 320 && y < 510) { Scene = SCENEB; currentDialouge = 1;}
                break;

            default:
                break;
            }
        }
}
#pragma endregion

#pragma region HomeScreen

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
        if (i < 6) { glColor3f(0, 0.22, 0.39); }
        else { glColor3f(1, 1, 1); }

        if (i < 12) { glBegin(GL_POLYGON); }

        else { glColor3f(0, 0, 0);  glLineWidth(3);  glBegin(GL_LINES); }

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
#pragma endregion

#pragma region DescriptionScene
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

#pragma endregion


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
    char Buffer[400] = "";
    string dialuoge;
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

    case SCENE:
        queue = createQueue(1000);//create a fresh queue
        DrawCurrentSceneBG(currentScene);
        RecoverFrame();
        DrawCharacter(SceneDialouges[currentScene][currentDialouge].first);
        DrawDialougeBox();
        RenderSpeaker(SceneDialouges[currentScene][currentDialouge].first);
        AnimateText(SceneDialouges[currentScene][currentDialouge].second);
        DrawClickToContinue();
        break;

    case CHOOSING:
        queue = createQueue(1000);//create a fresh queue
        DrawScene1BG();
        RecoverFrame();
        DrawCharacter(SceneDialouges[currentScene][currentDialouge].first);
        DrawDialougeBox();
        RenderSpeaker(SceneDialouges[currentScene][currentDialouge].first);
        RenderOptions();
        
        dialuoge = SceneDialouges[currentScene][currentDialouge].second;
        for (int i = 0; i < dialuoge.length(); i++)
        {
            Buffer[i] = dialuoge[i];
        }
        DrawDialouge(Buffer, DialougeXOffset, FirstLineY, 1);
        break;

        //see above and rewrite dialouges
    case SCENEA:
        queue = createQueue(1000); //create a fresh queue
        DrawCurrentSceneBG(currentScene);
        RecoverFrame();
        DrawCharacter(BranchADialouges[branchCounter][currentDialouge].first);
        DrawDialougeBox();
        RenderSpeaker(BranchADialouges[branchCounter][currentDialouge].first);
        AnimateText(BranchADialouges[branchCounter][currentDialouge].second);
        DrawClickToContinue();
        break;

    case SCENEB:
        queue = createQueue(1000); //create a fresh queue
        DrawCurrentSceneBG(currentScene);
        RecoverFrame();
        DrawCharacter(BranchBDialouges[branchCounter][currentDialouge].first);
        DrawDialougeBox();
        RenderSpeaker(BranchBDialouges[branchCounter][currentDialouge].first);
        AnimateText(BranchBDialouges[branchCounter][currentDialouge].second);
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
    "and were very excited. They began their journey 3 days from then in John's car and reached there the next night.  ",
    };

    SceneDialouges[0] = {
    {"Narrator" , "As they entered , they were awestruck looking at the magnificent mansion and start exploring it and checked out the    rooms they were going to stay in and started unpacking their luggages. "},
    {"Chris" , "Hey "+ PlayerName + " , Andy come check this room out..... its even bigger than my living room!! "},
    {"You" , "Wow.... my whole family can stay here. "},
    { "Andy" , "Ha ha ha... C'mon it isn't that big. But I have been wondering how did Dan get this mansion for such a low price.       Something doesnt feel right. "},
    {"Chris" , "C'mon Andy.... why are you like this, overthinking about everything, we are here to relax... we dont get this           opportunity everyday. "},
    {"You" , "Yes Chris is right... we are here to release our stress so dont increase it by overthinking. "},
    {"Andy" ,  "Yeah I think you guys are right.... if only I was a little more carefree like you guys. We are in this beautiful        mansion in this peaceful place and we are going to make the best of it. Besides, what can go wrong? "},
    {"????" ,  "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAHHHH!!!!! (Sudden scream from the other room....) "},
    {"You" ,  "I think I just heard Angela screaming...... we must hurry up and check what happened!! "},
    {"Narrator" ,  "They rush towards the scream.... "}
    };

    SceneDialouges[1] = {
    {"Narrator" , "Scene 2 , They have reached the source of the scream. "},
   {"You" , "ANGELLA!!! What happened why did you scream??? Are you alright?!! "},
   {"Angella" , "I just saw a huge cockroach over there.... aaaahhh so scary  :'-( "},
   { "Andy" , "Angela... you are so silly. By the way where is Emily ? ?  (^o^) "},
   { "Emily" , "Im here, i ran and hid in the washroom after i saw the cockroach  :'-( "},
   {"Angella" , "Im so sorry you guys.. I just got shocked seeing it all of a sudden.. let's go to bed, we have to go out tomorrow. "},
   {"Narrator" , "They head to their respective room to rest up for tomorrow. "}
    };

    SceneDialouges[2] = {
   {"Emily" , "Good Morning everyone...what a lovely morning. "},
   {"Andy" , "Yeah weather is really good outside..i hope nothing goes wrong "},
   {"Dan" , "Ohh wow Andy go ahead and jinx the day...today is a good day don’t decimate it for us. "},
   { "Emily" , "Stop Dan why are you always after his life...don’t mind him Andy he doesn’t mean any harm. "},
   { "John" , "Don’t worry you guys everything is planned out nothing will go wrong. "},
   { "You" , "I knew we can trust you John....so Andy there is nothing to worry about if John has planned then you know its going to be perfect...By the way where is Angela?? "},
   {"Emily" , "I tried waking her up in the morning but she didn’t respond...she was very tired yesterday so i thought i will let her sleep  a little more longer and after that i got ready and came here. "},
   {"Andy" , "Ohh is that so... i will just go and wake her up. "},
   {"Dan" , "AAGHHH...this means we will be late..i hope that girl won’t take time. "},
   {"Andy" , "AAAANNNNGEEELLLAAAAAAAAAAAAA!!!!!!!!!!!!! "},
    };

    SceneDialouges[3] = {
  {"Narrator" , "Everyone rushes into Angela’s room...... "},
  {"Narrator" , "And they tear up as they discover that angela is no more......... "},
  {"Narrator" , "Everyone is in utter disbelief and don’t understand what to do................... "},
  { "Narrator" , "John being quick on his feet calls for an ambulance, but they get to know that they won’t be able to come on time as   the roads were blocked due to landslide. "},
  { "Narrator" , "John informs the cops as well but the nearest police station is in the city and since the roads are blocked, they won’t be able to come that quickly. "},
  { "Narrator" , "A few hours later.... "},
    };

    SceneDialouges[4] = {
  {"Andy" , "OMG, poor angela, how can this happen.... "},
  {"Dan" , "yeah it is really shocking but we need to calm down and collect our thoughts "},
  {"Andy" , "Dan you don't have a heart...it looks like you don't even care that angela died. "},
  { "Emily" , "Andy is right you don't seem to be affected by Angela's death at all.. "},
  { "Andy" , "What I'm trying to say is that MAYBE YOU ARE THE ONE WHO KILLED ANGELA ....we all know you never liked her. "},
  { "Dan" , " WHAT THE HELL???...Are you being serious right now....how dare you accuse me of such heinous crime. "},
  { "Andy" , "We all know what kind of a person you are... i know you killed her. "+ PlayerName +" why don’t you speak up. "},
    };

    SceneDialouges[5] = {
 {"You" , "Did...Did Dan really do this????? "},
 {"You" , " I know him very well no matter how tough he acts in front of us he has a good heart. "},
 {"You" , "I can never forget the time when my parents died in a car accident 5 years ago... "},
 { "You" , "Only Dan was there to support me.... "},
 { "You" , "He was there for me whenever I needed him, he helped me overcome my depression... "},
  };

    SceneDialouges[6] = {
        {"Andy" ,  PlayerName + " Stop dreaming and speak up..I asked what do you think of this ? ? "},
    };

    SceneDialouges[7] = {
        {"Andy" ,  "I don't think its safe to be around him right now. We should lock him in some room. "},
        {"Emily" ,  "Yeah Andy is right we should lock this psycho. "},
        {"Dan" ,  "You are just blabbering about what could be the reason to kill her..but you are not bringing any proof on the table. "},
        {"You" ,  "You are right Dan but right now we have to lock you in otherwise things can get heated up and worsen everything. "},
        {"John" ,  "Yes Dan we have to calm things down right now, I dont believe that any of us killed her.It can be someone from outside. "},
        {"Dan" ,  "You are right John but the fact that Andy straight up blamed me for her death without any proof seems very odd to me... i need you to find who did.. i dont want the stigma of a killer on me. "},
        {"You" ,  "Dont worry Dan we will do something but for now you stay in this room. "},
    };


    //BRANCH A DIALOUGES
    BranchADialouges[0] = {
        {"You" , "I think we should not hurry up to the conclusions.. "},
        {"You" , "I think we should not hurry up to the conclusions and give Dan some chance to explain. "},
        {"John" , "I think " + PlayerName + " is right we should not point  fingers at each other without any solid proof. "},
        {"Dan" , "Thanks guys at least some people have brains unlike others. "}
    };


    //BRANCH B DIALOUGES
    BranchBDialouges[0] = {
       {"You" , "Yes Dan you have been hating on her from so long... "},
       {"You" , "Yes Dan you have been hating on her from so long...and we all know how you act when you get angry. "},
       {"John" , PlayerName +" is right, Dan you have had a lot of history with her and you do have a reason to kill her. "},
       {"Dan" , "Those things were in the past...we haven't fought in years. Everything was fine between us. "}
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