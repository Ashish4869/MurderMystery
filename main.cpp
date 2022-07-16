#include<GL/glut.h>
#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include <fstream>
#include<string>
#include<ctype.h>
#include <map>
#include<vector>
#include<math.h>
#define STB_IMAGE_IMPLEMENTATION    
#include "stb_image.h"

using namespace std;

//CONSTANTS
int TypingSpeed = 0; //20
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
enum Scenes { START, DESCRIPTION, SCENE, TOBECONTINUED, CHOOSING, SCENEA, SCENEB, THEEND, CREDITS, POSTCREDITS };
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
vector<pair<string, string>> Scene9DialougesWithSpeakers;
vector<pair<string, string>> Scene10DialougesWithSpeakers;
vector<pair<string, string>> Scene11DialougesWithSpeakers;
vector<pair<string, string>> Scene12DialougesWithSpeakers;
vector<pair<string, string>> Scene13DialougesWithSpeakers;
vector<pair<string, string>> Scene14DialougesWithSpeakers;
vector<pair<string, string>> Scene15DialougesWithSpeakers;
vector<pair<string, string>> Scene16DialougesWithSpeakers;
vector<pair<string, string>> Scene17DialougesWithSpeakers;
vector<pair<string, string>> Scene18DialougesWithSpeakers;
vector<pair<string, string>> Scene19DialougesWithSpeakers;
vector<pair<string, string>> Scene20DialougesWithSpeakers;
vector<pair<string, string>> Scene21DialougesWithSpeakers;

//BRANCH A
vector<pair<string, string>> Branch1ADialougesWithSpeakers;
vector<pair<string, string>> Branch2ADialougesWithSpeakers;
vector<pair<string, string>> Branch3ADialougesWithSpeakers;
vector<pair<string, string>> Branch4ADialougesWithSpeakers;
vector<pair<string, string>> Branch5ADialougesWithSpeakers;

//BRANCH B
vector<pair<string, string>> Branch1BDialougesWithSpeakers;
vector<pair<string, string>> Branch2BDialougesWithSpeakers;
vector<pair<string, string>> Branch3BDialougesWithSpeakers;
vector<pair<string, string>> Branch4BDialougesWithSpeakers;
vector<pair<string, string>> Branch5BDialougesWithSpeakers;

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
    Scene8DialougesWithSpeakers,
    Scene9DialougesWithSpeakers,
    Scene10DialougesWithSpeakers,
    Scene11DialougesWithSpeakers,
    Scene12DialougesWithSpeakers,
    Scene13DialougesWithSpeakers,
    Scene14DialougesWithSpeakers,
    Scene15DialougesWithSpeakers,
    Scene16DialougesWithSpeakers,
    Scene17DialougesWithSpeakers,
    Scene18DialougesWithSpeakers,
    Scene19DialougesWithSpeakers,
    Scene20DialougesWithSpeakers,
    Scene21DialougesWithSpeakers,
};

//Same as Scene Dialouges but for Branch A
vector<vector<pair<string, string>>> BranchADialouges =
{
    Branch1ADialougesWithSpeakers,
    Branch2ADialougesWithSpeakers,
    Branch3ADialougesWithSpeakers,
    Branch4ADialougesWithSpeakers,
    Branch5ADialougesWithSpeakers,
};

//Same as Scene Dialouges but for Branch B
vector<vector<pair<string, string>>> BranchBDialouges =
{
    Branch1BDialougesWithSpeakers,
    Branch2BDialougesWithSpeakers,
    Branch3BDialougesWithSpeakers,
    Branch4BDialougesWithSpeakers,
    Branch5BDialougesWithSpeakers,
};

