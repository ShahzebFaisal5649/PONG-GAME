#include <iostream>
#include <windows.h>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <mmsystem.h>
using namespace std;

int sc1, sc2, scro1, scro2;;
class Paddle
{
private:
    int paddle_x, paddle_y;
    int originalX, originalY;
public:
    Paddle() {
        paddle_x = 0;
        paddle_y = 0;
    }
    Paddle(int dx, int dy) {
        originalX = dx;
        originalY = dy;
        paddle_x = dx;
        paddle_y = dy;
    }
    void Reset() {
        paddle_x = originalX;
        paddle_y = originalY;
    }
    int getX() {
        return paddle_x;
    }
    int getY() {
        return paddle_y;
    }
    void moveUp() {
        paddle_y -= 30;
        if (paddle_y <= 50)
            paddle_y = 50;
    }
    void moveDown() {
        paddle_y += 30;
        if (paddle_y >= originalY * 2 - 60)
            paddle_y = originalY * 2 - 60;
    }
};

enum eDir1 { STOP1 = 0, UP = 1, DOWN = 2 };
class Computer
{
    int computer_x, computer_y;
    int originalX, originalY;
    eDir1 direction1;
public:
    Computer(int posX, int posY)
    {
        originalX = posX;
        originalY = posY;
        computer_x = posX; computer_y = posY;
        direction1 = STOP1;
    }
    void Reset()
    {
        computer_x = originalX;
        computer_y = originalY;
        direction1 = STOP1;
    }
    void changeDirection(eDir1 d)
    {
        direction1 = d;
    }
    void randomDirection()
    {
        direction1 = (eDir1)((rand() % 2) + 1);
    }
    int getX() {
        return computer_x;
    }
    int getY() {
        return computer_y;
    }
    eDir1 getDirection() {
        return direction1;
    }
    void Move() {
        if (direction1 == 1) {
            computer_y -= 10;
            if (computer_y <= 50) {
                computer_y = 50;
            }
        }
        else if (direction1 == 2) {
            computer_y += 10;
            if (computer_y >= originalY * 2 - 60) {
                computer_y = originalY * 2 - 60;
            }
        }
    }

};


enum eDir { STOP = 0, LEFT = 1, UPLEFT = 2, DOWNLEFT = 3, RIGHT = 4, UPRIGHT = 5, DOWNRIGHT = 6 };
class Ball
{
private:
    int ball_x, ball_y;
    int originalX, originalY;
    eDir direction;
public:
    Ball(int posX, int posY)
    {
        originalX = posX;
        originalY = posY;
        ball_x = posX; ball_y = posY;
        direction = STOP;
    }
    void Reset()
    {
        ball_x = originalX; ball_y = originalY;
        direction = STOP;
    }
    void changeDirection(eDir d)
    {
        direction = d;
    }
    void randomDirection()
    {
        direction = (eDir)((rand() % 6) + 1);
    }
    int getX() {
        return ball_x;
    }
    int getY() {
        return ball_y;
    }
    eDir getDirection() {
        return direction;
    }
    void Move() {
        if (direction == 1)
            ball_x -= 10;
        else if (direction == 2) {
            ball_x -= 10;
            ball_y -= 10;
        }
        else if (direction == 3) {
            ball_x -= 10;
            ball_y += 10;
        }
        else if (direction == 4)
            ball_x += 10;
        else if (direction == 5) {
            ball_x += 10;
            ball_y -= 10;
        }
        else if (direction == 6) {
            ball_x += 10;
            ball_y += 10;
        }
    }
};


