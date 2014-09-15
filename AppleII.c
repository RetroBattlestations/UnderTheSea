/* 100 BASIC Week 3: Under the Sea
   110 REM http://reddit.com/r/RetroBattlestations
   120 REM Original program written in BASIC by FozzTexx
   130 converted to C by buffering
   140 http://redd.it/2getg7
 */

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#define MAX_BUBBLES 5
#define MAX_FOOD 8
#define MAX_ENEMIES 7

#if defined(__APPLE2__) || defined(__APPLE2ENH__)
#define SPEAKER __asm__("sta $C030")
#else
#define SPEAKER ((void)0)
#endif

typedef unsigned char Byte;

typedef struct {
    Byte x;
    Byte y;
} Pos;

typedef struct {
    char* data;
    char* data2;
    char* eraseData;
    char* eraseData2;
    Byte width;
    Byte height;
} Sprite;

typedef struct {
    Pos pos;
    short air;
    short points;
    Sprite sprite;
    Sprite deadSprite;
    Byte level;
} Turtle;

typedef struct {
    Pos pos;
    Sprite* sprite;
    Byte type;
    Byte hit;
    Byte ticksX;
    Byte ticksY;
    char velX;
    char velY;
    short hitAmount;
} Enemy;

typedef struct {
    Pos pos;
    char type;
    Sprite sprite;
    Byte ticks;
} Bubble;

typedef struct {
    Pos pos;
    Sprite sprite;
} Food;

void setup();
void game();
void bubblesSetup();
void foodSetup();
void enemiesSetup();
void bubblesRun();
void foodRun();
void enemiesRun();
void eraseTurtle();
void drawSprite(Sprite* sprite, Pos* pos);
void eraseSprite(Sprite* sprite, Pos* pos);
void moveTurtle(char direction);
char readKeyboard();
void resetAir();
void udpateAir(short amount);
Byte hitTest(Byte x, Byte y);
void incrementScore(short amount);
void incrementLevel(char amount);
void updateAir(short amount);
void drawAir();
void drawData(char* data, Byte x, Byte y);
void wavesSetup();
void sound(short len, short delay);

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

const Byte surfacePos = 4;
const Byte maxAir = 10;
Byte rows;
Byte cols;

short highScore = 0;
short highLevel = 0;

const Byte airTicks = 80;
const Byte turtleTicks = 5;
Byte ticks = 0;
Byte maxBubbles;
Byte maxEnemies;

Turtle turtle;

Bubble bubbles[MAX_BUBBLES];
Byte numBubbles = 0;

Food food[MAX_FOOD];
Byte numFood = 0;

Sprite enemySprites[3];
Enemy enemies[MAX_ENEMIES];
Byte numEnemies = 0;

char* waveStr = "-_.";

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

void initSprite(Sprite* sprite)
{
    sprite->eraseData = " ";
    sprite->data = " ";
    sprite->width = 1;
    sprite->height = 1;
}

void setup()
{
    Byte i;
    
    clrscr();
    srand(0);
    screensize(&cols, &rows);
    turtle.pos.y = 18;
    turtle.points = 0;
    turtle.sprite.data      = " _-%#%#}o";
    turtle.sprite.data2     = "       )";
    turtle.sprite.eraseData = "         ";
    turtle.sprite.eraseData2 = "        ";
    turtle.sprite.width = 9;
    turtle.sprite.height = 2;
    turtle.deadSprite.data      = "  -OOOO}x";
    turtle.deadSprite.eraseData = "         ";
    turtle.deadSprite.width = 9;
    turtle.deadSprite.height = 1;
    turtle.level = 0;
    
    enemySprites[0].data = "^;;^";
    enemySprites[0].eraseData = "    ";
    enemySprites[0].width = 4;
    enemySprites[0].height = 1;
    enemySprites[1].data = "<><";
    enemySprites[1].eraseData = "    ";
    enemySprites[1].width = 3;
    enemySprites[1].height = 1;
    enemySprites[2].data = "$";
    enemySprites[2].eraseData = "    ";
    enemySprites[2].width = 1;
    enemySprites[2].height = 1;
    
    numBubbles = 0;
    for (i = 0; i < MAX_BUBBLES; ++i)
    {
        bubbles[i].pos.x = 0;
        bubbles[i].pos.y = 0;
        initSprite(&bubbles[i].sprite);
    }
    
    numFood = 0;
    for (i = 0; i < MAX_FOOD; ++i)
    {
        food[i].pos.x = 0;
        food[i].pos.y = 0;
        initSprite(&food[i].sprite);
    }
    
    numEnemies = 0;
    
    resetAir();
    incrementScore(0);
    incrementLevel(0);
}

