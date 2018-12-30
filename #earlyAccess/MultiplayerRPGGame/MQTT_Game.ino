//------------------------------------------------------//
//                   Configuration                      //
//                                                      //
// Your nick                                            //
String myNick = "Kacper1263";
//                                                      //
// Your multiplayer enemy nick                          //
String enemyNick = "Szoox";
//                                                      //
// Server                                               //
#define HOST_NAME "brocker.hivemq.com"
//------------------------------------------------------//



#include <LiquidCrystal.h>

#define btn1 10
#define btn2 11


String enemyTopic = String("MRG/multiplayer/" + enemyNick);

byte menuHome = 1;
byte menuFight = 1;
byte inGame = 0;
byte inFight = 0;
byte gameMode = 0;
byte myMove = 1;
int myDmg = 0;
int enemyDmg = 0;
int enemyCounter = 0;
byte sceneNumber = 0;
byte goNext = 0;
int myHP = 500;
int easyEnemyHP = 100;
byte leftOrRight = 0;
byte crit = 0;

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

byte leftArrow[] = {
  B00010,
  B00110,
  B01110,
  B11110,
  B11110,
  B01110,
  B00110,
  B00010
};

byte rightArrow[] = {
  B01000,
  B01100,
  B01110,
  B01111,
  B01111,
  B01110,
  B01100,
  B01000
};

byte potion[] = {
  B00000,
  B01010,
  B01010,
  B01010,
  B10001,
  B11111,
  B11111,
  B01110
};

byte downArrow[] = {
  B00100,
  B00100,
  B00100,
  B00100,
  B00100,
  B11111,
  B01110,
  B00100
};

void setup()
{
    pinMode(btn1, INPUT_PULLUP);
    pinMode(btn2, INPUT_PULLUP);

    lcd.begin(16, 2);
    lcd.clear();
    lcd.createChar(1, leftArrow);
    lcd.createChar(2, rightArrow);
    lcd.createChar(3, potion);
    lcd.createChar(4, downArrow);

    randomSeed(analogRead(0));

    homeScr();
}

void loop()
{
    
    if (digitalRead(btn1) == LOW && inGame == 0) {
        menuHome++;
        if (menuHome > 2) {
            menuHome = 1;
        }
        delay(200);
    }
    if (digitalRead(btn1) == LOW && inGame == 1) {
        menuFight++;
        if (menuFight > 3) {
            menuFight = 1;
        }
        delay(200);
    }
    if (inGame == 0) {
        homeScr();
    }
    if (gameMode == 1) {
        gameMode1();
    }
    if (gameMode == 2) {
        gameMode2();
    }

}

void homeScr(){
    lcd.setCursor(0, 0);
    lcd.print("Game mode:      ");
    if (menuHome == 1) {
        lcd.setCursor(0, 1);
        lcd.print("1) Single player");
        if (digitalRead(btn2) == LOW) {
            gameMode = 1;
        }
    }
    else if (menuHome == 2) {
        lcd.setCursor(0, 1);
        lcd.print("2) Multi player ");
        if (digitalRead(btn2) == LOW) {
            gameMode = 2;
        }
    }
    delay(20);
}

void newMessage(String incomingTopic, String payload, byte qos, bool retained) {
    if (enemyTopic == incomingTopic) {
        if (String("ON") == payload) {
            digitalWrite(13, HIGH);
        }
        else if (String("OFF") == payload) {
            digitalWrite(13, LOW);
        }
    }
}

