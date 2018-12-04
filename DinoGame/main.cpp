#include <FEHLCD.h>
#include <LCDColors.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <stdlib.h>

#define JUMPHEIGHT 30
#define GAMESPEED 20
#define DINOCOLOR 8355711
#define x1 40   // Horizontal position of dino
#define y2 132  // Vertical position of big cacti
#define y3 145  // Vertical position of small cacti
#define y4 70   // Vertical position of pterodactyl
#define OBSTRATE 30 // Rate at which obstacles appear

int MainMenu();
int PlayGame();
void StatsDisp();
void CreditsDisp();
void InstructionsDisp();
void ClearLCD(int frame, int y1, int crouch, int x2, int x3, int obsType1, int obsType2);
void DrawFrame(int frame, int y1, int crouch, int x2, int x3, int obsType1, int obsType2);
void DrawDinoR(int y, int color);
void DrawDinoL(int y, int color);
void DrawDinoJ(int y, int color);
void DrawDinoC(int y, int color);
void DrawBigObs(int x, int color);
void Draw2BigObs(int x, int color);
void DrawSmallObs(int x, int color);
void Draw2SmallObs(int x, int color);
void DrawBigSmallObs(int x, int color);
void DrawPterodactyl(int x, int color);

int main(void){
    int choice, replay = 1;

    do{
        choice = MainMenu();

        switch(choice){
            case 1: while(replay == 1){
                        replay = PlayGame();    // Returns a 1 if the player chooses to play again, a 0 if main menu is pressed
                    }
                    break;
            case 2: StatsDisp();
                    break;
            case 3: InstructionsDisp();
                    break;
            case 4: CreditsDisp();
                    break;
        }
    }while(true);
}

int MainMenu(){
    float x, y;

    LCD.SetBackgroundColor(WHITE);
    LCD.Clear();
    LCD.SetFontColor(BLACK);

    // Game title
    LCD.WriteAt("DINO RUNNER", 100, 10);

    // Black border rectangles
    LCD.FillRectangle(52, 66, 105, 55); // Top right
    LCD.FillRectangle(162, 66, 105, 55);    // Top left
    LCD.FillRectangle(52, 126, 105, 55); // Bottom right
    LCD.FillRectangle(162, 126, 105, 55); // Bottom left

    LCD.SetFontColor(WHITE);

    // White filler rectangles
    LCD.FillRectangle(57, 71, 95, 45);  // Top right
    LCD.FillRectangle(167, 71, 95, 45);  // Top left
    LCD.FillRectangle(57, 131, 95, 45);  // Bottom right
    LCD.FillRectangle(167, 131, 95, 45);  // Bottom left

    LCD.SetFontColor(BLACK);

    // Text
    LCD.WriteAt("Play", 62, 76);    // Top right
    LCD.WriteAt("Stats", 172, 76);   // Top left
    LCD.WriteAt("Instr.", 62, 136);    // Bottom right
    LCD.WriteAt("Credits", 172, 136); // Bottom left

    // Wait for touch screen press
    while(LCD.Touch(&x, &y));

    // Wait for touch screen release
    while(!LCD.Touch(&x, &y));

    // Change colors to show button has been pressed
    if(x > 57 && x < 152 && y > 71 && y < 116){
        LCD.SetFontColor(BLUE);
        LCD.FillRectangle(52, 66, 105, 55); // Top right
        LCD.SetFontColor(WHITE);
        LCD.FillRectangle(57, 71, 95, 45);  // Top right
        LCD.SetFontColor(BLUE);
        LCD.WriteAt("Play", 62, 76);    // Top right
        Sleep(500);

        return(1);
    }
    else if(x > 167 && x < 262 && y > 71 && y <116){
        LCD.SetFontColor(BLUE);
        LCD.FillRectangle(162, 66, 105, 55);    // Top left
        LCD.SetFontColor(WHITE);
        LCD.FillRectangle(167, 71, 95, 45);  // Top left
        LCD.SetFontColor(BLUE);
        LCD.WriteAt("Stats", 172, 76);   // Top left
        Sleep(500);

        return(2);
    }
    else if(x > 57 && x < 152 && y > 131 && y < 176){
        LCD.SetFontColor(BLUE);
        LCD.FillRectangle(52, 126, 105, 55); // Bottom right
        LCD.SetFontColor(WHITE);
        LCD.FillRectangle(57, 131, 95, 45);  // Bottom right
        LCD.SetFontColor(BLUE);
        LCD.WriteAt("Instructions", 62, 136);    // Bottom right
        Sleep(500);

        return(3);
    }
    else if(x > 167 && x < 262 && y > 131 && y < 176){
        LCD.SetFontColor(BLUE);
        LCD.FillRectangle(162, 126, 105, 55); // Bottom left
        LCD.SetFontColor(WHITE);
        LCD.FillRectangle(167, 131, 95, 45);  // Bottom left
        LCD.SetFontColor(BLUE);
        LCD.WriteAt("Credits", 172, 136); // Bottom left
        Sleep(500);

        return(4);
    }
}