//PostCredits Scene
vector<pair<string, string>> PostCreditDialouge;
vector<vector<pair<string, string>>> PostCreditDialouges =
{
    PostCreditDialouge
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
    {"Angela", { {650, 172}, {650, 390}, {800, 390}, {800, 172} } },
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

map <string, vector<pair<int, int>> > scene8CharacterPos =
{
    {"Chris",   { {160, 172}, {160, 400}, {410, 400}, {410, 172} } },
    {"Andy",    { {400, 172}, {400, 400}, {600, 400}, {600, 172} } },
    {"John",    { {600, 172}, {600, 400}, {790, 400}, {790, 172} } },
    {"Emily",   { {800, 172}, {800, 400}, {940, 400}, {940, 172} } },
};

map <string, vector<pair<int, int>> > scene6CharacterPos =
{
    {"Dan",     { {530, 172}, {530, 400}, {715, 400}, {715, 172} } },
};

map <string, vector<pair<int, int>> > scene11CharacterPos =
{
    {"John", { {450, 172}, {450,  400}, {700, 400}, {700, 172} } },
};

map <string, vector<pair<int, int>> > scene12CharacterPos =
{
    {"John", { {450, 172}, {450,  400}, {700, 400}, {700, 172} } },
     {"Dan",  { {700, 172}, {700, 418}, {900, 418}, {900, 172} } },
};

map <string, vector<pair<int, int>> > scene13CharacterPos =
{
    {"John", { {450, 172}, {450,  400}, {700, 400}, {700, 172} } },
     {"Dan",  { {700, 172}, {700, 418}, {900, 418}, {900, 172} } },
     {"Chris",  { {200, 172}, {200,  400}, {450, 400}, {450, 172} } },
};

map <string, vector<pair<int, int>> > scene16CharacterPos =
{
    {"Danny",     { {530, 172}, {530, 400}, {715, 400}, {715, 172} } },
};

//Array of characters and their postion in the scene
vector<map <string, vector<pair<int, int>> >> SceneCharacterPos
{
    scene1CharacterPos, //0
    scene2CharacterPos, //1
    scene3CharacterPos, //2
    scene3CharacterPos, //3
    scene3CharacterPos, //4
    scene6CharacterPos, //5
    scene3CharacterPos, //6
    scene3CharacterPos, //7
    scene8CharacterPos, //8
    scene8CharacterPos, //9
    scene8CharacterPos, //10
    scene8CharacterPos, //11
    scene11CharacterPos, //12
    scene12CharacterPos, //13
    scene13CharacterPos, //14
    scene8CharacterPos, //15
    scene16CharacterPos, //16
    scene8CharacterPos, //17
    scene8CharacterPos, //18
    scene3CharacterPos, //19
    scene16CharacterPos, //20
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
        else { glColor3f(1, 0.64, 0); }

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
        if (i == 2) { glColor3f(1, 1, 0); }
        else { glColor3f(0, 0, 0); }

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
            if (scene != 1)
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
            if (scene != 1)
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
        else if (i >= 0)
        {
            glColor3f(0.96, 0.96, 0.86);
        }


        if (i <= 3)
        {
            glBegin(GL_POLYGON);
        }
        else if (i >= 4)
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
            glVertex2f(HallRoom[i][j].first, HallRoom[i][j].second + 60);
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
                glVertex2f(ColumnDesign[i][j].first + increment, ColumnDesign[i][j].second + 60);
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
            glVertex2f(UnderColumnDesign[i][j].first + 520, UnderColumnDesign[i][j].second + 20);
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
            else if (i >= 15 && i <= 18)
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
    case 12:
    case 13:
    case 14:
        glColor3f(0.58, 0.43, 0.20); // cream color
        break;
    case 1:
        glColor3f(0.83, 0.01, 0.16); // red tint
        break;
    case 2:
    case 4:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 15:
    case 17:
    case 18:
    case 19:
        glColor3f(0.28, 0.25, 0.23); //dark chocolate
        break;
    case 5:
    case 16:
    case 20:
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
    char path[30] = "Assets\\";
    strcat_s(path, characterS); //Storing the path of the folder where the character assets lie
    strcat_s(path, ".png"); //appending .png so that we get the name of the image file to load


    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //Clears the pixel image data so that we can draw new images

    glGenTextures(1, &characterTexture); //Returns a texture , we pass 1 , cause we want only 1 texture and pointer to the reference variable
    stbi_set_flip_vertically_on_load(true); //PNG images on loading are upside down for some reason so we flip it
    int width, height, nrChannels;

    //the stbi_load function is used to convert image dat into a contiguous array of char blocks , which can be mapped to the texture and draw as a image
    //It takes 4 parameters - char array which holds the path of the file with respect to the main cpp file , the width of the image , the height of the image
    // and the number of color channels and the number of components per image
    unsigned char* character = stbi_load(path, &width, &height, &nrChannels, 0);

    glBindTexture(GL_TEXTURE_2D, characterTexture); //It binds a texture to second variable passed

    if (character != NULL)
    {
        //this function maps the texture image with the given char array block
        //We set GL_RGBA because we are working with PNG files
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, character);
    }
    else
    {
        //Failed to load character
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

    if (currentScene < 2)
    {
        DrawSofa();
    }
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
    if (scene > 3) //once we reach this scene we have seen all the scenes and dont want the 1sec gaps
    {
        NewScene = false;
    }

    switch (scene)
    {
    case 0: //Pilot Scene
    case 12://Discuss about Dan
    case 13://Discuss with Dan
    case 14://Dicuss with Dan and Chris
        DrawScene1BG();
        break;

    case 1: //Intro to Angella Scene
        DrawScene2BG();
        break;
    case 2: //Angella's Death Scene - Before
    case 4: //Angella's Death Scene - After Death
    case 6: //Andy's Question (This is where we get make our first choice)
    case 7: //Locking Dan
    case 8: //Finding evidence
    case 9://Susing everyone
    case 10: // Andy dialouge
    case 11:
    case 15:
    case 17:
    case 18:
    case 19: 
        DrawScene3BG();
        break;

    case 3: //Angella's Death Scene - Narration
    case 5: //FlashBack of Dan
    case 16:
    case 20:
        DrawScene4BG();
        break;
    }
}

#pragma endregion

#pragma region FrameFunctions
//Clears the frame so that we can redraw a new Frame
void ClearFrame()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glEnd();
    glFlush();
}

//Recovers the BG of the frame that we lost from the prev clear frame
void RecoverFrame()
{
    DrawCurrentSceneBG(currentScene);
}
#pragma endregion

//This is deals with the animation of the dialouges
#pragma region DialougeSystem
//checking if the length of the text goes beyond that of the dialouge box
//Takes 2 parameters , the actual dialouge and the text in which we will store either the part of the dialouge or the whole dialouge
bool CheckOverFlow(string& dialouge, string& text)
{
    if (dialouge.length() > 120)
    {
        text = dialouge.substr(0, 120); //create a substring of length 120 characters
        dialouge.erase(0, 120); //erases first 120 characters
        return true;
    }
    else  //The length of the dialouge was below 120 characters so no need to split
    {
        text = dialouge;
        return false;
    }
}

//Filling the queue with characters that we need for animating the dialouges
//Takes 1 parameter which is the text to fill the queue with
void FillQueue(string text)
{
    for (auto x : text)
    {
        Enqueue(queue, x);
    }
}

//Drawing text of the speaker name on the red hexagon
//Takes one parameter, which is the name of the speaker to render
void RenderSpeaker(string speaker)
{
    char speakerinChar[100] = "";

    for (int i = 0; i < speaker.length(); i++)
    {
        speakerinChar[i] = speaker[i];
    }

    DrawDialouge(speakerinChar, SpeakerXPos, SpeakerYPos, 0);
}

//Animating the text for the given dialouge
//Takes one paramter , the dialouge to animate
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

    //While the queue is not exahausted of the dialouge we dequeue an element draw it on screen , wait for few milliseconds and repeat the process while the condition is true
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

    if (OverflowFirstLine) //if the dialouge is larger then one line then we draw the text on the second line , the first line data is obtained from the above while loop
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

        if (OverflowSecondLine) //if the dialouge is larger then two lines then we draw the text on the third line , the first and second line data is obtained from the above while loops
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