void game()
{
    Byte maxPos = cols - turtle.sprite.width;
    char keyChar;
   
    sound(100, 2);
    sound(200, 1);
    sound(400, 0);
    sound(100, 2);
    sound(200, 1);
    sound(400, 0);
    sound(100, 2);
    sound(200, 1);
    sound(300, 0);
    sound(200, 1);
    sound(200, 2);

    maxBubbles = MAX_BUBBLES;
    maxEnemies = MAX_ENEMIES - 2;
    
    while (turtle.air > 0)
    {
        bubblesSetup();
        foodSetup();
        enemiesSetup();
        wavesSetup();

        turtle.pos.x = 1;
        drawSprite(&turtle.sprite, &turtle.pos);
        
        incrementLevel(1);
        switch (turtle.level)
        {
            case 5 :
                --maxBubbles;
                ++maxEnemies;
                break;
            case 10 :
                --maxBubbles;
                ++maxEnemies;
                break;
            case 15 :
                --maxBubbles;
                break;
        }
        
        while (turtle.pos.x < maxPos && turtle.air > 0)
        {
            ++ticks;
            if ((ticks % turtleTicks) == 0)
            {
                SPEAKER;
                ++turtle.pos.x;
                drawSprite(&turtle.sprite, &turtle.pos);
            }
            if ((ticks % airTicks) == 0)
            {
                updateAir(-1);
            }
            
            bubblesRun();
            foodRun();
            enemiesRun();
            
            keyChar = readKeyboard();
            switch (keyChar)
            {
                case 'A':
                case 'a':
                    moveTurtle(-1);
                    break;
                case 'Z':
                case 'z':
                    moveTurtle(1);
                    break;
                case 'Q':
                case 'q':
                    return;
            }
            
//            if (turtle.pos.y <= (surfacePos + 1) && turtle.air < maxAir)
//            {
//                resetAir();
//            }
        }
        
        eraseSprite(&turtle.sprite, &turtle.pos);
    }
}

// 5010
void bubblesRun()
{
    Byte i;
    Byte nextY;
    Byte y;
    Bubble* bubble;
    for (i = 0; i < numBubbles; ++i)
    {
        bubble = &bubbles[i];
        y = bubble->pos.y;
        if (y > surfacePos)
        {
            nextY = y;
            if ((ticks % bubble->ticks) == 0)
            {
                nextY = y - 1;
            }
            if (hitTest(bubble->pos.x, nextY))
            {
                sound(50, 1);
                turtle.air += bubble->type;
                updateAir(0);
                nextY = 0;
            }
            // 5080
            if (nextY != y)
            {
                eraseSprite(&bubble->sprite, &bubble->pos);
                bubble->pos.y = nextY;
                if (nextY > surfacePos)
                {
                    drawSprite(&bubble->sprite, &bubble->pos);
                }
            }
            // 5110
        }
        // 5120
    }
}

// 5140
void bubblesSetup()
{
    Byte i;
    Bubble* bubble;
    
    for (i = 0; i < numBubbles; ++i)
    {
        bubble = &bubbles[i];
        eraseSprite(&bubble->sprite, &bubble->pos);
    }
    
    numBubbles = (rand() % maxBubbles) + 1;
    for (i = 0; i < numBubbles; ++i)
    {
        bubble = &bubbles[i];
        bubble->pos.x = rand() % cols;
        bubble->pos.y = rows - (rand() % 4);
        if (rand() % 2)
        {
            bubble->sprite.data = "O";
            bubble->type = 2;
        }
        else
        {
            bubble->sprite.data = "o";
            bubble->type = 1;
        }
        bubble->ticks = 7 + (rand() % 30);
        drawSprite(&bubble->sprite, &bubble->pos);
    }
}