void gameMode1() {
    if (inGame == 0) {
        lcd.setCursor(0, 0);
        lcd.print("Single player   ");
        lcd.setCursor(0, 1);
        lcd.print("                ");
        delay(3000);
        lcd.clear();
        inGame = 1;
    }
    if (myHP <= 0) {
        easyEnemyHP = 100;
        myHP = 500;
        lcd.clear();
        lcd.print("Died!");
        lcd.setCursor(0, 1);
        lcd.print("Kiled:");
        lcd.print(enemyCounter);
        lcd.setCursor(15, 1);
        lcd.write(4);

        goNext = 0;
        while (goNext == 0) {
            okGoNext();
        }
        enemyCounter = 0;
        myMove = 1;
        inGame = 0;
        inFight = 0;
        gameMode = 0;
        homeScr();
    }

    if (inFight == 0) {
        changeScene();
    }
    
    if (sceneNumber == 0 || sceneNumber == 1) {
        if (inFight == 0) {
            lcd.clear();
            lcd.print("Walka");
            delay(3000);
        }
        inFight = 1;
        fight();
    }
    else if (sceneNumber == 2) {
        unknownPotionFound();
    }
    else if (sceneNumber == 3) {
        hpPotionFound();
    }
}

void gameMode2() {
   
        
    
}

void changeScene() {
    sceneNumber = random(0, 4);
}

void fight(){
enemyMove:
    if (easyEnemyHP <= 0) {
        easyEnemyHP = 100;
        enemyCounter++;
        lcd.clear();
        lcd.print("Win! Kiled: ");
        lcd.print(enemyCounter);
        inFight = 0;
        myMove = 1;
        delay(4000);
    }
    if (myHP <= 0) {
        easyEnemyHP = 100;
        myHP = 500;
        lcd.clear();
        lcd.print("Died!");
        lcd.setCursor(0, 1);
        lcd.print("Kiled:");
        lcd.print(enemyCounter);
        lcd.setCursor(15, 1);
        lcd.write(4);

        goNext = 0;
        while (goNext == 0) {
            okGoNext();
        }
        enemyCounter = 0;
        myMove = 1;
        inGame = 0;
        inFight = 0;
        gameMode = 0;
        homeScr();
    }
    lcd.setCursor(0, 0);
    lcd.print("Y:");
    lcd.print(myHP);
    lcd.print(" ");
    if (myMove == 1) {
        lcd.write(1);
    }
    else {
        lcd.write(2);
    }
    lcd.print(" E:");
    lcd.print(easyEnemyHP);
    if (menuFight == 1 && myMove == 1) {
        lcd.setCursor(0, 1);
        lcd.print("1) Weak   (80%) ");
        if (digitalRead(btn2) == LOW) {
            weakAttack();
            easyEnemyHP = easyEnemyHP - myDmg;
            lcd.setCursor(0, 1);
            if (crit == 1) {
                lcd.print("CRIT! ");
            }
            lcd.print("-");
            lcd.print(myDmg);
            lcd.print(" HP           ");
            if (crit == 1) {
                delay(2000);
                crit = 0;
            }
            delay(1500);
            lcd.clear();
            myMove = 0;
            goto enemyMove;
        }
    }
    if (menuFight == 2 && myMove == 1) {
        lcd.setCursor(0, 1);
        lcd.print("2) Medium (50%) ");
        if (digitalRead(btn2) == LOW) {
            mediumAttack();
            easyEnemyHP = easyEnemyHP - myDmg;
            lcd.setCursor(0, 1);
            if (crit == 1) {
                lcd.print("CRIT! ");
            }
            lcd.print("-");
            lcd.print(myDmg);
            lcd.print(" HP           ");
            if (crit == 1) {
                delay(2000);
                crit = 0;
            }
            delay(1500);
            lcd.clear();
            myMove = 0;
            goto enemyMove;
        }
    }
    if (menuFight == 3 && myMove == 1) {
        lcd.setCursor(0, 1);
        lcd.print("3) Strong (30%) ");
        if (digitalRead(btn2) == LOW) {
            strongAttack();
            easyEnemyHP = easyEnemyHP - myDmg;
            lcd.setCursor(0, 1);
            if (crit == 1) {
                lcd.print("CRIT! ");
            }
            lcd.print("-");
            lcd.print(myDmg);
            lcd.print(" HP           ");
            if (crit == 1) {
                delay(2000);
                crit = 0;
            }
            delay(1500);
            lcd.clear();
            myMove = 0;
            goto enemyMove;
        }
    }
    if (myMove == 0) {
        lcd.setCursor(0, 1);
        lcd.print("Enemy move      ");
        delay(1500);
        easyEnemyAttack();
        myHP = myHP- enemyDmg;
        lcd.setCursor(0, 1);
        if (crit == 1) {
            lcd.print("CRIT! ");
        }
        lcd.print("-");
        lcd.print(enemyDmg);
        lcd.print(" HP           ");
        if (crit == 1) {
            delay(2000);
            crit = 0;
        }
        delay(1500);
        lcd.clear();
        myMove = 1;
    }
}