int PlayGame(){
    float x, y, framerate;
    bool gameOver = false, touch = false, jump = true;
    int y1 = 137, crouch = 0, x2 = 300, x3;
    int frame = 1;
    int vel = 0, grav = 4, x2vel = 0, x3vel = 0, obsType1, obsType2;
    int rand_num, x2type, x3type;
    srand(TimeNow()*1000);  // Random number seed


    // Start countdown
    LCD.SetBackgroundColor(WHITE);
    LCD.Clear();
    LCD.SetFontColor(DINOCOLOR);
    LCD.WriteAt("3", 155, 115);
    Sleep(1000);
    LCD.Clear();
    LCD.WriteAt("2", 155, 115);
    Sleep(1000);
    LCD.Clear();
    LCD.WriteAt("1", 155, 115);
    Sleep(1000);
    LCD.Clear();
    LCD.WriteAt("Go!", 152, 115);
    Sleep(500);
    LCD.Clear();

    while(!gameOver){
        ClearLCD(frame - 1, y1, crouch, x2, x3, obsType1, obsType2);

        framerate = frame/TimeNow();
        crouch = 0;
        touch = LCD.Touch(&x, &y);
        LCD.SetFontColor(BLACK);
        LCD.WriteAt(touch, 0, 25);
        LCD.WriteAt(framerate, 220, 215); // Framerate for debug
        LCD.WriteAt("Highscore: ", 140, 0); // Add this in
        LCD.WriteAt("Score: ", 140, 25);
        LCD.WriteAt(frame, 250, 25);    // Score is kept as number frames survived

        if(touch && y1 > 75 && y < 120 && jump){
            vel = 25;
        }
        else if(touch && y >= 120){
            if(y1 < 137){
                vel = vel - 7;
                crouch = 1;
            }else{
                crouch = 1;
            }
        }

        if(y1 > 75){
            jump = false;
        }

        y1 = y1 - vel;
        if(y1 > 137){
            y1 = 137;
            vel = 0;
            jump = true;
        }

        vel = vel - grav;

        x2 = x2 - x2vel;    // Moves obstacle 1 to the left
        x3 = x3 - x3vel;    // Moves obstacle 2 to the left

        if(x2 <= 0){
            x2 = 300;
            x2vel = 0;
            obsType1 = 0;
        }
        if(x3 <= 0){
            x3 = 300;
            x3vel = 0;
            obsType2 = 0;
        }

        if(frame % OBSTRATE == 0){
            rand_num = rand() % 40;
            if(rand_num >= 0 && rand_num <= 9){ // One big cactus
                if(x2 == 300){
                    obsType1 = 1;
                    x2type = 1;
                    x2vel = 10;
                }
                else if(x3 == 300){
                    obsType2 = 1;
                    x3type = 1;
                    x3vel = 10;
                }
            }
            else if(rand_num > 9 && rand_num <= 17){    // Big and small
                if(x2 == 300){
                    obsType1 = 2;
                    x2type = 2;
                    x2vel = 10;
                }
                else if(x3 == 300){
                    obsType2 = 2;
                    x3type = 2;
                    x3vel = 10;
                }
            }
            else if(rand_num > 17 && rand_num <= 23){   // One small
                if(x2 == 300){
                    obsType1 = 3;
                    x2type = 3;
                    x2vel = 10;
                }
                else if(x3 == 300){
                    obsType2 = 3;
                    x3type = 3;
                    x3vel = 10;
                }
            }
            else if(rand_num > 23 && rand_num <= 25){   // Two bigs
                if(x2 == 300){
                    obsType1 = 4;
                    x2type = 4;
                    x2vel = 10;
                }
                else if(x3 == 300){
                    obsType2 = 4;
                    x3type = 4;
                    x3vel = 10;
                }
            }
            else if(rand_num > 25 && rand_num <= 27){   // Two smalls
                if(x2 == 300){
                    obsType1 = 5;
                    x2type = 5;
                    x2vel = 10;
                }
                else if(x3 == 300){
                    obsType2 = 5;
                    x3type = 5;
                    x3vel = 10;
                }
            }
            else if(rand_num > 27 && rand_num <= 29){   // Pterodactyl
                if(x2 == 300){
                    obsType1 = 6;
                    x2type = 6;
                    x2vel = 10;
                }
                else if(x3 == 300){
                    obsType2 = 6;
                    x3type = 6;
                    x3vel = 10;
                }
            }
            else{   // Nothing appears
                if(x2 == 300){
                    obsType1 = 0;
                    x2type = 0;
                    x2vel = 0;
                }
                else if(x3 == 300){
                    obsType2 = 0;
                    x3type = 0;
                    x3vel = 0;
                }
            }
        }

        DrawFrame(frame, y1, crouch, x2, x3, obsType1, obsType2);

        if(x1 + 40 >= x2 || x1 + 40 >= x3){
            if(obsType1 == 1){  // One big cactus hitbox
                if(x1 < x2 + 25){
                    if(y1 + 43 > 132){
                        break;
                    }
                }
            }
            if(obsType1 == 2){  // Big and small cactus hitbox
                if(x1 < x2 + 40){
                    if(y1 + 43 > 132){
                        break;
                    }
                }
            }
            if(obsType1 == 3){  // One small cactus hitbox
                if(x1 < x2 + 15){
                    if(y1 + 43 > 145){
                        break;
                    }
                }
            }
            if(obsType1 == 4){  // Two big cacti hitbox
                if(x1 < x2 + 40){
                    if(y1 + 43 > 132){
                        break;
                    }
                }
            }
            if(obsType1 == 5){  // Two small cacti hitbox
                if(x1 < x2 + 30){
                    if(y1 + 43 > 145){
                        break;
                    }
                }
            }
            if(obsType1 == 6){  // Pterodactyl hitbox
                if(x1 < x2 + 45){
                    if(y1 + 10 > y4 + 20 && !crouch){
                        break;
                    }
                }
            }
            if(obsType2 == 1){  // One big cactus hitbox
                if(x1 < x3 + 25){
                    if(y1 + 43 > 132){
                        break;
                    }
                }
            }
            if(obsType2 == 2){  // Big and small cactus hitbox
                if(x1 < x3 + 40){
                    if(y1 + 43 > 132){
                        break;
                    }
                }
            }
            if(obsType2 == 3){  // One small cactus hitbox
                if(x1 < x3 + 15){
                    if(y1 + 43 > 145){
                        break;
                    }
                }
            }
            if(obsType2 == 4){  // Two big cacti hitbox
                if(x1 < x3 + 40){
                    if(y1 + 43 > 132){
                        break;
                    }
                }
            }
            if(obsType2 == 5){  // Two small cacti hitbox
                if(x1 < x3 + 30){
                    if(y1 + 43 > 145){
                        break;
                    }
                }
            }
            if(obsType2 == 6){  // Pterodactyl hitbox
                if(x1 < x3 + 45){
                    if(y1 + 10 > y4 + 20 && !crouch){
                        break;
                    }
                }
            }
        }

        frame++;
        touch = false;
        Sleep(20);
    }
    LCD.SetBackgroundColor(WHITE);
    LCD.Clear();

    // Black border rectangles
    LCD.FillRectangle(52, 66, 105, 55); // Top right
    LCD.FillRectangle(162, 66, 105, 55);    // Top left

    LCD.SetFontColor(WHITE);

    // White filler rectangles
    LCD.FillRectangle(57, 71, 95, 45);  // Top right
    LCD.FillRectangle(167, 71, 95, 45);  // Top left

    LCD.SetFontColor(BLACK);

    // Text
    LCD.WriteAt("Replay", 62, 76);    // Top right
    LCD.WriteAt("Menu", 172, 76);   // Top left

    // Change colors to show button has been pressed
    if(x > 57 && x < 152 && y > 71 && y < 116){
        LCD.SetFontColor(BLUE);
        LCD.FillRectangle(52, 66, 105, 55); // Top right
        LCD.SetFontColor(WHITE);
        LCD.FillRectangle(57, 71, 95, 45);  // Top right
        LCD.SetFontColor(BLUE);
        LCD.WriteAt("Replay", 62, 76);    // Top right
        Sleep(500);

        return(1);
    }
    else if(x > 167 && x < 262 && y > 71 && y <116){
        LCD.SetFontColor(BLUE);
        LCD.FillRectangle(162, 66, 105, 55);    // Top left
        LCD.SetFontColor(WHITE);
        LCD.FillRectangle(167, 71, 95, 45);  // Top left
        LCD.SetFontColor(BLUE);
        LCD.WriteAt("Quit", 172, 76);   // Top left
        Sleep(500);

        return(0);
    }
}

