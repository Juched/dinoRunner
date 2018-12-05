#include <FEHLCD.h>
#include <LCDColors.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <stdlib.h>
#include <FEHBuzzer.h>

#define JUMPHEIGHT 30
#define GAMESPEED 20
#define DINOCOLOR 000000
#define x1 40   // Horizontal position of dino
#define y2 132  // Vertical position of big cacti
#define y3 156  // Vertical position of small cacti
#define y4 100   // Vertical position of pterodactyl
#define OBSTRATE 30 // Rate at which obstacles appear

class Stats{    // Class containing functions and variables for statistics calculations
public:
    Stats();
    void CumulativeScore(int);  // Cumulative score among all runs
    void HighScore(int);    // Highest scoring run
    void JumpCount(int);    // Number of times jumped
    void JukeCount(int);    // Number of times avoiding obstacles
    void StatsDisp();   // Method to display statistics
    int PlayGame();  // Function where calculations are made to print things in the right location
private:
    int cScore, hScore, totalhops, totaljukes;
};

int MainMenu(); // Menu option function
void CreditsDisp(); // Credits display function
void InstructionsDisp();    // Instructions display function
void ClearLCD(int frame, int y1, int crouch, int x2, int x3, int obsType1, int obsType2);   // Rewrites the previous frame with white to avoid flashing
void DrawFrame(int frame, int y1, int crouch, int x2, int x3, int obsType1, int obsType2);  // Draws each frame
void DrawDinoR(int y, int color);   // Dino with right leg down
void DrawDinoL(int y, int color);   // Dino with left leg down
void DrawDinoJ(int y, int color);   // Jumping dino
void DrawDinoC(int y, int color);   // Crouching dino
void DrawBigObs(int x, int color);  // 1 big cactus
void Draw2BigObs(int x, int color); // 2 big cacti
void DrawSmallObs(int x, int color);    // 1 small cactus
void Draw2SmallObs(int x, int color);   // 2 small cacti
void DrawBigSmallObs(int x, int color); // 1 big and 1 small cacti
void DrawPterodactyl(int x, int color); // Pterodactyl

int main(void){
    int choice, replay;

    Stats S;    // Declare object of Stats class, passed into play game function

    do{
        choice = MainMenu();    // Call main menu function, returns a value based on the option selected
        replay = 1;
        switch(choice){
            case 1: while(replay == 1){ // Run forever so long as replay keeps getting pressed
                        replay = S.PlayGame();    // Returns a 1 if the player chooses to play again, a 0 if main menu is pressed
                    }
                    break;
            case 2: S.StatsDisp();  // Display statistics
                    break;
            case 3: InstructionsDisp(); // Display instructions
                    break;
            case 4: CreditsDisp();  // Display credits
                    break;
        }
    }while(true);   // Run indefinitely
}

Stats::Stats(){ // Constructor
    cScore = 0;
    hScore = 0;
    totalhops = 0;
    totaljukes = 0;
}

void Stats::CumulativeScore(int frame){ // Calculate cumulative score
    cScore = cScore + frame;
}

void Stats::HighScore(int frame){   // Calculate high score
    if(frame > hScore){
        hScore = frame;
    }
}

void Stats::JukeCount(int jukes){   // Calculate total jukes
    totaljukes = totaljukes + jukes;
}

void Stats::JumpCount(int hops){    // Calculate total hops
    totalhops = totalhops + hops;
}

void Stats::StatsDisp(){    // Display stats function
    float x, y;

    // Reset screen
    LCD.SetBackgroundColor(WHITE);
    LCD.Clear();
    LCD.SetFontColor(BLACK);

    // Print values
    LCD.WriteLine("Statistics");
    LCD.WriteLine("");
    LCD.Write("High Score: ");
    LCD.WriteLine(hScore);
    LCD.Write("Total Score: ");
    LCD.WriteLine(cScore);
    LCD.Write("Total Avoided: ");
    LCD.WriteLine(totalhops);
    LCD.Write("Total Jumps: ");
    LCD.WriteLine(totalhops);

    // Wait for touch
    while(LCD.Touch(&x, &y));

    // Wait for touch release
    while(!LCD.Touch(&x, &y));

    Sleep(300);
}