// 6510
void foodRun()
{
    Byte i;
    Food* f;
    
    for (i = 0; i < numFood; ++i)
    {
        f = &food[i];
        if (hitTest(f->pos.x, f->pos.y))
        {
            sound(60, 2);
            sound(60, 1);
            sound(60, 0);
            incrementScore(10);
            eraseSprite(&f->sprite, &f->pos);
            f->pos.y = rows + cols;
        }
    }
    // 6650
}

// 6660
void foodSetup()
{
    Byte i;
    Food* f;
    
    for (i = 0; i < numFood; ++i)
    {
        f = &food[i];
        eraseSprite(&f->sprite, &f->pos);
    }
    
    numFood = (rand() % MAX_FOOD) + 1;
    for (i = 0; i < numFood; ++i)
    {
        f = &food[i];
        f->pos.x = rand() % cols;
        f->pos.y = (rand() % (rows - surfacePos)) + surfacePos;
        f->sprite.data = (rand() % 2) ? "x" : "y";
        drawSprite(&f->sprite, &f->pos);
    }
    // 6710
}

// 6010
void enemiesRun()
{
    Byte i;
    Byte newX;
    Byte newY;
    Enemy* e;
    
    for (i = 0; i < numEnemies; ++i)
    {
        e = &enemies[i];
        if (e->pos.y > surfacePos)
        {
            newX = e->pos.x;
            newY = e->pos.y;
            if ((ticks % e->ticksX) == 0)
            {
                newX += e->velX;
            }
            if ((ticks % e->ticksY) == 0)
            {
                newY += e->velY;
            }
            
            // 6070
            if (newX != e->pos.x || newY != e->pos.y)
            {
                eraseSprite(e->sprite, &e->pos);
                e->pos.x = newX;
                e->pos.y = newY;
                if (newY > surfacePos)
                {
                    drawSprite(e->sprite, &e->pos);
                }
            }
            if (!e->hit && hitTest(e->pos.x, e->pos.y))
            {
                drawSprite(&turtle.deadSprite, &turtle.pos);
                sound(35, 40);
                incrementScore(-5);
                updateAir(e->hitAmount);
                if (turtle.air == 0)
                {
                    break;
                }
                drawSprite(&turtle.sprite, &turtle.pos);
                e->hit = 1;
            }
            // 6110
        }
        //6120
    }
    // 6130
}

//6140
void enemiesSetup()
{
    Byte i;
    Enemy* e;
    
    for (i = 0; i < numEnemies; ++i)
    {
        e = &enemies[i];
        eraseSprite(e->sprite, &e->pos);
    }
    //6160
    numEnemies = 1 + (rand() % maxEnemies);
    for (i = 0; i < numEnemies; ++i)
    {
        e = &enemies[i];
        e->pos.x = rand() % cols;
        e->pos.y = (rand() % (rows - surfacePos - 1)) + surfacePos + 1;
        e->ticksX = (rand() % 40) + 20;
        e->ticksY = (rand() % 40) + 20;
        e->type = rand() % 3;
        e->sprite = &enemySprites[e->type];
        if (e->sprite->width > 1)
        {
            e->hitAmount = -2;
        }
        else
        {
            e->hitAmount = -1;
        }
        e->hit = 0;
        e->velX = (e->pos.x > (cols >> 1)) ? -1 : 1;
        e->velY = (e->pos.y > (rows >> 1)) ? -1 : 1;
        
        drawSprite(e->sprite, &e->pos);
    }
    //6220
}


// 3010
void moveTurtle(char direction)
{
    Byte minY = surfacePos + 1;
    Byte maxY = rows - turtle.sprite.height;
    
    eraseSprite(&turtle.sprite, &turtle.pos);
    
    turtle.pos.y += direction;
    if (turtle.pos.y < minY)
    {
        turtle.pos.y = minY;
    }
    if (turtle.pos.y > maxY)
    {
        turtle.pos.y = maxY;
    }

    drawSprite(&turtle.sprite, &turtle.pos);
}

// 4010
void updateAir(short amount)
{
    short newAir = turtle.air + amount;
    //printf("amount:%d newAmount:%d", amount, newAir);
    if (newAir < 0)
    {
        newAir = 0;
    }
    if (newAir > maxAir)
    {
        newAir = maxAir;
    }
    turtle.air = newAir;
    drawAir();
}