//Deals with logic of branching the game flow with the option chosen
#pragma region BranchingLogic
//Drawing the UI for the options that can be selected
//we have hardcoded to have only 2 options
void RenderOptions()
{
    string fc = BranchADialouges[branchCounter][0].second;
    string sc = BranchBDialouges[branchCounter][0].second;
    char FirstChoice[200] = "";
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
    //the GL_BLEND is used to create a transparent effect
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0, 0, 0, 0.75);
    glBegin(GL_POLYGON);
    glVertex2f(400, 500);
    glVertex2f(400, 700);
    glVertex2f(1000, 700);
    glVertex2f(1000, 500);
    glEnd();
    DrawDialouge(FirstChoice, 420, 600, 1);

    glColor4f(0, 0, 0, 0.75);
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

//Here we check which all scenes have a branching at the end
bool CheckIfBranching()
{
    switch (currentScene)
    {
    case 6: //Andy's Question (This is where we get make our first choice)
    case 8: //Questioning Andy about justice to angela
    case 9: //john claim
    case 10: //Sus Andy
    case 17:
        return true;

    default:
        return false;
    }
}
#pragma endregion

//We make a file if the player has reached the secret ending at the very end
void MakeFile()
{
    fstream file;
    file.open("Thanks.txt", ios::out);

    //Content that we are gonna put into the file
    file << "Hey " + PlayerName + " !!!"
        << endl << "If you have this file generated , that means you sat through the entire game and the little message we dropped the end.... "
        << endl << "Thank you so much for playing the game , we worked so hard on , it actually means a lot!!! "
        << endl << "As promised in the post credits scene the game is gonna be REMADE to be much more better "
        << endl << "The game will be soon published on ITCH.io and Playstore and will be available on your phones too!!! "
        << endl << "If you wanna be updated when the game launches , let the developers of the game know, and we will send a email when the game is published! "
        << endl << "Once again thanks for playing the game and reading this message"
        << endl << "And of course please suppport the release of the upcoming game by sharing this current game with your friends!!"
        << endl << "See you there , in the REMAKE of murderMystery";
    file.close();
}

//MouseCallBacks responsible for the advancement of the game
#pragma region MouseCallback
//Advances the game story on click
void AnimateNextDialouge(int button, int state, int x, int y)
{
    //Here we transtion to the different state depending on the state that we are currently in
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) //When the left mouse button is clicked
    {


        switch (Scene)
        {
        case START:
            Scene = DESCRIPTION; //if we are in Start start transition to description start
            break;

        case DESCRIPTION:
            Scene = SCENE;  //if we are in description start transition to SCENE start
            TypingSpeed = 5;
            break;

        case SCENE:
            
            if (currentDialouge == SceneDialouges[currentScene].size() - 1) //we check if the current dialouge number we are viewing is equal to the size of the dialouges for that scene
            {
                if (CheckIfBranching()) //Check if branching exist for that scene
                {
                    //If yes go to CHOOSING state
                    Scene = CHOOSING;
                }
                else //if No go to the next scene
                {
                    ClearFrame();
                    currentScene++;
                    currentDialouge = 0;
                    if (currentScene == SceneDialouges.size())
                    {
                        Scene = THEEND; // we go to credits scene once all dialouges are exhausted
                        return;
                    }


                    NewScene = true;
                    cout << currentScene << endl;
                    cout << SceneDialouges.size() << endl;
                    //Scene = TOBECONTINUED;
                }

            }
            else //If we are not at the last dialouge for the scene , just increment the currentdialouge
            {
                currentDialouge++;
            }
            break;

        case SCENEA: //Branch A of the option
            if (currentDialouge == BranchADialouges[branchCounter].size() - 1) //if we are reached the end of dialouges , we reset the values and go back to the main scene
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

        case SCENEB: //Branch A of the option
            if (currentDialouge == BranchBDialouges[branchCounter].size() - 1) //if we are reached the end of dialouges , we reset the values and go back to the main scene
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
            if (x > 400 && x < 1000 && y > 40 && y < 220) { Scene = SCENEA; currentDialouge = 1; } //if the mouse coordinates lies within boundary of the first option , branch to Branch A
            if (x > 400 && x < 1000 && y > 320 && y < 510) { Scene = SCENEB; currentDialouge = 1; }//if the mouse coordinates lies within boundary of the second option , branch to Branch B
            break;

        case THEEND: //Shows the The end text animated
            Scene = CREDITS;
            break;

        case CREDITS: //Shows the credit scene
            Scene = POSTCREDITS;
            break;

        case POSTCREDITS: //Shows the post Credit scene
            if (currentDialouge == PostCreditDialouges[0].size() - 1)
            {
                MakeFile();
                exit(0);
            }
            else
            {
                currentDialouge++;
            }
            break;


        default:
            break;
        }
    }
}
#pragma endregion

//Draw Home Screen
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

#pragma region Credits
//Draw the End title
void DrawTheEND()
{
    string theend = "THE END....";
    // string saav = "Yenchina Saav Ya";

    char buffer1[200] = "";
    // char buffer4[200] = "";

    int i = 0;

    FillQueue(theend);

    while (!isEmpty(queue))
    {
        buffer1[i] = Dequeue(queue);
        DrawDescriptionText(buffer1, 600, 400);
        Sleep(100);
        i++;
        glEnd();
        glFlush();
    }

    for (int i = 0; i < theend.length(); i++)
    {
        buffer1[i] = theend[i];
    }


    glClear(GL_COLOR_BUFFER_BIT);
    DrawDescriptionText(buffer1, 600, 400);
    glEnd();
    glFlush();

}

//Show the credits without any animation
void DrawCredits()
{
    char CreditsHeading[100] = "CREDITS";
    char BinseWork[100] = "Story , Dialouges , Background Design";
    char Binse[100] = "Binse Varghese";

    char AshishWork[100] = "Game Development and Deployment";
    char Ashish[100] = "Ashish Kishore Kumar";

    glClear(GL_COLOR_BUFFER_BIT);
    DrawDescriptionText(CreditsHeading, 600, 550);
    DrawDescriptionText(BinseWork, 500, 450);
    DrawDescriptionText(Binse, 575, 400);
    DrawDescriptionText(AshishWork, 500, 250);
    DrawDescriptionText(Ashish, 550, 200);
    glEnd();
    glFlush();
}

#pragma endregion


//Loads the main screen
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