int MainMenu(){
    float x, y;

    // Reset screen
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

        return(1);  // Return value for play option
    }
    else if(x > 167 && x < 262 && y > 71 && y <116){
        LCD.SetFontColor(BLUE);
        LCD.FillRectangle(162, 66, 105, 55);    // Top left
        LCD.SetFontColor(WHITE);
        LCD.FillRectangle(167, 71, 95, 45);  // Top left
        LCD.SetFontColor(BLUE);
        LCD.WriteAt("Stats", 172, 76);   // Top left
        Sleep(500);

        return(2);  // Return value for stats option
    }
    else if(x > 57 && x < 152 && y > 131 && y < 176){
        LCD.SetFontColor(BLUE);
        LCD.FillRectangle(52, 126, 105, 55); // Bottom right
        LCD.SetFontColor(WHITE);
        LCD.FillRectangle(57, 131, 95, 45);  // Bottom right
        LCD.SetFontColor(BLUE);
        LCD.WriteAt("Instructions", 62, 136);    // Bottom right
        Sleep(500);

        return(3);  // Return value for instructions option
    }
    else if(x > 167 && x < 262 && y > 131 && y < 176){
        LCD.SetFontColor(BLUE);
        LCD.FillRectangle(162, 126, 105, 55); // Bottom left
        LCD.SetFontColor(WHITE);
        LCD.FillRectangle(167, 131, 95, 45);  // Bottom left
        LCD.SetFontColor(BLUE);
        LCD.WriteAt("Credits", 172, 136); // Bottom left
        Sleep(500);

        return(4);  // Return value for credits option
    }
}

