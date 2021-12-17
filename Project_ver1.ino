#include <LiquidCrystal.h>
#include <TimerOne.h>
LiquidCrystal lcd(7,6,5,4,3,2);

volatile int ledNumber = 0;
volatile int badHits = 0;
volatile int timeAtBlink = 0;
volatile int score = 0;
volatile int gameOver = 0;
volatile int finalScore = -1;
volatile int printedResult = 0;
int result[2000];
int counter;

void setup()
{
    lcd.begin(16,2);
    counter = 0;
    pinMode(18 ,INPUT);
    pinMode(19 ,INPUT);
    pinMode(20 ,INPUT);
    pinMode(21 ,INPUT);
    digitalWrite(18, HIGH);
    digitalWrite(19, HIGH);
    digitalWrite(20, HIGH);
    digitalWrite(21, HIGH);
    attachInterrupt(digitalPinToInterrupt(20), functieLed1, FALLING);
    attachInterrupt(digitalPinToInterrupt(21), functieLed2, FALLING);
    attachInterrupt(digitalPinToInterrupt(17), functieLed3, FALLING);
    attachInterrupt(digitalPinToInterrupt(16), restartJoc, FALLING);
    Timer1.initialize(1000000);
    Timer1.attachInterrupt(displayLed);
    printLCDInitialStatus();
    Serial.begin(115200);
    Serial1.begin(115200);
    Serial.println("The game has started!");
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
    sendData("AT+RST\r\n", 2000, false);
    sendData("AT+CWMODE=2\r\n", 1000, false);
    sendData("AT+CIFSR\r\n", 1000, true); 
    sendData("AT+CWSAP?\r\n", 2000, true);   
    sendData("AT+CIPMUX=1\r\n", 1000, false);
    sendData("AT+CIPSERVER=1,80\r\n", 1000, false);
}

void printLCDInitialStatus()
{
    lcd.setCursor(0,0);
    lcd.print("Score:");
    lcd.setCursor(7,1);
    lcd.print("Bad hit:");
    lcd.setCursor(0, 1);
    lcd.print("led:");
}

void loop()
{
    if(badHits >= 3)
    {
       Timer1.stop();
       gameOver = 1;
       if(finalScore == -1)
       {
           finalScore = score;  
       }
       if(printedResult == 0) 
       {
         Serial.println("The game is over!");
         Serial.print("The score: ");
         Serial.println(finalScore);
         result[++counter] = finalScore;
         printedResult = 1;
         if (Serial1.available()) 
         {
           if (Serial1.find("+IPD,")) 
           {
             int maxim = -1;
             for(int i = 1; i <= counter; i++)
             {
               if(result[i] > maxim)
               {
                maxim = result[i];
               }
             }
             delay(500);
             int connectionId = Serial1.read() - 48; 
             String webpage = "<h1>3 Buttons Game</h1>";
             String cipSend = "AT+CIPSEND=";
             cipSend += connectionId;
             cipSend += ",";
             if(maxim != -1)
             {
              webpage += "<p>Best result: ";
              webpage += maxim;
              webpage += "</p>";
              webpage += "\n\n<p>Last result: ";
              for(int i = counter; i >= 1; i--)
              {
                webpage += "<p>";
                webpage += result[i];
                webpage += "</p>";
              }
             }
             cipSend += webpage.length();
             cipSend += "\r\n";
             sendData(cipSend, 100, true);
             sendData(webpage, 150, true);
            
             String closeCommand = "AT+CIPCLOSE=";
             closeCommand += connectionId;
             closeCommand += "\r\n";
             sendData(closeCommand, 300, true);
           }
         }
       }
       lcd.clear();
       lcd.setCursor(0, 0);
       lcd.print("Game over!");
       lcd.setCursor(0, 1);
       lcd.print("Score: ");
       lcd.setCursor(7, 1);
       lcd.print(finalScore);
       delay(1000);
    }
}

