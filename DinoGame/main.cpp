#include <FEHLCD.h>
#include <LCDColors.h>
#include <FEHIO.h>
#include <FEHUtility.h>

int MainMenu(); // Returns an int depending on what option is pressed
int PlayGame();
void StatsDisp();
void CreditsDisp();
void InstructionsDisp();

int main(void){
    int choice, replay = 1;
    bool quit = false;

    while(!quit){
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
    }
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
    LCD.WriteAt("Stats", 62, 76);   // Top left
    LCD.WriteAt("Quit", 62, 76);    // Bottom right
    LCD.WriteAt("Credits", 62, 76); // Bottom left

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
        LCD.WriteAt("Stats", 62, 76);   // Top left
        Sleep(500);

        return(2);
    }
    else if(x > 57 && x < 152 && y > 131 && y < 176){
        LCD.SetFontColor(BLUE);
        LCD.FillRectangle(52, 126, 105, 55); // Bottom right
        LCD.SetFontColor(WHITE);
        LCD.FillRectangle(57, 131, 95, 45);  // Bottom right
        LCD.SetFontColor(BLUE);
        LCD.WriteAt("Quit", 62, 76);    // Bottom right
        Sleep(500);

        return(3);
    }
    else if(x > 167 && x < 262 && y > 131 && y < 176){
        LCD.SetFontColor(BLUE);
        LCD.FillRectangle(162, 126, 105, 55); // Bottom left
        LCD.SetFontColor(WHITE);
        LCD.FillRectangle(167, 131, 95, 45);  // Bottom left
        LCD.SetFontColor(BLUE);
        LCD.WriteAt("Credits", 62, 76); // Bottom left
        Sleep(500);

        return(4);
    }
}

int PlayGame(){
    float x, y;
    bool gameOver = false;
    int jump = 0, crouch = 0;

    LCD.SetBackgroundColor(WHITE);

    LCD.Clear();
    LCD.SetFontColor(BLACK);

    LCD.WriteAt("3", 155, 115);
    Sleep(1000);
    LCD.Clear();
    LCD.WriteAt("2", 155, 115);
    Sleep(1000);
    LCD.Clear();
    LCD.WriteAt("1", 155, 115);
    Sleep(1000);
    LCD.Clear();
    LCD.WriteAt("Go!", 155, 115);
    Sleep(500);
    LCD.Clear();

    LCD.DrawHorizontalLine(180, 0, 319);
    LCD.DrawHorizontalLine(181, 0, 319);
    LCD.DrawHorizontalLine(182, 0, 319);

    LCD.DrawRectangle(25, 130, 25, 50);

    while(!gameOver){
        x = 0;
        y = 0;
        // Get touch values
        LCD.Touch(&x, &y);

        if(y > 120){
            crouch = 1;
            LCD.Write("Touched at ");
            LCD.Write(x);
            LCD.Write(", ");
            LCD.WriteLine(y);
            LCD.Write("crouch = ");
            LCD.WriteLine(crouch);
            crouch = 0;
        }
        else if(y < 120 && y != 0){
            jump = 1;
            LCD.Write("Touched at ");
            LCD.Write(x);
            LCD.Write(", ");
            LCD.WriteLine(y);
            LCD.Write("jump = ");
            LCD.WriteLine(jump);
            jump = 0;
        }
    }

    // Wait for touch
    while(LCD.Touch(&x, &y));

    // Wait for touch release
    while(!LCD.Touch(&x, &y));

    return(0);
}

int StatsDisp(){
    LCD.SetBackgroundColor(WHITE);
    LCD.Clear();
    LCD.SetFontColor(BLACK);
    LCD.Write("Statistics");

    // Wait for touch
    while(LCD.Touch(&x, &y));

    // Wait for touch release
    while(!LCD.Touch(&x, &y));
}

int CreditsDisp(){
    LCD.SetBackgroundColor(WHITE);
    LCD.Clear();
    LCD.SetFontColor(BLACK);
    LCD.Write("Credits");

    // Wait for touch
    while(LCD.Touch(&x, &y));

    // Wait for touch release
    while(!LCD.Touch(&x, &y));
}