int Stats::PlayGame(){
    float x, y;
    bool gameOver = false, touch = false, jump = true;
    int y1 = 137, crouch = 0, x2 = 300, x3;
    int frame = 1, hops = 0, jukes = 0;
    int vel = 0, grav = 4, x2vel = 0, x3vel = 0, obsType1, obsType2;
    int rand_num, x2type, x3type;
    int highScore = 0;

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
        // Overwrite previous frame with white color to avoid flashing that ocurrs with .Clear()
        ClearLCD(frame - 1, y1, crouch, x2, x3, obsType1, obsType2);

        crouch = 0;
        touch = LCD.Touch(&x, &y);  // Look for touched screen
        LCD.SetFontColor(BLACK);
        LCD.WriteAt("Highscore: ", 0, 0);
        LCD.WriteAt(hScore, 120, 0);
        LCD.WriteAt("Score: ", 0, 25);
        LCD.WriteAt(frame, 75, 25);    // Score is kept as number frames survived

        //*********************************Jump controls*********************************

        // Condition to set velocity to dino
        if(touch && y1 > 75 && y < 120 && jump){    // Jump when top half of screen is pressed
            vel = 25;
            Buzzer.Tone(FEHBuzzer::A6, 50); // Tone when jumping
            hops++; // Jump counter
        }
        else if(touch && y >= 120){ // Crouch when bottom half of screen is pressed
            if(y1 < 137){
                vel = vel - 7;  // Go down faster when crouching if airborne
                crouch = 1;
            }else{
                crouch = 1;
            }
        }

        // Max jump height
        if(y1 > 75){
            jump = false;
        }

        y1 = y1 - vel;  // Jump upwards by decreasing y1 coordinate

        // Stop when back on the ground
        if(y1 > 137){
            y1 = 137;
            vel = 0;
            jump = true;
        }

        // Acceleration constant
        vel = vel - grav;

        //*********************************Obstacles*********************************

        x2 = x2 - x2vel;    // Moves obstacle 1 to the left
        x3 = x3 - x3vel;    // Moves obstacle 2 to the left

        // Start condition
        if(x2 <= 0){
            x2 = 300;
            x2vel = 0;
            obsType1 = 0;
        }
        // Obstacle returns to start when at end of screen
        if(x3 <= 0){
            x3 = 300;
            x3vel = 0;
            obsType2 = 0;
        }

        if(frame % OBSTRATE == 0){  // Obstacle rate adjustment
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

        // Draw entire frame
        DrawFrame(frame, y1, crouch, x2, x3, obsType1, obsType2);

        //*********************************Hitboxes*********************************
        // obsType1 and 2 signify a max of 2 obstacles on the screen at one time

        if(x1 + 35 >= x2 || x1 + 35 >= x3){
            if(obsType1 == 1){  // One big cactus hitbox
                if(x1 < x2 + 30){
                    if(y1 + 43 > 132){
                        break;
                    }
                }
            }
            if(obsType1 == 2){  // Big and small cactus hitbox
                if(x1 < x2 + 35){
                    if(y1 + 43 > 132){
                        break;
                    }
                }
            }
            if(obsType1 == 3){  // One small cactus hitbox
                if(x1 < x2 + 25){
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
                    if(y1 + 43 > 156){
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
                    if(y1 + 43 > 156){
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
            else{
                jukes++;    // If no hitbox is encountered, increase avoided obstacle counter
            }
        }

        // Every 100 points/frames, make a noise
        if(frame % 100 == 0){
            Buzzer.Tone(FEHBuzzer::A7, 50);
        }

        // Frame counter
        frame++;

        // Reset touch
        touch = false;

        // Buffer
        Sleep(20);
    }   // End of while loop

    // Death noise
    Buzzer.Tone(FEHBuzzer::A7, 750);

    // Calculate statistics
    CumulativeScore(frame);
    HighScore(frame);
    JumpCount(hops);
    JukeCount(jukes);

    // Clear screen
    LCD.SetBackgroundColor(WHITE);
    LCD.Clear();

    LCD.SetFontColor(BLACK);

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
    LCD.WriteAt("Quit", 172, 76);   // Top left

    while(true){
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
            LCD.WriteAt("Replay", 62, 76);    // Top right
            Sleep(500);

            return(1);  // Replay option
        }
        else if(x > 167 && x < 262 && y > 71 && y <116){
            LCD.SetFontColor(BLUE);
            LCD.FillRectangle(162, 66, 105, 55);    // Top left
            LCD.SetFontColor(WHITE);
            LCD.FillRectangle(167, 71, 95, 45);  // Top left
            LCD.SetFontColor(BLUE);
            LCD.WriteAt("Quit", 172, 76);   // Top left
            Sleep(500);

            return(0);  // Quit option
        }
    }
}   // End PlayGame

void DrawFrame(int frame, int y1, int crouch, int x2, int x3, int obsType1, int obsType2){

    // Set colors
    LCD.SetBackgroundColor(WHITE);
    LCD.SetFontColor(BLACK);

    // Draw ground
    LCD.DrawHorizontalLine(180, 0, 319);
    LCD.DrawHorizontalLine(181, 0, 319);
    LCD.DrawHorizontalLine(182, 0, 319);

    // Switch between right and left legs down every 2 frames
    if(frame % 4 >= 0 && frame % 4 < 2 && y1 == 137 && !crouch){
        DrawDinoL(y1, DINOCOLOR);
    }
    else if(frame % 4 >= 2 && frame % 4 <= 3 && y1 == 137 && !crouch){
        DrawDinoR(y1, DINOCOLOR);
    }

    // Draw jumping dino
    else if(y1 < 137 && !crouch){
        DrawDinoJ(y1, DINOCOLOR);
    }

    // Draw crouching dino
    else if(crouch){
        DrawDinoC(y1 + 10, DINOCOLOR);
    }

    if(x2 != 300){
        // Cases for each randomly generated object type
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
    }
    if(x3 != 300){
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
}

void ClearLCD(int frame, int y1, int crouch, int x2, int x3, int obsType1, int obsType2){
    // Same as DrawFrame except it is passed previous frame and color is always white

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

    // Reset screen
    LCD.SetBackgroundColor(WHITE);
    LCD.Clear();
    LCD.SetFontColor(BLACK);

    // Print instructions
    LCD.WriteLine("Instructions");
    LCD.WriteLine("");
    LCD.WriteLine("1. Tap on the top half of the screen to jump.");
    LCD.WriteLine("2. Tap on the bottom half of the screen to crouch.");
    LCD.WriteLine("3. Crouching while airborne makes you go down faster.");
    LCD.WriteLine("4. Avoid making contact with any obstacles.");
    LCD.WriteLine("5. Good luck!");

    // Wait for touch
    while(LCD.Touch(&x, &y));

    // Wait for touch release
    while(!LCD.Touch(&x, &y));

    Sleep(300);
}

void CreditsDisp(){
    float x, y;

    // Reset screen
    LCD.SetBackgroundColor(WHITE);
    LCD.Clear();
    LCD.SetFontColor(BLACK);

    // Print credits
    LCD.WriteLine("Credits");
    LCD.WriteLine("");
    LCD.WriteLine("Authors: Alex Myers and Parth Parekh");
    LCD.WriteLine("");
    LCD.WriteLine("Sources: ");
    LCD.WriteLine("\thttps://opencv.org");
    LCD.WriteLine("\thttps://chromedino.com");
    LCD.WriteLine("\thttps://u.osu.edu/fehproteus");
    LCD.WriteLine("");
    LCD.WriteLine("Website: https://u.osu.edu/feh1281au18sec23585i2/");


    // Wait for touch
    while(LCD.Touch(&x, &y));

    // Wait for touch release
    while(!LCD.Touch(&x, &y));

    Sleep(300);
}

void DrawDinoR(int y1, int color){  // Right leg down dino
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

void DrawDinoL(int y1, int color){  // Left leg down dino
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

void DrawDinoJ(int y1, int color){  // Jumping dino
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

void DrawDinoC(int y1, int color){  // Crouching dino
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

void DrawBigObs(int x2, int color){ // 1 big cactus
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

void Draw2BigObs(int x2, int color){    // 2 big cacti
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

void DrawSmallObs(int x3, int color){   // 1 small cactus
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

void Draw2SmallObs(int x3, int color){  // 2 small cacti
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

void DrawBigSmallObs(int x2, int color){    // 1 small 1 big cactus

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

void DrawPterodactyl(int x4, int color){    // Pterodactyl
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