void DrawFrame(int frame, int y1, int crouch, int x2, int x3, int obsType1, int obsType2){

    LCD.SetBackgroundColor(WHITE);
    LCD.SetFontColor(BLACK);

    LCD.Write(y1);
    LCD.DrawHorizontalLine(180, 0, 319);
    LCD.DrawHorizontalLine(181, 0, 319);
    LCD.DrawHorizontalLine(182, 0, 319);

    if(frame % 4 >= 0 && frame % 4 < 2 && y1 == 137 && !crouch){
        DrawDinoL(y1, DINOCOLOR);
    }
    else if(frame % 4 >= 2 && frame % 4 <= 3 && y1 == 137 && !crouch){
        DrawDinoR(y1, DINOCOLOR);
    }
    else if(y1 < 137 && !crouch){
        DrawDinoJ(y1, DINOCOLOR);
    }
    else if(crouch){
        DrawDinoC(y1 + 10, DINOCOLOR);
    }

    switch(obsType1){
        case 0: break;
        case 1: DrawBigObs(x2, DINOCOLOR);
                break;
        case 2: DrawBigSmallObs(x2, DINOCOLOR);
                break;
        case 3: DrawSmallObs(x2, DINOCOLOR);
                break;
        case 4: Draw2BigObs(x2, DINOCOLOR);
                break;
        case 5: Draw2SmallObs(x2, DINOCOLOR);
                break;
        case 6: DrawPterodactyl(x2, DINOCOLOR);
                break;
    }
    switch(obsType2){
        case 0: break;
        case 1: DrawBigObs(x3, DINOCOLOR);
                break;
        case 2: DrawBigSmallObs(x3, DINOCOLOR);
                break;
        case 3: DrawSmallObs(x3, DINOCOLOR);
                break;
        case 4: Draw2BigObs(x3, DINOCOLOR);
                break;
        case 5: Draw2SmallObs(x3, DINOCOLOR);
                break;
        case 6: DrawPterodactyl(x3, DINOCOLOR);
                break;
    }
}