class Pong
{
private:
    int width, height;
    int score1, score2;
    char s1[2], s2[2];
    bool win;
    char up1, down1, up2, down2;
    bool quit = true;
    bool computer = true;
    bool flag = true;
    Ball* ball;
    Paddle* player1;
    Paddle* player2;
    Paddle* player;
    Computer* computer1;
public:
    void GetWindowDimensions(int& width, int& height) const 
    {
        HWND consoleHandle = GetConsoleWindow();
        RECT rc;
        GetClientRect(consoleHandle, &rc);
        width = rc.right;
        height = rc.bottom;
    }
    void myRect(int x1, int y1, int x2, int y2, COLORREF lineColor, COLORREF fillColor) {
        HWND console_handle = GetConsoleWindow();
        HDC device_context = GetDC(console_handle);

        //change the color by changing the values in RGB (from 0-255)
        HPEN pen = CreatePen(PS_SOLID, 2, lineColor);
        SelectObject(device_context, pen);
        HBRUSH brush = ::CreateSolidBrush(fillColor);
        SelectObject(device_context, brush);

        Rectangle(device_context, x1, y1, x2, y2);
        DeleteObject(pen);
        DeleteObject(brush);
        ReleaseDC(console_handle, device_context);
    }
    void myEllipse(int x1, int y1, int x2, int y2, COLORREF lineColor, COLORREF fillColor) {
        HWND console_handle = GetConsoleWindow();
        HDC device_context = GetDC(console_handle);

        //change the color by changing the values in RGB (from 0-255)
        HPEN pen = CreatePen(PS_SOLID, 2, lineColor);
        SelectObject(device_context, pen);
        HBRUSH brush = ::CreateSolidBrush(fillColor);
        SelectObject(device_context, brush);
        Ellipse(device_context, x1, y1, x2, y2);
        DeleteObject(pen);
        DeleteObject(brush);
        ReleaseDC(console_handle, device_context);
    }
    void myLine(int x1, int y1, int x2, int y2, COLORREF lineColor) {

        HWND console_handle = GetConsoleWindow();
        HDC device_context = GetDC(console_handle);

        //change the color by changing the values in RGB (from 0-255)
        HPEN pen = CreatePen(PS_SOLID, 2, lineColor); //2 is the width of the pen
        SelectObject(device_context, pen);
        MoveToEx(device_context, x1, y1, NULL);
        LineTo(device_context, x2, y2);
        DeleteObject(pen);

        ReleaseDC(console_handle, device_context);
    }
    void SetWindowTitle(const wchar_t* Title) {
        SetConsoleTitleW(Title); // Use SetConsoleTitleW for wide-character strings
    }

    void myDrawTextWithFont(int x, int y, int ht, const wchar_t str[], COLORREF lineColor, COLORREF fillColor) {
        RECT rects;
        HFONT hFont;
        rects.left = x;
        rects.top = y;
        rects.right = x + ht;
        rects.bottom = y + ht;

        HWND console_handle = GetConsoleWindow();
        HDC device_context = GetDC(console_handle);
        hFont = CreateFontW(ht, 0, 0, 0, FW_DONTCARE, FALSE, TRUE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
            CLIP_DEFAULT_PRECIS, 1, VARIABLE_PITCH, TEXT("Impact")); // Use CreateFontW for wide-character strings
        SelectObject(device_context, hFont);

        SetTextColor(device_context, lineColor);
        SetBkColor(device_context, fillColor);
        DrawTextW(device_context, str, -1, &rects, DT_TOP | DT_NOCLIP); // Use DrawTextW for wide-character strings
        DeleteObject(hFont);
        ReleaseDC(console_handle, device_context);
    }