void functieLed1()
{
    Serial.println();
    int badHit = 0;
    if(gameOver == 0)
    {
      int timeAtHit = millis();
      Serial.print("Time at hit: ");
      Serial.print(timeAtHit);
      Serial.print(", time at blink: ");
      Serial.print(timeAtBlink);
      Serial.println();
      Serial.println("The button 1 was pressed!");
      if((timeAtHit - timeAtBlink) / 1000 > 1)
      {
        badHit = 1;
        badHits++;
      }
      else
      {
        if(ledNumber != 1)
        {
          badHit = 1;
          badHits++;
        }
        else
        {
          score++;
        }
      }
    }
    if(badHit)
    {
      Serial.print("That was a bad hit: \nThe active led: ");
      Serial.print(ledNumber);
      Serial.print(", Button pressed: ");
      Serial.println("1");
    }
    else
    {
      Serial.print("That was a good hit: \nThe active led: ");
      Serial.print(ledNumber);
      Serial.print(", Button pressed: ");
      Serial.println("1");
    }
    Serial.println();
}

void functieLed2()
{
    Serial.println();
    int badHit = 0;
    if(gameOver == 0)
    {
      int timeAtHit = millis();
      Serial.print("Time at hit: ");
      Serial.print(timeAtHit);
      Serial.print(", time at blink: ");
      Serial.print(timeAtBlink);
      Serial.println();
      Serial.println("The button 2 was pressed!");
      if((timeAtHit - timeAtBlink) / 1000 > 1)
      {
        badHit = 1;
        badHits++;
      }
      else
      {
        if(ledNumber != 2)
        {
          badHit = 1;
          badHits++;
        }
        else
        {
          score++;
        }
      }
    }
    if(badHit)
    {
      Serial.print("That was a bad hit: \nThe active led: ");
      Serial.print(ledNumber);
      Serial.print(", Button pressed: ");
      Serial.println("2");
    }
    else
    {
      Serial.print("That was a good hit: \nThe active led: ");
      Serial.print(ledNumber);
      Serial.print(", Button pressed: ");
      Serial.println("2");
    }
    Serial.println();
}

void functieLed3()
{
    Serial.println();
    int badHit = 0;
    if(gameOver == 0)
    {
      int timeAtHit = millis();      
      Serial.print("Time at hit: ");
      Serial.print(timeAtHit);
      Serial.print(", time at blink: ");
      Serial.print(timeAtBlink);
      Serial.println();
      Serial.println("The button 3 was pressed!");
      if((timeAtHit - timeAtBlink) / 1000 > 1)
      {
        badHit = 1;
        badHits++;
      }
      else
      {
        if(ledNumber != 3)
        {
          badHit = 1;
          badHits++;
        }
        else
        {
          score++;
        }
      }
    }
    if(badHit)
    {
      Serial.print("That was a bad hit: \nThe active led: ");
      Serial.print(ledNumber);
      Serial.print(", Button pressed: ");
      Serial.println("3");
    }
    else
    {
      Serial.print("That was a good hit: \nThe active led: ");
      Serial.print(ledNumber);
      Serial.print(", Button pressed: ");
      Serial.println("3");
    }
    Serial.println();
}

void restartJoc()
{
    Serial.print("\n\n\n");
    Serial.println("The game was restarted");
    Timer1.start();
    lcd.clear();
    badHits = 0;
    score = 0;
    gameOver = 0;
    finalScore = -1;
    printedResult = 0;
    printLCDInitialStatus();
}


void displayLed()
{
    ledNumber = random(3) + 1;
    Serial.print("The led ");
    Serial.print(ledNumber);
    Serial.println(" was activated");
    lcd.setCursor(4, 1);
    lcd.print(ledNumber);
    lcd.setCursor(15, 1);
    lcd.print(badHits);
    lcd.setCursor(6, 0);
    lcd.print(score);
    timeAtBlink = millis();
}

String sendData(String command, const int timeout, boolean debug)
{
   String response = "";
   Serial1.print(command); 
   long int time = millis();
   while ((time + timeout) > millis()) 
   {
     while (Serial1.available()) {
       char c = Serial1.read();
       response += c;
     }
   }
   if(debug)
   {
    Serial1.print(response);
   }
   return response;
}