void ClearLCD(int frame, int y1, int crouch, int x2, int x3, int obsType1, int obsType2){

    LCD.SetBackgroundColor(WHITE);
    LCD.SetFontColor(WHITE);

    LCD.Write(y1);
    LCD.DrawHorizontalLine(180, 0, 319);
    LCD.DrawHorizontalLine(181, 0, 319);
    LCD.DrawHorizontalLine(182, 0, 319);

    if(frame % 4 >= 0 && frame % 4 < 2 && y1 == 137 && !crouch){
        DrawDinoL(y1, 16777215);
    }
    else if(frame % 4 >= 2 && frame % 4 <= 3 && y1 == 137 && !crouch){
        DrawDinoR(y1, 16777215);
    }
    else if(y1 < 137 && !crouch){
        DrawDinoJ(y1, 16777215);
    }
    else if(crouch){
        DrawDinoC(y1 + 10, 16777215);
    }

    switch(obsType1){
        case 0: break;
        case 1: DrawBigObs(x2, 16777215);
                break;
        case 2: DrawBigSmallObs(x2, 16777215);
                break;
        case 3: DrawSmallObs(x2, 16777215);
                break;
        case 4: Draw2BigObs(x2, 16777215);
                break;
        case 5: Draw2SmallObs(x2, 16777215);
                break;
        case 6: DrawPterodactyl(x2, 16777215);
                break;
    }
    switch(obsType2){
        case 0: break;
        case 1: DrawBigObs(x3, 16777215);
                break;
        case 2: DrawBigSmallObs(x3, 16777215);
                break;
        case 3: DrawSmallObs(x3, 16777215);
                break;
        case 4: Draw2BigObs(x3, 16777215);
                break;
        case 5: Draw2SmallObs(x3, 16777215);
                break;
        case 6: DrawPterodactyl(x3, 16777215);
                break;
    }
}

void InstructionsDisp(){
    float x, y;
    LCD.SetBackgroundColor(WHITE);
    LCD.Clear();
    LCD.SetFontColor(BLACK);
    LCD.Write("Instructions");

    // Wait for touch
    while(LCD.Touch(&x, &y));

    // Wait for touch release
    while(!LCD.Touch(&x, &y));

    Sleep(300);
}

void StatsDisp(){
    float x, y;
    LCD.SetBackgroundColor(WHITE);
    LCD.Clear();
    LCD.SetFontColor(BLACK);
    LCD.Write("Statistics");

    // Wait for touch
    while(LCD.Touch(&x, &y));

    // Wait for touch release
    while(!LCD.Touch(&x, &y));

    Sleep(300);
}