//initializes screen values
void init()
{
    glClearColor(0, 0, 0, 1);
    gluOrtho2D(0.0, 1280.0, 0.0, 720.0);
}

//Displays the requried content based on the state of the game
void display() //display function is called repeatedly by the main function so keep all rendering functions here
{
    //We draw Objects in the following order
    //1.Backgrounds
    //2.Objects
    //3.Characters
    //4.Dialougebox
    //5.Speaker name
    //6.Dialouge
    //7.Click to conitnue indicator or Branching Options

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
        RecoverFrame();
        DrawCharacter(SceneDialouges[currentScene][currentDialouge].first);
        DrawDialougeBox();
        RenderSpeaker(SceneDialouges[currentScene][currentDialouge].first);
        AnimateText(SceneDialouges[currentScene][currentDialouge].second);
        DrawClickToContinue();
        break;

    case CHOOSING:
        queue = createQueue(1000);//create a fresh queue
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
        DrawCharacter(BranchADialouges[branchCounter][currentDialouge].first);
        DrawDialougeBox();
        RenderSpeaker(BranchADialouges[branchCounter][currentDialouge].first);
        AnimateText(BranchADialouges[branchCounter][currentDialouge].second);
        DrawClickToContinue();
        break;

    case SCENEB:
        queue = createQueue(1000); //create a fresh queue
        RecoverFrame();
        DrawCharacter(BranchBDialouges[branchCounter][currentDialouge].first);
        DrawDialougeBox();
        RenderSpeaker(BranchBDialouges[branchCounter][currentDialouge].first);
        AnimateText(BranchBDialouges[branchCounter][currentDialouge].second);
        DrawClickToContinue();
        break;

    case THEEND:
        DrawTheEND();
        break;

    case CREDITS:
        DrawCredits();
        break;

    case POSTCREDITS:
        queue = createQueue(1000);//create a fresh queue
        DrawDialougeBox();
        RenderSpeaker(PostCreditDialouges[0][currentDialouge].first);
        AnimateText(PostCreditDialouges[0][currentDialouge].second);
        DrawClickToContinue();
        break;

    default:
        cout << "Something went wrong";
        break;

    }
    glEnd();
    glFlush();
}