    Pong() {
        win = true; // Add this line to initialize win variable
        GetWindowDimensions(width, height);
        const wchar_t Title[] = L"PONG GAME"; // Use wide-character string literal
        SetWindowTitle(Title);

        const wchar_t text[] = L"Press 'p' for Player vs Player"; // Use wide-character string literal
        const wchar_t text2[] = L"Press 'c' for Player vs Computer"; // Use wide-character string literal

        // Use wide-character strings for text
        myDrawTextWithFont(270, 265, 60, Title, RGB(255, 255, 0), RGB(255, 0, 0));
        myDrawTextWithFont(320, 360, 40, text, RGB(255, 255, 0), RGB(255, 0, 0));
        myDrawTextWithFont(320, 405, 40, text2, RGB(255, 255, 0), RGB(255, 0, 0));

        while (flag) {
            if (_kbhit()) {
                char key = _getch();
                if (key == 'p') {
                    quit = false;
                    flag = false;
                    computer = false; // Add this line
                }
                else if (key == 'c') {
                    computer = true;
                    flag = false;
                }
            }
        }

        up1 = 'w'; up2 = 'i';
        down1 = 's'; down2 = 'k';
        score1 = score2 = 0;
        ball = new Ball(width / 2, height / 2);
        player1 = new Paddle(40, height / 2);
        player2 = new Paddle(width - 40, height / 2);
        player = new Paddle(40, height / 2);
        computer1 = new Computer(width - 40, height / 2);

    }
    ~Pong() {
        delete ball, player1, player2, computer1, player;
    }
    void ScoreUp(Paddle* player) {
        if (player == player1) {
            score1++;
        }
        else if (player == player2) {
            score2++;
        }

        ball->Reset();
        player1->Reset();
        player2->Reset();
        computer1->Reset();


        if (score1 == 10) {
            win = false; // Player 2 wins
            quit = true;
            computer = true;
        }
        else if (score2 == 10) {
            win = true; // Player 1 wins
            quit = true;
            computer = true;
        }
    }
    void UpdateHighScore1(int scr)
    {
        ifstream fin("player1vsplayer2_highscore.txt");
        int s[1] = { 0 };
        for (int i = 0; i < 1; i++)
            fin >> s[i];
        int ind = -1;
        fin.close();
        for (int i = 0; i < 1; i++)
        {
            if (scr > s[i])
            {
                ind = i;
                break;
            }
        }
        if (ind != -1)
        {
            for (int i = 0; i > ind; i--)
            {
                s[i] = s[i - 1];
            }
            s[ind] = scr;
            ofstream fout("player1vsplayer2_highscore.txt");
            for (int i = 0; i < 1; i++)
            {
                fout << s[i] << endl;

            }
        }
    }
    void Display() {
        system("cls");

        int ballx = ball->getX();
        int bally = ball->getY();
        int player1x = player1->getX();
        int player2x = player2->getX();
        int player1y = player1->getY();
        int player2y = player2->getY();

        const wchar_t Title[] = L"GamePlay";
        SetWindowTitle(Title);
        myDrawTextWithFont(width / 2, 0, 50, Title, RGB(255, 0, 0), RGB(255, 255, 0));
        const wchar_t quit[] = L"Enter 'q' for quit. ";
        myLine(0, 50, 0, height, RGB(255, 255, 0));
        myLine(0, 50, width, 50, RGB(255, 255, 0));
        myLine(442, 480, 442, 50, RGB(255, 255, 0));
        myLine(0, height, width, height, RGB(255, 255, 0));
        myLine(width, 50, width, height, RGB(255, 255, 0));

        myRect(player1x, player1y, player1x + 10, player1y + 60, RGB(255, 255, 255), RGB(255, 255, 255));
        myRect(player2x, player2y, player2x - 10, player2y + 60, RGB(255, 255, 255), RGB(255, 255, 255));

        myEllipse(ballx, bally, ballx + 20, bally + 20, RGB(255, 0, 0), RGB(255, 0, 0));

        ofstream inscore1("player1_score.txt");
        inscore1 << score1;
        inscore1.close();
        ofstream inscore2("player2_score.txt");
        inscore2 << score2;
        inscore2.close();

        ifstream outscore1("player1_score.txt");
        outscore1 >> s1;
        outscore1.close();
        ifstream outscore2("player2_score.txt");
        outscore2 >> s2;
        outscore2.close();
        if (score1 > score2) {
            UpdateHighScore1(score1);
        }
        else {
            UpdateHighScore1(score2);
        }
        const wchar_t* score1Str = reinterpret_cast<const wchar_t*>(s1); // Convert char array to wide-character string
        const wchar_t* score2Str = reinterpret_cast<const wchar_t*>(s2); // Convert char array to wide-character string
        myDrawTextWithFont(40, 0, 40, score2Str, RGB(255, 0, 0), RGB(12, 12, 12));
        myDrawTextWithFont(width - 40, 0, 40, score1Str, RGB(255, 0, 0), RGB(12, 12, 12));
        myDrawTextWithFont(10, 500, 40, quit, RGB(255, 0, 0), RGB(12, 12, 12));
        scro1 = score1;
        scro2 = score2;
    }

    void UpdateHighScore(int scr)
    {
        ifstream fin("computer_highscore.txt");
        int s[1] = { 0 };
        for (int i = 0; i < 1; i++)
            fin >> s[i];
        int ind = -1;
        fin.close();
        for (int i = 0; i < 1; i++)
        {
            if (scr > s[i])
            {
                ind = i;
                break;
            }
        }
        if (ind != -1)
        {
            for (int i = 0; i > ind; i--)
            {
                s[i] = s[i - 1];
            }
            s[ind] = scr;
            ofstream fout("computer_highscore.txt");
            for (int i = 0; i < 1; i++)
            {
                fout << s[i] << endl;

            }
        }
    }