void CreditsDisp(){
    float x, y;
    LCD.SetBackgroundColor(WHITE);
    LCD.Clear();
    LCD.SetFontColor(BLACK);
    LCD.Write("Credits");

    // Wait for touch
    while(LCD.Touch(&x, &y));

    // Wait for touch release
    while(!LCD.Touch(&x, &y));

    Sleep(300);
}

void DrawDinoR(int y1, int color){
    // x1 and y1 are top left corner coordinates

    //****************Standard among dinos R, L, and J****************
    LCD.SetFontColor(color);

    LCD.FillRectangle(22 + x1, y1, 18, 15);
    LCD.FillRectangle(13 + x1, 19 + y1, 13, 15);
    LCD.FillRectangle(18 + x1, 15 + y1, 11, 2);
    LCD.FillRectangle(16 + x1, 17 + y1, 14, 2);
    LCD.FillRectangle(25 + x1, 19 + y1, 4, 10);
    LCD.FillRectangle(25 + x1, 29 + y1, 2, 3);
    LCD.FillRectangle(29 + x1, 19 + y1, 4, 3);
    LCD.FillRectangle(32 + x1, 23 + y1, 2, 2);
    LCD.FillRectangle(10 + x1, 23 + y1, 2, 2);
    LCD.FillRectangle(7 + x1, 24 + y1, 6, 10);
    LCD.FillRectangle(8 + x1, 34 + y1, 15, 2);
    LCD.FillRectangle(x1, 15 + y1, 2, 14);
    LCD.FillRectangle(2 + x1, 19 + y1, 2, 12);
    LCD.FillRectangle(4 + x1, 22 + y1, 2, 11);

    LCD.SetFontColor(WHITE);

    LCD.FillRectangle(21 + x1, y1, 2, 2);
    LCD.FillRectangle(39 + x1, y1, 1, 2);
    LCD.FillRectangle(25 + x1, 3 + y1, 2, 2);
    LCD.FillRectangle(31 + x1, 11 + y1, 9, 2);
    LCD.FillRectangle(37 + x1, 13 + y1, 3, 2);
    //****************End standard****************

    //****************Unique to right leg down****************
    LCD.SetFontColor(color);
    LCD.FillRectangle(10 + x1, 36 + y1, 4, 4);
    LCD.FillRectangle(14 + x1, 36 + y1, 2, 2);
    LCD.FillRectangle(10 + x1, 39 + y1, 2, 3);
    LCD.DrawHorizontalLine(42 + y1, 12 + x1, 13 + x1);
    LCD.FillRectangle(21 + x1, 37 + y1, 5, 2);
}

void DrawDinoL(int y1, int color){
    // x1 and y1 are top left corner coordinates

    //****************Standard among dinos R, L, and J****************
    LCD.SetFontColor(color);

    LCD.FillRectangle(22 + x1, y1, 18, 15);
    LCD.FillRectangle(13 + x1, 19 + y1, 13, 15);
    LCD.FillRectangle(18 + x1, 15 + y1, 11, 2);
    LCD.FillRectangle(16 + x1, 17 + y1, 14, 2);
    LCD.FillRectangle(25 + x1, 19 + y1, 4, 10);
    LCD.FillRectangle(25 + x1, 29 + y1, 2, 3);
    LCD.FillRectangle(29 + x1, 19 + y1, 4, 3);
    LCD.FillRectangle(32 + x1, 23 + y1, 2, 2);
    LCD.FillRectangle(10 + x1, 23 + y1, 2, 2);
    LCD.FillRectangle(7 + x1, 24 + y1, 6, 10);
    LCD.FillRectangle(8 + x1, 34 + y1, 15, 2);
    LCD.FillRectangle(x1, 15 + y1, 2, 14);
    LCD.FillRectangle(2 + x1, 19 + y1, 2, 12);
    LCD.FillRectangle(4 + x1, 22 + y1, 2, 11);

    LCD.SetFontColor(WHITE);

    LCD.FillRectangle(21 + x1, y1, 2, 2);
    LCD.FillRectangle(39 + x1, y1, 1, 2);
    LCD.FillRectangle(25 + x1, 3 + y1, 2, 2);
    LCD.FillRectangle(31 + x1, 11 + y1, 9, 2);
    LCD.FillRectangle(37 + x1, 13 + y1, 3, 2);
    //****************End standard****************

    //****************Unique to left leg down****************
    LCD.SetFontColor(color);

    LCD.DrawHorizontalLine(36 + y1, 11 + x1, 14 + x1);
    LCD.FillRectangle(13 + x1, 37 + y1, 4, 2);
    LCD.DrawHorizontalLine(36 + y1, 19 + x1, 22 + x1);
    LCD.FillRectangle(21 + x1, 37 + y1, 2, 6);
    LCD.FillRectangle(23 + x1, 41 + y1, 2, 2);
}