void weakAttack() {
    int luck, dmg;
    luck = random(0, 101);
    if (luck <= 80) {
        dmg = random(1, 36);
    }
    else if (luck >= 87 && luck <= 100) {
        dmg = random(100, 151);
        crit = 1;
    }
    else {
        dmg = 0;
    }
    myDmg = dmg;
}
void mediumAttack() {
    int luck, dmg;
    luck = random(0, 101);
    if (luck <= 50) {
        dmg = random(35, 71);
    }
    else if (luck >= 87 && luck <= 100) {
        dmg = random(100, 151);
        crit = 1;
    }
    else {
        dmg = 0;
    }
    myDmg = dmg;
}
void strongAttack() {
    int luck, dmg;
    luck = random(0, 101);
    if (luck <= 30) {
        dmg = random(50, 101);
    }
    else if (luck >= 87 && luck <= 100) {
        dmg = random(150, 201);
        crit = 1;
    }
    else {
        dmg = 0;
    }
    myDmg = dmg;
}
void easyEnemyAttack() {
    int luck, dmg;
    crit = 0;
    luck = random(0, 101);
    if (luck <= 55) {
        dmg = random(1, 31);
    }
    else if (luck >= 76 && luck <= 88) {
        dmg = random(50, 101);
        crit = 1;
    }
    else {
        dmg = 0;
    }
    enemyDmg = dmg;
}


void hpPotionFound() {
    int giveHP = 0;
    giveHP = random(1, 70);

    lcd.clear();
    lcd.print("Znaleziono: ");
    lcd.write(3);
    delay(2000);
    lcd.setCursor(0, 1);
    lcd.print("HP +");        
    lcd.print(giveHP);
    myHP = myHP + giveHP;

    lcd.setCursor(15, 1);
    lcd.write(4);

    goNext = 0;
    while (goNext == 0) {
        okGoNext();
    }
}
void unknownPotionFound() {
    int HP = 0;
    HP = random(1, 70);

    lcd.clear();
    lcd.print("Znaleziono: ");
    lcd.write(3);
    delay(2000);
    lcd.setCursor(0, 1);
    lcd.print("Nieznana!      ");
    lcd.write(4);

    goNext = 0;
    while (goNext == 0) {
        okGoNext();
    }

    lcd.clear();
    lcd.print("Lewy  - Pij ");
    lcd.setCursor(0, 1);
    lcd.print("Prawy - Nie pij ");
    delay(500);

    goNext = 0;
    while (goNext == 0) {
        okGoNextLeftOrRight();
    }

    if (leftOrRight == 0) {
        isPotionGood();
    }
}

void isPotionGood() {
    byte isGood = 0;
    byte goodOrBad = 0;

    isGood = random(0, 2);
    goodOrBad = random(40, 151);

    if (isGood == 1) {
        lcd.clear();
        lcd.print("HP +");
        lcd.print(goodOrBad);
        myHP = myHP + goodOrBad;
        delay(3000);
    }
    else if (isGood == 0) {
        lcd.clear();
        lcd.print("HP -");
        lcd.print(goodOrBad);
        myHP = myHP - goodOrBad;
        delay(3000);
    }
}

void okGoNext() {
    if (digitalRead(btn1) == LOW) {
        goNext = 1;
    }
}
void okGoNextLeftOrRight() {
    if (digitalRead(btn1) == LOW) {
        leftOrRight = 0;
        goNext = 1;
        delay(500);
    }
    else if (digitalRead(btn2) == LOW) {
        leftOrRight = 1;
        goNext = 1;
        delay(500);
    }
}