    void vsDisplay() {
        system("cls");

        int ballx = ball->getX();
        int bally = ball->getY();
        int playerx = player->getX();
        int computerx = computer1->getX();
        int playery = player->getY();
        int computery = computer1->getY();

        const wchar_t Title[] = L"GamePlay";
        SetWindowTitle(Title);
        myDrawTextWithFont(width / 2, 0, 50, Title, RGB(255, 0, 0), RGB(255, 255, 0));
        const wchar_t quit[] = L"Enter 'q' for quit. ";
        myLine(0, 50, 0, height, RGB(255, 255, 0));
        myLine(0, 50, width, 50, RGB(255, 255, 0));
        myLine(442, 480, 442, 50, RGB(255, 255, 0));
        myLine(0, height, width, height, RGB(255, 255, 0));
        myLine(width, 50, width, height, RGB(255, 255, 0));

        myRect(playerx, playery, playerx + 10, playery + 60, RGB(255, 255, 255), RGB(255, 255, 255));
        myRect(computerx, computery, computerx - 10, computery + 60, RGB(255, 255, 255), RGB(255, 255, 255));

        myEllipse(ballx, bally, ballx + 20, bally + 20, RGB(255, 0, 0), RGB(255, 0, 0));

        ofstream inscore1("computer_score.txt");
        inscore1 << score1;
        inscore1.close();
        ofstream inscore2("player_score.txt");
        inscore2 << score2;
        inscore2.close();

        ifstream outscore1("computer_score.txt");
        outscore1 >> s1;
        outscore1.close();
        ifstream outscore2("player_score.txt");
        outscore2 >> s2;
        outscore2.close();
        if (score1 > score2) {
            UpdateHighScore(score1);
        }
        else {
            UpdateHighScore(score2);
        }

        const wchar_t* score1Str = reinterpret_cast<const wchar_t*>(s1); // Convert char array to wide-character string
        const wchar_t* score2Str = reinterpret_cast<const wchar_t*>(s2); // Convert char array to wide-character string
        myDrawTextWithFont(40, 0, 40, score2Str, RGB(255, 0, 0), RGB(255, 255, 0));
        myDrawTextWithFont(width - 40, 0, 40, score1Str, RGB(255, 0, 0), RGB(255, 255, 0));
        myDrawTextWithFont(10, 500, 40, quit, RGB(255, 0, 0), RGB(255, 255, 0));
        sc1 = score1;
        sc2 = score2;
    }

    void Movement() {
        ball->Move();

        if (_kbhit()) {
            char key1 = _getch();
            if (key1 == up1)    // lower case 'w'
                player1->moveUp();
            else if (key1 == down1)  // lower case 's'
                player1->moveDown();
            if (key1 == up2)    // lower case 'i'
                player2->moveUp();
            else if (key1 == down2)   // lower case 'k'
                player2->moveDown();
            if (ball->getDirection() == STOP)
                ball->randomDirection();
            if (key1 == 'q')
                quit = true;
        }

    }
    void vsMovement() {
        ball->Move();
        computer1->Move();

        if (_kbhit()) {
            char key1 = _getch();
            if (key1 == up1)    // lower case 'w'
                player->moveUp();
            else if (key1 == down1)   // lower case 's'
                player->moveDown();

            if (ball->getDirection() == STOP)
                ball->randomDirection();
            if (computer1->getDirection() == STOP)
                computer1->randomDirection();

            if (key1 == 'q')
                computer = true;
        }

    }
    void Working() {
        int ballx = ball->getX();
        int bally = ball->getY();
        int player1x = player1->getX();
        int player2x = player2->getX();
        int player1y = player1->getY();
        int player2y = player2->getY();

        // Left paddle collision
        if (ballx <= player1x + 10 && ballx + 20 >= player1x && bally + 20 >= player1y && bally <= player1y + 60) {
            ball->changeDirection((eDir)((rand() % 3) + 4)); // Randomize the direction after collision
        }

        // Right paddle collision
        if (ballx + 20 >= player2x - 10 && ballx <= player2x && bally + 20 >= player2y && bally <= player2y + 60) {
            ball->changeDirection((eDir)((rand() % 3) + 1)); // Randomize the direction after collision
        }

        // Bottom wall collision
        if (bally >= height - 10) {
            ball->changeDirection(ball->getDirection() == DOWNRIGHT ? UPRIGHT : UPLEFT);
        }

        // Top wall collision
        if (bally <= 50) {
            ball->changeDirection(ball->getDirection() == UPRIGHT ? DOWNRIGHT : DOWNLEFT);
        }

        // Left wall (player 1 misses)
        if (ballx <= 0) {
            ScoreUp(player2);
        }

        // Right wall (player 2 misses)
        if (ballx >= width) {
            ScoreUp(player1);
        }
    }