void DrawDinoJ(int y1, int color){
    // x1 and y1 are top left corner coordinates

    //****************Standard among dinos R, L, and J****************
    LCD.SetFontColor(color);

    LCD.FillRectangle(22 + x1, y1, 18, 15);
    LCD.FillRectangle(13 + x1, 19 + y1, 13, 15);
    LCD.FillRectangle(18 + x1, 15 + y1, 11, 2);
    LCD.FillRectangle(16 + x1, 17 + y1, 14, 2);
    LCD.FillRectangle(25 + x1, 19 + y1, 4, 10);
    LCD.FillRectangle(25 + x1, 29 + y1, 2, 3);
    LCD.FillRectangle(29 + x1, 19 + y1, 4, 3);
    LCD.FillRectangle(32 + x1, 23 + y1, 2, 2);
    LCD.FillRectangle(10 + x1, 23 + y1, 2, 2);
    LCD.FillRectangle(7 + x1, 24 + y1, 6, 10);
    LCD.FillRectangle(8 + x1, 34 + y1, 15, 2);
    LCD.FillRectangle(x1, 15 + y1, 2, 14);
    LCD.FillRectangle(2 + x1, 19 + y1, 2, 12);
    LCD.FillRectangle(4 + x1, 22 + y1, 2, 11);

    LCD.SetFontColor(WHITE);

    LCD.FillRectangle(21 + x1, y1, 2, 2);
    LCD.FillRectangle(39 + x1, y1, 1, 2);
    LCD.FillRectangle(25 + x1, 3 + y1, 2, 2);
    LCD.FillRectangle(31 + x1, 11 + y1, 9, 2);
    LCD.FillRectangle(37 + x1, 13 + y1, 3, 2);
    //****************End standard****************

    //****************Unique to jumping****************
    LCD.SetFontColor(color);

    LCD.DrawHorizontalLine(36 + y1, 11 + x1, 14 + x1);
    LCD.FillRectangle(13 + x1, 37 + y1, 2, 6);
    LCD.FillRectangle(15 + x1, 41 + y1, 2, 2);
    LCD.DrawHorizontalLine(36 + y1, 19 + x1, 22 + x1);
    LCD.FillRectangle(21 + x1, 37 + y1, 2, 6);
    LCD.FillRectangle(23 + x1, 41 + y1, 2, 2);
}

void DrawDinoC(int y1, int color){
    // x1 and y1 are top left corner coordinates

    LCD.SetFontColor(color);

    LCD.FillRectangle(x1 + 18, y1 + 3, 24, 19);
    LCD.FillRectangle(x1, y1, 3, 8);
    LCD.FillRectangle(x1 + 3, y1 + 3, 2, 8);
    LCD.FillRectangle(x1 + 5, y1 + 3, 3, 10);
    LCD.FillRectangle(x1 + 8, y1 + 5, 10, 11);
    LCD.FillRectangle(x1 + 10, y1 + 16, 8, 2);
    LCD.FillRectangle(x1 + 13, y1 + 18, 5, 3);
    LCD.FillRectangle(x1 + 42, y1 + 1, 28, 15);
    LCD.FillRectangle(x1 + 42, y1 + 16, 15, 2);
    LCD.FillRectangle(x1 + 47, y1 + 18, 17, 2);
    LCD.FillRectangle(x1 + 37, y1 + 22, 4, 1);
    LCD.FillRectangle(x1 + 37, y1 + 23, 2, 1);
    LCD.FillRectangle(x1 + 37, y1 + 24, 5, 2);
    LCD.FillRectangle(x1 + 16, y1 + 21, 14, 2);
    LCD.FillRectangle(x1 + 15, y1 + 23, 7, 2);
    LCD.FillRectangle(x1 + 23, y1 + 23, 7, 2);
    LCD.FillRectangle(x1 + 15, y1 + 25, 6, 1);
    LCD.FillRectangle(x1 + 23, y1 + 25, 4, 2);
    LCD.FillRectangle(x1 + 15, y1 + 26, 4, 2);
    LCD.FillRectangle(x1 + 23, y1 + 27, 3, 2);
    LCD.FillRectangle(x1 + 15, y1 + 28, 3, 2);
    LCD.FillRectangle(x1 + 15, y1 + 30, 2, 3);
    LCD.FillRectangle(x1 + 17, y1 + 31, 2, 2);
    LCD.FillRectangle(x1 + 23, y1 + 29, 2, 4);
    LCD.FillRectangle(x1 + 25, y1 + 31, 2, 2);

    LCD.SetFontColor(WHITE);
    LCD.FillRectangle(x1 + 42, y1 + 1, 2, 4);
    LCD.FillRectangle(x1 + 44, y1 + 1, 3, 3);
    LCD.DrawPixel(x1 + 47, y1 + 1);
    LCD.FillRectangle(x1 + 68, y1 + 1, 2, 2);
    LCD.FillRectangle(x1 + 49, y1 + 5, 3, 3);
}