//4030
void drawAir()
{
    Byte i;

    cputsxy(cols - 7 - maxAir, 2, "Air: |");
    for (i = 0; i < turtle.air; ++i)
    {
        cputc('#');
    }
    if (turtle.air < maxAir)
    {
        for (i = turtle.air; i < maxAir; ++i)
        {
            cputc(' ');
        }
    }
    cputc('|');
}

// 4100
void resetAir()
{
    turtle.air = maxAir;
    drawAir();
}

// 4510
void incrementScore(short amount)
{
    turtle.points += amount;
    gotoxy(7, 2);
    puts("    ");
    gotoxy(1, 2);
    printf("Score:%d", turtle.points);
}

void incrementLevel(char amount)
{
    turtle.level += amount;
    gotoxy(13, 2);
    printf("L:%d", turtle.level);
}

// 5510
Byte hitTest(Byte x, Byte y)
{
    if (x < (turtle.pos.x + turtle.sprite.width - 2))
    {
        return 0;
    }
    if (x > (turtle.pos.x + turtle.sprite.width + 1))
    {
        return 0;
    }
    if (y < (turtle.pos.y - 1))
    {
        return 0;
    }
    if (y > (turtle.pos.y + 1))
    {
        return 0;
    }
    return 1;
}

// 1510
void drawSprite(Sprite* sprite, Pos* pos)
{
    drawData(sprite->data, pos->x, pos->y);
    if (sprite->height == 2)
    {
        drawData(sprite->data2, pos->x, pos->y+1);
    }
}

void eraseSprite(Sprite* sprite, Pos* pos)
{
    drawData(sprite->eraseData, pos->x, pos->y);
    if (sprite->height == 2)
    {
        drawData(sprite->eraseData2, pos->x, pos->y+1);
    }
}

void drawData(char* data, Byte x, Byte y)
{
    if (/*y == 0 ||*/ y > rows || /*x == 0 ||*/ x > cols)
    {
        return;
    }
    gotoxy(x, y);
    cputs(data);
}

char readKeyboard()
{
    if (kbhit())
    {
        return cgetc();
    }
    return 0x00;
}

void wavesSetup()
{
    Byte pos = 0;
    Byte maxPos = cols-3;
    while (pos < maxPos)
    {
        cputsxy(pos, surfacePos, waveStr);
        pos += rand() % 4;
    }
    cputsxy(maxPos, surfacePos, waveStr);
}

Byte gameOver()
{
    Byte leftCol;
    
    //4110
    for (; turtle.pos.y > surfacePos + 1; --turtle.pos.y)
    {
        drawSprite(&turtle.deadSprite, &turtle.pos);
        sound(20,(turtle.pos.y * 2) + 20);
        eraseSprite(&turtle.deadSprite, &turtle.pos);
    }
    
    if (turtle.points > highScore)
    {
        highScore = turtle.points;
    }
    if (turtle.level > highLevel)
    {
        highLevel = turtle.level;
    }
    
    leftCol = (cols/2) - 10;
    cputsxy(leftCol, 10, "********************");
    cputsxy(leftCol, 11, "*                  *");
    cputsxy(leftCol, 12, "*    GAME OVER     *");
    cputsxy(leftCol, 13, "*                  *");
    cputsxy(leftCol, 14, "* High Score:      *");
    cputsxy(leftCol, 15, "*      Level:      *");
    cputsxy(leftCol, 16, "*                  *");
    cputsxy(leftCol, 17, "* Play Again?  Y/N *");
    cputsxy(leftCol, 18, "*                  *");
    cputsxy(leftCol, 19, "********************");
    
    gotoxy(leftCol+13, 14);
    cprintf("%d", highScore);
    gotoxy(leftCol+13, 15);
    cprintf("%d", highLevel);
    
    while (1)
    {
        switch (readKeyboard())
        {
            case 'y' :
            case 'Y' :
                return 1;
                
            case 'n' :
            case 'N' :
                return 0;
        }
    }
    return 0;
}

void sound(short len, short delay)
{
    short ii, jj;
    for (ii = 0; ii < len; ++ii)
    {
        SPEAKER;
        for (jj = 0; jj < delay; ++jj)
        {
        }
    }
}

int main(void)
{
    while (1)
    {
        setup();
        game();
        if (!gameOver())
        {
            break;
        }
    }
    return 0;
}