    void vsWorking() {
        int ballx = ball->getX();
        int bally = ball->getY();
        int playerx = player->getX();
        int computerx = computer1->getX();
        int playery = player->getY();
        int computery = computer1->getY();

        // Left paddle collision
        if (ballx <= playerx + 10 && ballx + 20 >= playerx && bally + 20 >= playery && bally <= playery + 60) {
            ball->changeDirection((eDir)((rand() % 3) + 4)); // Randomize the direction after collision
        }

        // Computer paddle collision
        if (ballx + 20 >= computerx - 10 && ballx <= computerx && bally + 20 >= computery && bally <= computery + 60) {
            ball->changeDirection((eDir)((rand() % 3) + 1)); // Randomize the direction after collision
        }

        if (computery + 60 >= height)
            computer1->changeDirection(computer1->getDirection() == DOWN ? UP : UP);
        if (computery <= 50)
            computer1->changeDirection(computer1->getDirection() == UP ? DOWN : DOWN);

        // Bottom wall collision
        if (bally >= height - 10) {
            ball->changeDirection(ball->getDirection() == DOWNRIGHT ? UPRIGHT : UPLEFT);
        }

        // Top wall collision
        if (bally <= 50) {
            ball->changeDirection(ball->getDirection() == UPRIGHT ? DOWNRIGHT : DOWNLEFT);
        }

        // Left wall (player misses)
        if (ballx <= 0) {
            ScoreUp(player2);
        }

        // Right wall (computer misses)
        if (ballx >= width) {
            ScoreUp(player1);
        }
    }

    void Run() {
        while (!quit) {
            if (!computer) {
                Display();
                Movement();
                Working();
                if (win && score1 == 10) {
                    const wchar_t p2win[] = L"Player 2 Wins !!";
                    myDrawTextWithFont(width / 2, height - 50, 50, p2win, RGB(255, 0, 0), RGB(12, 12, 12));
                    const wchar_t end[] = L"Game Ends";
                    myDrawTextWithFont(width / 2, height / 2, 50, end, RGB(255, 255, 0), RGB(255, 0, 0));
                    break; // Exit the loop when the game ends
                }
                else if (!win && score2 == 10) {
                    const wchar_t p1win[] = L"Player 1 Wins !!";
                    myDrawTextWithFont(width / 2, height - 50, 50, p1win, RGB(255, 0, 0), RGB(12, 12, 12));
                    const wchar_t end[] = L"Game Ends";
                    myDrawTextWithFont(width / 2, height / 2, 50, end, RGB(255, 255, 0), RGB(255, 0, 0));
                    break; // Exit the loop when the game ends
                }
            }
            else {
                vsDisplay();
                vsMovement();
                vsWorking();
                if (win && score1 == 10) {
                    const wchar_t p2win[] = L"You Lose :";
                    myDrawTextWithFont(width / 2, height - 50, 50, p2win, RGB(255, 0, 0), RGB(12, 12, 12));
                    const wchar_t end[] = L"Game Ends";
                    myDrawTextWithFont(width / 2, height / 2, 50, end, RGB(255, 255, 0), RGB(255, 0, 0));
                    break; // Exit the loop when the game ends
                }
                else if (!win && score2 == 10) {
                    const wchar_t p1win[] = L"You Win !!";
                    myDrawTextWithFont(width / 2, height - 50, 50, p1win, RGB(0, 255, 0), RGB(12, 12, 12));
                    const wchar_t end[] = L"Game Ends";
                    myDrawTextWithFont(width / 2, height / 2, 50, end, RGB(255, 255, 0), RGB(255, 0, 0));
                    break; // Exit the loop when the game ends
                }
            }

            if (quit) {
                score1 = score2 = 0;
                break; // Exit the loop if quit is true
            }
        }
    }

};
int main() 
{
    Pong game;
    system("color 64");
    game.Run();
    return 0;
}