void DrawBigObs(int x2, int color){
    LCD.SetFontColor(color);

    LCD.FillRectangle(x2 + 8, y2, 7, 48);
    LCD.FillRectangle(x2 + 18, y2 + 6, 5, 21);
    LCD.FillRectangle(x2, y2 + 6, 7, 15);
    LCD.FillRectangle(x2 + 1, y2 + 18, 7, 5);
    LCD.FillRectangle(x2 + 15, y2 + 25, 7, 4);
    LCD.DrawHorizontalLine(y2 + 23, x2 + 3, x2 + 7);
    LCD.DrawHorizontalLine(y2 + 29, x2 + 15, x2 + 19);

    LCD.SetFontColor(WHITE);

    LCD.DrawPixel(x2 + 8, y2);
    LCD.DrawPixel(x2 + 14, y2);
    LCD.DrawPixel(x2, y2 + 6);
    LCD.DrawPixel(x2 + 5, y2 + 6);
    LCD.DrawPixel(x2 + 18, y2 + 6);
    LCD.DrawPixel(x2 + 22, y2 + 6);
}

void Draw2BigObs(int x2, int color){
    LCD.SetFontColor(color);

    LCD.FillRectangle(x2 + 8, y2, 7, 48);
    LCD.FillRectangle(x2 + 18, y2 + 6, 5, 21);
    LCD.FillRectangle(x2, y2 + 6, 7, 15);
    LCD.FillRectangle(x2 + 1, y2 + 18, 7, 5);
    LCD.FillRectangle(x2 + 15, y2 + 25, 7, 4);
    LCD.DrawHorizontalLine(y2 + 23, x2 + 3, x2 + 7);
    LCD.DrawHorizontalLine(y2 + 29, x2 + 15, x2 + 19);

    LCD.SetFontColor(WHITE);

    LCD.DrawPixel(x2 + 8, y2);
    LCD.DrawPixel(x2 + 14, y2);
    LCD.DrawPixel(x2, y2 + 6);
    LCD.DrawPixel(x2 + 5, y2 + 6);
    LCD.DrawPixel(x2 + 18, y2 + 6);
    LCD.DrawPixel(x2 + 22, y2 + 6);

    x2 = x2 + 25;

    LCD.SetFontColor(color);

    LCD.FillRectangle(x2 + 8, y2, 7, 48);
    LCD.FillRectangle(x2 + 18, y2 + 6, 5, 21);
    LCD.FillRectangle(x2, y2 + 6, 7, 15);
    LCD.FillRectangle(x2 + 1, y2 + 18, 7, 5);
    LCD.FillRectangle(x2 + 15, y2 + 25, 7, 4);
    LCD.DrawHorizontalLine(y2 + 23, x2 + 3, x2 + 7);
    LCD.DrawHorizontalLine(y2 + 29, x2 + 15, x2 + 19);

    LCD.SetFontColor(WHITE);

    LCD.DrawPixel(x2 + 8, y2);
    LCD.DrawPixel(x2 + 14, y2);
    LCD.DrawPixel(x2, y2 + 6);
    LCD.DrawPixel(x2 + 5, y2 + 6);
    LCD.DrawPixel(x2 + 18, y2 + 6);
    LCD.DrawPixel(x2 + 22, y2 + 6);
}

void DrawSmallObs(int x3, int color){
    LCD.SetFontColor(color);

    LCD.FillRectangle(x3 + 4, y3, 4, 24);
    LCD.FillRectangle(x3 + 9, y3 + 4, 3, 7);
    LCD.FillRectangle(x3, y3 + 4, 3, 10);
    LCD.FillRectangle(x3 + 1, y3 + 13, 3, 2);
    LCD.FillRectangle(x3 + 8, y3 + 9, 3, 3);
    LCD.DrawPixel(x3 + 1, y3 + 3);
    LCD.DrawPixel(x3 + 10, y3 + 3);

    LCD.SetFontColor(WHITE);
    LCD.DrawPixel(x3 + 4, y3);
    LCD.DrawPixel(x3 + 7, y3);
}