//Initialize all the varialbles for the story
void InitializeVariables()
{
    Description = {
    "One day John, Peter, Dan, Angela, Chris, Emily and " + PlayerName + " , who were best friends since college, who work in ",
    "different MNCs got tired of their day to day life and wanted to go on a long vaccation. John suggested to go to the ",
    "countryside so that they can get rid of the city rush and all the friends agreed. They decided to book a hotel but later ",
    "Dan suggested that he knows a person who owns a mansion and is willing to rent it for a fair price, all of them agreed ",
    "and were very excited. They began their journey 3 days from then in John's car and reached there the next night.  ",
    };

    SceneDialouges[0] = { // Pilot 
    {"Narrator" , "As they entered , they were awestruck looking at the magnificent mansion and start exploring it and checked out the    rooms they were going to stay in and started unpacking their luggages. "},
    {"Chris" , "Hey " + PlayerName + " , Andy come check this room out..... it's even bigger than my living room!! "},
    {"You" , "Wow.... my whole family can stay here. "},
    { "Andy" , "Hahaha...C'mon it isn't that big. But I have been wondering how did Dan get this mansion for such a low price.         Something doesn't feel right. "},
    {"Chris" , "C'mon Andy... why are you like this, overthinking about everything, we are here to relax...  we don't get this         opportunity everyday. "},
    {"You" , "Yes Chris is right... we are here to release our stress so don't increase it by overthinking. "},
    {"Andy" ,  "Yeah I think you guys are right.... if only I was a little more carefree like you guys. We are in this beautiful        mansion in this peaceful place and we are going to make the best of it. Besides, what can go wrong? "},
    {"????" ,  "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAHHHH!!!!! (Sudden scream from the other room....) "},
    {"You" ,  "I think I just heard Angela screaming...... we must hurry up and check what happened!! "},
    {"Chris" , "You guys go ahead I'll call rest of them. "},
    {"Narrator" ,  "They rush towards the scream.... "}
    };

    SceneDialouges[1] = { //Intro to Angella
    {"Narrator" , "Scene 2 , They have reached the source of the scream. "},
   {"You" , "ANGELA!!! What happened why did you scream??? Are you alright?!! "},
   {"Angela" , "I just saw a huge cockroach over there.... aaaahhh so scary  :'-( "},
   {"You" , "Are you serious we got so  scared... Angela why you such a coward ? " },
   { "Andy" , "Angela... you are so silly. By the way where is Emily ? ?  (^o^) "},
   { "Emily" , "Im here, I ran and hid in the washroom after I saw the cockroach  :'-( "},
   {"Angela" , "Im so sorry you guys.. I just got shocked seeing it all of a sudden.. let's go to bed, we have to go out tomorrow. "},
   {"Narrator" , "They head to their respective room to rest up for tomorrow. "}
    };

    SceneDialouges[2] = { //Angella's Death Scene - Before
   {"You" , "Good Morning everyone...what a lovely morning. "},
   {"Andy" , "Yeah weather is really good outside..I hope nothing goes wrong. "},
   {"Dan" , "Ohh wow Andy... go ahead and jinx the day... today is a good day don't decimate it for us. "},
   { "Emily" , "Stop Dan why are you always after his life... don't mind him Andy he doesn’t mean any harm. "},
   { "John" , "Don't worry you guys everything is planned out nothing will go wrong. "},
   { "You" , "I knew we can trust you John.... so Andy there is nothing to worry about, if John has planned then you know its going  to be perfect... By the way where is Angela?? "},
   {"Emily" , "I tried waking her up in the morning but she didn't respond...she was very tired yesterday so I thought I will let her sleep a little more longer and after that I got ready and came here. "},
   {"Andy" , "Ohh is that so... I will just go and wake her up. "},
   {"Dan" , "AAGHHH...this means we will be late..I hope that girl won’t take time. "},
   {"Andy" , "AAAANNNNGEEELLLAAAAAAAAAAAAA!!!!!!!!!!!!! "},
    };

    SceneDialouges[3] = { //Angella's Death Scene - Narration
  {"Narrator" , "Everyone rushes into Angela's room...... "},
  {"Narrator" , "And they tear up as they discover that Angela is no more......... "},
  {"Narrator" , "Everyone is in utter disbelief and don't understand what to do................... "},
  { "Narrator" , "John being quick on his feet calls for an ambulance, but they get to know that they won't be able to come on time as   the roads were blocked due to landslide. "},
  { "Narrator" , "John informs the cops as well but the nearest police station is in the city and since the roads are blocked, they won't be able to come that quickly. "},
  { "Narrator" , "A few hours later.... "},
    };

    SceneDialouges[4] = { //Angella's Death Scene - After
  {"Andy" , "OMG, poor Angela, how can this happen.... "},
  {"Dan" , "Yeah it is really shocking but we need to calm down and collect our thoughts. "},
  {"Andy" , "Dan you don't have a heart... it looks like you don't even care that Angela died. "},
  { "Emily" , "Andy is right you don't seem to be affected by Angela's death at all.. "},
   {"Dan" , "What are you both implying??? "},
  { "Andy" , "What I'm trying to say is that MAYBE YOU ARE THE ONE WHO KILLED ANGELA ....we all know you never liked her. "},
  { "Dan" , " WHAT THE HELL???... Are you being serious right now.... how dare you accuse me of such heinous crime. "},
  { "Andy" , "We all know what kind of a person you are... I know you killed her. " + PlayerName + " why don't you speak up. "},
    };

    SceneDialouges[5] = { //FlashBack to Dan
    {"You" , "Did...Did Dan really do this????? "},
    {"You" , " I know him very well no matter how tough he acts in front of us he has a good heart. "},
    {"You" , "I can never forget the time when my parents died in a car accident 5 years ago... "},
    { "You" , "Only Dan was there to support me.... "},
    { "You" , "He was there for me whenever I needed him, he helped me overcome my depression... "},
    };

    SceneDialouges[6] = { //Andy's Question
        {"Andy" ,  PlayerName + " Stop dreaming and speak up... I asked what do you think of this ? ? "},
    };

    SceneDialouges[7] = { //Locking Dan
        {"Andy" ,  "I don't think its safe to be around him right now. We should lock him in some room. "},
        {"Emily" ,  "Yeah Andy is right we should lock this psycho. "},
        {"Dan" ,  "You are just blabbering about what could be the reason to kill her.... but you are not bringing any proof on the table. "},
        {"You" ,  "You are right Dan, but right now we have to lock you in otherwise things can get heated up and worsen everything. "},
        {"John" ,  "Yes Dan we have to calm things down right now, I don't believe that any of us killed her. It can be someone from        outside. "},
        {"Dan" ,  "You are right John, but the fact that Andy straight up blamed me for her death without any proof seems very odd to me.. I need you to find who did it... I don't want the stigma of a killer on me. "},
        {"You" ,  "Don't worry Dan we will do something but for now you stay in this room. "},
        {"Dan" ,  "I need to tell you something very important... "},
    };

    SceneDialouges[8] = {
        {"Narrator" ,  "30 minutes after locking Dan in his room. "},
        {"Chris" ,  "I can't believe that Dan would go that far to take out his anger.  "},
        {"Andy" ,  "It should not surprise you after all its Dan. "},
        {"Andy" ,  "Emily didn't you see or hear anything last night when Dan was there. "},
        {"Emily" ,  "No Andy I didn't see or hear anything... I wish I was awake... then she would have been with us. "},
        {"Narrator", "You and John Enter the hall. "},
        {"John", "We have locked him in the room. So now let's think this through. "},
        {"You", "(If what Dan said is right then...) "},
        {"Andy", "I hope the cops come fast so that they can take that killer away and bring justice to Angela. "},
    };

    SceneDialouges[9] = { //Finding Evidence
       {"You" ,  "Hmm maybe we should go through his bags... "},
       {"Emily" ,  "Yeah exactly... who knows how long the cops will take to reach here.  "},
       {"John" ,  "Let me bring them real quick.  "},
       {"Narrator" ,  "John brings everyones bags to the living room. "},
       {"Andy" ,  "Let's start with this bag I think he hid the poison in this one. "},
       {"Andy" ,  "Its not here... let's check the other bag it must be.... Ohh look I found it, it was here in this bag. This proves that   he is the killer. "},
       {"Emily" ,  "Good job Andy... see we told you that he has to be the killer.  "},
       {"John" ,  "Yeah you were right about that..... and you were right about the fact that she died because of poisoning.  "},
       {"John" ,  "But as far as I remember you are no doctor so how did you know she died because of poison.  "},
       {"Andy" ,  "What do you mean... I don't understand what you are trying to say...  "},
    };

    SceneDialouges[10] = { //Throwing Blame
        {"John" ,  "And Andy , you could also have been the one to kill her I mean didn't you two have an history of your own. "},
        {"Emily" ,  PlayerName + " you better watch your mouth! Don't blame me, I didn't do anything, I can never kill her!  "},
        {"Emily" ,  "But Andy I do remember you two used to fight a lot.  "},
        {"Andy" ,  "C'mon guys are you  serious?? "},
        {"Andy" ,  "That was a long time ago! "},
        {"Andy" ,  "And you all know I can't even kill a bug... how will I kill her and "+ PlayerName+ " , I just assumed thats how he killed her. "},
        {"Chris" ,  "I just remembered something.... "},
        {"Chris" ,  "Few minutes back you asked Emily if she heard anything last night... how did you know she died yesterday night... she   could have died in the morning.  "},
        {"Andy" ,  "I just assumed that he must have poisoned her yesterday night. Guys you are just overthinking.  "},
    };

    SceneDialouges[11] = { //Andy's Dialouge
       {"Andy" ,  "Yeah because once they come we can tell that he is the killer and we found the poison in his bag, it will be very easy for them now. "},
    };

    SceneDialouges[12] = { //Discussing about dan
       {"Narrator" ,  "Few minutes after the arguement , they went to meet talk to Dan, but Dan is using the washroom. "},
       {"John" ,  PlayerName +" we have no evidence to prove Dan innocent, do you think Andy is right and Dan is the one who killed him. "},
       {"You" , "I can't say anything John, my heart doesn't believe that he killed her but the evidence makes it very difficult for me to believe Dan. "},
       {"John" , "Maybe we should talk to Dan about this again and if he is the killer then lets ask him to surrender. "},
       {"John" , "Ok lets talk to him. First let him come out of the washroom. "},
    };

    SceneDialouges[13] = { //Discussing with Dan
       {"Narrator" ,  "Dan comes out of the Washroom. "},
       {"You" ,  "Dan we found a bottle of poison in your bag. "},
       {"Dan" , "What are you saying, its impossible!!! "},
       {"Dan" , "Trust me guys I didn't do it. Andy is plotting this against me. He took his revenge from her and is blaming everything  on me so that he can escape. "},
       {"John" , "We can't do anything Dan. Please if you have done just confess. "},
       {"Dan" , "HAVE YOU LOST YOUR MIND!! "},
       {"Dan" , "I can never do such appalling thing. You guys have to do something and get me out of this situation. "},
       {"You" , "But do you have any proof that he is lying or that he is the one who killed her??? You have only given us the motive,   not the evidence. "},
    };

    SceneDialouges[14] = { //Discussing with Dan with chris
      {"Narrator" ,  "Chris overhears the conversation so far.. "},
      {"Chris" ,  "Maybe I can help. "},
      {"John" , "Chris! What are you doing here? "},
      {"Chris" , "I just came to see Dan because even I don't believe that he killed her. "},
      {"Chris" , "I didn't speak up before because I was not able to think straight after seeing Angela like that. "},
      {"Dan" , "Thanks for trusting me Chris. "},
      {"Chris" , "I have a plan to save Dan, I think it will work. "},
      {"John" , "What's the plan Chris?? "},
      {"Chris" , "Yeah, I don't know if you guys noticed this or not but the moment we left that drug store Andy was acting really weird. "},
      {"You" , "What do you mean?? "},
      {"Chris" , "He was very cautious about his medicines... "},
      {"Chris" , "When I asked him about what all medicines he bought he got flustered and didn't tell me properly... I even noticed him  throwing away a piece of paper, at that moment I didn't think much of it but now I believe he had bought something and  he didn't want any of us to know. "},
      {"John" , "But that doesn't mean that we can prove that he killed Angela... "},
      {"Chris" , "I know John, but it is very suspicious, it could be the reciept for those medicines which he bought, why would he throw it away. "},
      {"Chris" , "I know it doesn't prove anything but we can try using it as our leverage. "},
      {"You" , "And how will we do it? "},
      {"Chris" , "I have an idea, first we will just go over there and talk to everyone normally and later on we will bring out the topic of the reciept from the drug store which obviously we don't have but we have to act as if we have it and we just have   to lure him to talk about it. "},
      {"Chris" , "It can work, what do you think "+PlayerName+" ? "},
      {"You" , "Yeah we can give it a shot. "},
    };

    SceneDialouges[15] = { //Prep for accusation
       {"Andy" ,  "So did he confess?? "},
       {"John" ,  "Yes Dan has agreed to surrender, he said he has no evidence to prove his innocence.  "},
       {"Andy" ,  "Of course he doesn't have any evidence. Finally Angela will get justice and Dan will get what he deserves.  "},
       {"John" ,  "Yeah you are right Andy, Angela will get justice and her KILLER will get what he DESERVES. "},
       {"Chris" ,  "Guys I don't understand one thing, that why would he kill   her now I mean when we all are there. He could have killed her before or some other time. "},
       {"Chris" ,  "Why take such a risk??? "},
       {"Andy" ,  "He is probably not that smart as he shows. "},
    };

    SceneDialouges[16] = { //Monolouge
       {"You" ,  "I don't know what to do now.... "},
       {"You" ,  "If we don't catch him early we might loose Dan....  "},
       {"You" ,  "But we don't know what all excuses he has planned and will our bluff even work?  "},
       {"You" ,  "Should we just wait for the cops???? "},
       {"You" ,  "But if we don't come up with any evidence Dan's life will be destroyed... "},
       {"You" ,  "I have to think this through. "},
    };

    SceneDialouges[17] = { //Monolouge
      {"You" ,  "I think the best thing to do is to ...."},
    };

    SceneDialouges[18] = { //Confession
     {"Andy" ,  "I think there is no point in hiding it now..... "},
     {"You" ,  "Yeah Andy I think its high time that you tell us why did you do that. "},
     {"Andy" ,  "I can't believe I got caught like this... thought I was smarter than that hahaha.. "},
     {"Chris" ,  "Its not funny Andy. "},
     {"John" ,  "WHY...why would you do that, wasn't she your friend! "},
     {"Emily" ,  "What are you saying Andy....I don't understand. "},
     {"Andy" ,  "SHUT UP JUST SHUT UP!! "},
     {"Andy" ,  "I worked so hard, I was so close and she had to steal it away from me.  "},
     {"John" ,  "What did she take away from you that made you take her life?  "},
     {"Andy" ,  "I'll tell you what she took away from me.  "},
     {"Andy" ,  "You all know how hard it was for me to get into “SANTA MONICA” . I started from the bottom of the coorporate ladder and worked my way up to the senior developer post ...  "},
     {"Andy" ,  "And that's when Angela joined our company, she was my junior and she was in my team.  "},
     {"Andy" ,  "We both were assigned a project and we worked day and night to impress our bosses which could have given me my dream    post which is the manager of “SANTA MONICA” .  "},
     {"Andy" ,  "Everything was going perfectly but towards the end when we were  supposed to demonstrate our project, on that very      morning my younger brother got into an accident and I had to rush him to the hospital and left everything to Angela... "},
     {"Andy" ,  "BUT THAT WOMAN BETRAYED ME!!! "},
     {"Emily" ,  "Andy I think you have got the wrong idea.. "},
     {"Andy" ,  "I'M NOT DONE TALKING!!!! "},
     {"Andy" ,  "The next morning when I went to the office....  I heard we have a new manager and to my surprise it was Angela.... "},
     {"Andy" ,  "That made me furious, I just walked out... "},
     {"Andy" ,  "I couldn't think about anything else except for the betrayal that I just faced not only from my colleague but from a    friend.... a close friend. "},
     {"Andy" ,  "HOW DARE SHE BETRAY ME I HELPED HER SO MUCH!! "},
     {"Andy" ,  "She didn't even know anything properly... I taught her from scratch!! "},
     {"Andy" ,  "Even for the project I did the most work and in the end she took away all the credits! "},
     {"Andy" ,  "And ever since that I didn't talk to her and just left the company... but the anger still remained and I had to get rid of that somehow...... "},
     {"Andy" ,  "And that's when I decided that I'll get rid of her, and that's when you all asked to meet up. "},
     {"Andy" ,  "That's when I planned how to get rid of her, I decided to use poison to kill her and put the blame on Dan as I knew      already that they both had a huge fight because Dan never liked Angela's personality.  "},
     {"Andy" ,  "Once we started our journey I asked you to stop at the nearest drug store to get my medications and that's where I        bought pure ethanol as I knew it will be the perfect poison.  "},
     {"Andy" ,  "At night when everyone had slept I decided go to her room and kill her so I just put some ethanol on my handkerchief,    then I just simply put it near her face so that she breathes it in and slowly goes into deep sleep.  "},
     {"Andy" ,  "And I hid the bottle in Dan's bag to focus everything on him.  "},
     {"Emily" ,  "Andy.... you are wrong, she didn't mean to steal your job..  "},
     {"Emily" ,  "When she got to know that she is getting promoted she declined it and said that you deserve it, you have all the skills but the manager insisted her to take the job. "},
     {"Andy" ,  "NOOO YOU ARE LYING!!! She wanted the post for herself. "},
     {"Emily" ,  "She tried to contact you, but you just vanished and never picked her call. "},
     {"You" ,  "Andy if you had not jumped to conclusion and if you had talked it out, she would have been with us. "},
     {"Andy" ,  "THAT'S NOT TRUE..... "},
     {"Andy" ,  "If she was actually a true friend she would have discussed it with me because she knew how badly I wanted that post. "},
     {"Emily" ,  "But the boss rejected her suggestion he told her that we might have to cut off few people to save money for the company that's when she thought that if she becomes the manager she could save you, and that's exactly what she did Andy. "},
     {"Emily" ,  "She never wanted to steal your job..... she just wanted to save you. "},
     {"Andy" ,  "NO.. THIS CAN'T BE... NOOOOOO... ANGELAAA... I'M SO SORRY. "},
     {"Andy" ,  "I got so blinded by my anger I couldn't control myself and I decided to kill her the day this happened... if only I had given her a chance to explain herself, this would have not happened. "},
     {"Chris" ,  PlayerName + ", the cops are here. "},
    };

    SceneDialouges[19] = { //Aftermath
      {"Narrator" ,  "They go to bring out Dan. And cops hand cufffed Andy. "},
      {"Andy" ,  "I want to apologize to Dan before I leave. "},
      {"Dan" ,  "I can't forgive you Andy.. "},
      {"Dan" ,  "That you have done is unforgivable, not the fact you were blaming me for this horrendus crime but for killing our        friend, someone's daughter, someone's sister and for abandoning your family, you destroyed two families Andy you can't be forgiven. "},
    };

    SceneDialouges[20] = { //Aftermath - moral
      {"Narrator" ,  "After the cops took Andy away they all went back home with a heavy heart... "},
      {"You" ,  "Can't believe something like this happened... "},
      {"You" ,  "Just because the two people didn't communicate, didn't share what they felt?! "},
      {"You" ,  "If only people were open about their feelings.....  "},
      {"You" ,  "Things like this could have been avoided.... "},
    };



    //BRANCH A DIALOUGES
    BranchADialouges[0] = { //Suporting Dan
        {"You" , "I think we should not hurry up to the conclusions... "},
        {"You" , "I think we should not hurry up to the conclusions and give Dan some chance to explain. "},
        {"John" , "I think " + PlayerName + " is right we should not point  fingers at each other without any solid proof. "},
        {"Dan" , "Thanks guys at least some people have brains unlike others. "}
    };

    BranchADialouges[1] = { //Doubting Andy
      {"You" , "Why are you in such a hurry?? "},
      {"You" , "Why are you in such a hurry?? "},
      {"Andy" , "What do you mean? Im not in any hurry I just want justice for angela... and for that reason I think we should go and    look for some evidence in his bags. "},
    };

    BranchADialouges[2] = { //Getting john claim right
      {"You" , "He is saying you are blaming dan for no reason. "},
      {"You" , "What he is trying to say is... you just started blaming Dan without any thought behind it. "},
      {"You" , "You never even considered the fact that someone from outside could have come to kill her or Emily who is her room -     mate could have done it. "},
      {"You" , "And the fact that you were so sure that she died because of the poison is very suspicious. "},
    };

    BranchADialouges[3] = { //Against Andy
     {"You" , "But you are being very precise with everything.... "},
     {"You" , "But you are being very precise with everything.....  its almost like you planned the whole thing. "},
     {"Andy" , PlayerName + " thats nothing but rubbish, you are saying this just to save that killer. "},
     {"John" , "ITS NOT YOUR JOB TO DECIDE WETHER HE IS A KILLER!!!!. "},
     {"John" , "We have called cops and they will decide what will happen. "},
    };

    BranchADialouges[4] = { //Climax - Reciept
    {"You" , "Try catching Andy now. "},
    {"John" , "By the way Andy while we were there talking to Dan "+ PlayerName + " found something. "},
    {"You" , "Yeah, its a reciept from Everyday Drugs where we stopped yesterday while we were coming to this mansion as you said you wanted to buy some of your meds. "},
    {"Andy" , "WHAT!!!!! HOW DID YOU GET THAT!!! "},
    {"You" , "What do you mean, it was right next to your bag. "},
    {"Andy" , "That's not possible, its not mine, its probably Dan's he must have tried to frame me. "},
    {"Chris" , "But this receipt is in your name, Andy. "},
    {"Andy" , "BUT THAT'S NOT POSSIBLE!!!! "},
    {"John" , "Why not, you went to that medical store and bought medicines for which they gave you the reciept, why are you getting so worked up about it. "},
    {"Andy" , "BECAUSE I THREW IT AWAY AS SOON AS I GOT IT SO THAT NO ONE KNOWS I BOUGHT PURE ETHANOL. "},
    {"You" , "Why do you need pure ethanol??? "},
    };





    //BRANCH B DIALOUGES
    BranchBDialouges[0] = { //Against Dan
       {"You" , "Yes Dan you have been hating on her from so long... "},
       {"You" , "Yes Dan you have been hating on her from so long... and we all know how you act when you get angry. "},
       {"John" , PlayerName + " is right, Dan you have had a lot of history with her and you do have a reason to kill her. "},
       {"Dan" , "Those things were in the past... we haven't fought in years. Everything was fine between us. "}
    };

    BranchBDialouges[1] = { //Suporting Andy
       {"You" , "Yeah the cops must be here within 2 hours... "},
       {"You" , "Yeah the cops must be here within 2 hours... Hope they find enough evidence. "},
       {"Andy" , "Yeah since you mentioned it... I'm pretty sure that if we go through his things we can find some evidence. "},
    };

    BranchBDialouges[2] = { //Getting john claim wrong
      {"You" , "He is saying you are not that smart. "},
      {"You" , "What he is trying to say is that you are not that smart to figure out what could have killed her and what made you        think that it was poison that killed her. "},
      {"John" , "Yeah "+ PlayerName + " you are kind of right...  "},
      {"John" , "But what I'm exactly trying to say is, you never even considered the fact that someone from outside could have come to   kill her. And the fact that you were so sure that she died because of the poison is very suspicious.  "},
      {"You" , "Yeah you are right, even Emily could have done it as she was in the same room with her all night.  "},
    };

    BranchBDialouges[3] = { //Supportign Andy
    {"You" , "Hmm maybe you are right... "},
    {"You" , "Hmm maybe you are right... "},
    {"Emily" , "Yeah its probably because of the situation that he thought that Dan killed her... but he is not wrong, I mean the proof is right in front of us.   "},
    {"Chris" , "Yeah I guess all of us must have thought that this happened yesterday night, doing it in the morning when we were awake would have been difficult. And maybe we should just wait for the cops.  "},
    };

    BranchBDialouges[4] = { //Climax - Kerchief
    {"You" , "Try catching Andy after lunch. "},
    {"John" , "Sorry Andy we didn't trust in you. "},
    {"Andy" , "Its ok John, its not your fault you were just trying to save this friendship of ours, you didn't know that Dan will    turn out like that. "},
    {"You" , "Guys I know that none of us wants to eat right now but we have to because we need to make sure that none of us fall     ill.  "},
    {"Emily" , "Yeah you are right we should not be starving as it won't help us and only make things worse. "},
    {"Emily" , "I'll cook something for all of us, John please help me with the cooking.  "},
    {"Chris" , "Should we give some to Dan I mean we can't let him starve as we don't know when the cops will come.  "},
    {"Andy" , "Why do you want a killer to eat with us?  "},
    {"Emily" , "Its not about that its just if something happens to him before the cops come he won't be punished for his doings.  "},
    {"Andy" , "Hmm maybe you are right...let that killer eat.  "},
    {"Narrator" , "Few minutes after having food...  "},
    {"Chris" , "Hey Andy, can you please pass me your handkerchief I need dry off the water.  "},
    {"Andy" , "No I'm sorry I can't give you mine as it is dirty.  "},
    {"John" , "How is it dirty? I never saw you use it.  "},
    {"Emily" , "Now that you mentioned it, I remember you didn't lend me your handkerchief when I was mourning.  "},
    {"You" , "Is it actually dirty or do you have a reason not to lend it.  "},
    {"Andy" , "YEAH I JUST TOLD YOU TWICE THAT ITS DIRTY!  "},
    {"Chris" , "So what, I just want something to dry my hands... now stop being so stubborn and pass me your handkerchief.  "},
    {"Andy" , "IF YOU ARE THAT EAGER TO DIE JUST TAKE IT.  "},
    {"You" , "What do you mean by that ANDY?  "},
    {"Andy" , "I can't let my friends die. "},
    {"John" , "Wasn't Angela your friend ANDY?? "},
    };








    PostCreditDialouges[0] = { //Post Credit scene
        {"Game" , "WAAAI-WWAAIITTTTTT!!!!!"},
        {"Game" , "Don't turn off the game yettt..."},
        {"Game" , "Damn that was close... if your are reading this , that means you are willing to listen what I have to say "},
        {"Game" , "I have a small message that the developers left for those who played the game till the very end... "},
        {"Game" , "The message goes as such....."},
        {"Game" , "Hey!! " + PlayerName + " if your are reading this , you must have sat through the entire game , and let's be honest.... without any      animation or music or some post processing it must have been a littttle bit dull and boring... "},
        {"Game" , "I understand , when we were making the game , we felt the same..... "},
        {"Game" , "The game doesn't really sell without the music and all. "},
        {"Game" , "But there wasn't much we could do , this being a mini project for our computer graphics subject , we have been limited to very primitive tools... "},
        {"Game" , "GLUT being a very old API and upon that for our story we couldnt really go all out , cause you know , we have to demo   this to our examiners during exam , cant have them swears and dark scenes in our game.... "},
        {"Game" , "So being kinda unsatisfied with the limitations imposed on us................. "},
        {"Game" , "We have decided to REMAKE this game in a game engine , UNITY , and go loose on the story making it R rated!!! "},
        {"Game" , "We gonna add all the good stuff , like music , animation , post processsing , sound effects and all that!! "},
        {"Game" , "So be ready, for the REMAKE of Murder Mystery , coming soon this  MOONSOON!! "},
        {"Game" , "I know its monsoon already , but its coming in monsoon lol. "},
        {"Game" , "See you then!! "},
        {"Game" , "......ok That was the message the developer had to say. "},
        {"Game" , "I have done my job , Click once more and the game shall close on its own. Dont click on that top left X unless you       wanna miss something nice.. "},
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