void Draw2SmallObs(int x3, int color){
    LCD.SetFontColor(color);

    LCD.FillRectangle(x3 + 4, y3, 4, 24);
    LCD.FillRectangle(x3 + 9, y3 + 4, 3, 7);
    LCD.FillRectangle(x3, y3 + 4, 3, 10);
    LCD.FillRectangle(x3 + 1, y3 + 13, 3, 2);
    LCD.FillRectangle(x3 + 8, y3 + 9, 3, 3);
    LCD.DrawPixel(x3 + 1, y3 + 3);
    LCD.DrawPixel(x3 + 10, y3 + 3);

    LCD.SetFontColor(WHITE);
    LCD.DrawPixel(x3 + 4, y3);
    LCD.DrawPixel(x3 + 7, y3);

    x3 = x3 + 15;

    LCD.SetFontColor(color);

    LCD.FillRectangle(x3 + 4, y3, 4, 24);
    LCD.FillRectangle(x3 + 9, y3 + 4, 3, 7);
    LCD.FillRectangle(x3, y3 + 4, 3, 10);
    LCD.FillRectangle(x3 + 1, y3 + 13, 3, 2);
    LCD.FillRectangle(x3 + 8, y3 + 9, 3, 3);
    LCD.DrawPixel(x3 + 1, y3 + 3);
    LCD.DrawPixel(x3 + 10, y3 + 3);

    LCD.SetFontColor(WHITE);
    LCD.DrawPixel(x3 + 4, y3);
    LCD.DrawPixel(x3 + 7, y3);
}

void DrawBigSmallObs(int x2, int color){

    LCD.SetFontColor(color);

    LCD.FillRectangle(x2 + 8, y2, 7, 48);
    LCD.FillRectangle(x2 + 18, y2 + 6, 5, 21);
    LCD.FillRectangle(x2, y2 + 6, 7, 15);
    LCD.FillRectangle(x2 + 1, y2 + 18, 7, 5);
    LCD.FillRectangle(x2 + 15, y2 + 25, 7, 4);
    LCD.DrawHorizontalLine(y2 + 23, x2 + 3, x2 + 7);
    LCD.DrawHorizontalLine(y2 + 29, x2 + 15, x2 + 19);

    LCD.SetFontColor(WHITE);

    LCD.DrawPixel(x2 + 8, y2);
    LCD.DrawPixel(x2 + 14, y2);
    LCD.DrawPixel(x2, y2 + 6);
    LCD.DrawPixel(x2 + 5, y2 + 6);
    LCD.DrawPixel(x2 + 18, y2 + 6);
    LCD.DrawPixel(x2 + 22, y2 + 6);

    x2 = x2 + 25;

    LCD.SetFontColor(color);

    LCD.FillRectangle(x2 + 4, y3, 4, 24);
    LCD.FillRectangle(x2 + 9, y3 + 4, 3, 7);
    LCD.FillRectangle(x2, y3 + 4, 3, 10);
    LCD.FillRectangle(x2 + 1, y3 + 13, 3, 2);
    LCD.FillRectangle(x2 + 8, y3 + 9, 3, 3);
    LCD.DrawPixel(x2 + 1, y3 + 3);
    LCD.DrawPixel(x2 + 10, y3 + 3);

    LCD.SetFontColor(WHITE);
    LCD.DrawPixel(x2 + 4, y3);
    LCD.DrawPixel(x2 + 7, y3);
}

void DrawPterodactyl(int x4, int color){
    LCD.SetFontColor(color);

    LCD.FillRectangle(x4, y4, 18, 13);
    LCD.FillRectangle(x4 + 14, y4 + 5, 4, 11);
    LCD.FillRectangle(x4 + 17, y4 + 9, 2, 9);
    LCD.FillRectangle(x4 + 19, y4 + 9, 4, 28);
    LCD.FillRectangle(x4 + 23, y4 + 9, 2, 26);
    LCD.FillRectangle(x4 + 25, y4 + 9, 3, 21);
    LCD.FillRectangle(x4 + 28, y4 + 9, 2, 19);
    LCD.FillRectangle(x4 + 30, y4 + 9, 5, 16);
    LCD.FillRectangle(x4 + 35, y4 + 12, 3, 13);
    LCD.FillRectangle(x4 + 38, y4 + 14, 4, 11);
    LCD.FillRectangle(x4 + 42, y4 + 14, 2, 9);
    LCD.FillRectangle(x4 + 44, y4 + 19, 6, 4);
    LCD.FillRectangle(x4 + 44, y4 + 14, 7, 4);

    LCD.SetFontColor(WHITE);

    LCD.FillRectangle(x4, y4, 2, 9);
    LCD.FillRectangle(x4 + 2, y4, 3, 7);
    LCD.FillRectangle(x4 + 5, y4, 2, 5);
    LCD.FillRectangle(x4 + 7, y4, 3, 2);
}
