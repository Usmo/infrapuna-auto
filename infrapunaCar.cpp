
#include <IRremote.h>       // include the IRremote library
#define RECEIVER_PIN 7      // define the IR receiver pin
IRrecv receiver(RECEIVER_PIN);      // create a receiver object of the IRrecv class
decode_results results;             // create a results object of the decode_results class
unsigned long key_value = 0;        // variable to store the pressed key value

int buttonState = 0;
int forward_pin = 6;                // Määritellään pinnit joilla ohjataan moottoreita
int backward_pin = 5;
int left_pin = 11;
int right_pin = 10;
int led_pin = 8;                    //LEDien vilkuttamiseen toinen pinni.
int flashingLightsState = 0;        //Muuttujalla seurataan vilkkuvien valojen tilaa
int movement = 0;           //muuttujalla seurataan liikkumistilaa.  Positiivinen eteenpäin ja negatiivinen taaksepäin.
int turning = 0;            //muuttujalla seurataan kääntymistilaa.  Positiivinen oikealle ja negatiivinen vasemmalle.

void stopLedFlashing();     //Funktio, jolla vilkutus sammutetaan
void startLedFlashing();    //Funktio, jolla vilkutus käynnistetään
void speedValuesUpdate();   //Funktio, jolla tarkastetaan movement-muuttujan tieto ja päivitetään nopeus sen mukaisesti
void turnValuesUpdate();    //Funktio, jolla tarkastetaan turning-muuttujan tieto ja päivitetään kääntyminen sen mukaisesti


void setup()
{
  
  Serial.begin(9600);       // begin serial communication with a baud rate of 9600
  receiver.enableIRIn();    // enable the receiver
  receiver.blink13(true);   // enable blinking of the built-in LED when an IR signal is received
  //pinMode(2, INPUT);
  pinMode(13, OUTPUT);
  
  pinMode(forward_pin, OUTPUT);     //Pinnien määrittelyt outputeiksi
  pinMode(backward_pin, OUTPUT);
  pinMode(left_pin, OUTPUT);
  pinMode(right_pin, OUTPUT);
  pinMode(led_pin, OUTPUT);
  pinMode(9, OUTPUT);

  movement = 0;             // Arvojen asettaminen nolliksi, jotta auto pysyy paikoillaan alussa.
  turning = 0;
  speedValuesUpdate();
  turnValuesUpdate();

  TCCR1A = 0;               // Timerin bittien reset
  TCCR1B = 0;
}

void loop()
{
  
  /*

Nappien koodit:

YLÖS = 800F041E , 800F841E          (1E69883 , A36FD29F)
ALAS = 800F041F , 800F841F          (E696EE , A26FD10A)
OIKEA = 800F0421 , 800F8421         (8E1449A9 , 79945765)
VASEN = 800F0420 , 800F8420         (C1C13C4C , 634A7668)
OK nappi = 800F0422 , 800F8422      (EC9BC835 , 11BBA79)
PARTYVALO nappi (Windows) = 800F040D , 800F840D (27CFDF92 , 134FED4E)

Koodit vaihtelee vuorotellen, kun nappia painaa. 
Suluissa olevat arvot tulee sen jälkeen, kun oikealle painiketta on kerran painettu. En tiedä miksi.

  */

    if (receiver.decode(&results)) {                    //Jos ir-decoderi saa arvon

        if (results.value == 0X800F041E || results.value == 0X800F841E || results.value == 0X1E69883 || results.value == 0XA36FD29F) {  //Verrataan ir-decoderin tulosta YLÖS nappiin

            if (movement == 3){                          //Jos movement on jo valmiiksi 3, ei tehdä mitään ja tulostetaan serialiin teksti
                Serial.println("SPEED ALREADY MAXED");
            }
            else {                                      // Jos movement ei ole 3, lisätään movement-muuttujaan ykkönen ja päivitetään nopeus.
                movement++;
                speedValuesUpdate();
            }

            Serial.println("UP BUTTON PRESSED");        // Tulostetaan serial-monitoriin tieto painetusta napista.
            delay(100);                                 // Pieni viive, jotta yhdestä painalluksesta ei rekisteröityisi tahattomia lisäpainalluksia.
        }

        else if (results.value == 0X800F041F || results.value == 0X800F841F || results.value == 0XE696EE || results.value == 0XA26FD10A){  //Verrataan ir-decoderin tulosta ALAS nappiin
            
            if (movement == -3){                         // Jos movement on jo valmiiksi -3, ei tehdä mitään ja tulostetaan serialiin teksti
                Serial.println("REVERSE ALREADY MAXED");
            }
            else {                                      // Jos movement ei ole -3, vähennetään muuttujasta ykkönen ja päivitetään nopeus.
                movement--;
                speedValuesUpdate();
            }

            Serial.println("DOWN BUTTON PRESSED");      // Tulostetaan serial-monitoriin tieto painetusta napista.
            delay(100);                                 // Pieni viive, jotta yhdestä painalluksesta ei rekisteröityisi tahattomia lisäpainalluksia.
        }

        else if (results.value == 0X800F0421 || results.value == 0X800F8421 || results.value == 0X8E1449A9 || results.value == 0X79945765){  //Verrataan ir-decoderin tulosta OIKEA nappiin

            if (turning == 2){                           // Jos kääntyminen on jo max-arvossa, ei tehdä muuta kuin tulostus serial monitoriin.
                Serial.println("RIGHT TURNING ALREADY MAXED");
            }
            else {                                      // Muuten lisätään turning-muuttujaan yksi ja päivitetään kääntyminen.
                turning++;
                turnValuesUpdate();
            }

            Serial.println("RIGHT BUTTON PRESSED");     // Tulostetaan serial-monitoriin tieto painetusta napista.
            delay(100);                                 // Pieni viive, jotta yhdestä painalluksesta ei rekisteröityisi tahattomia lisäpainalluksia.
            
        }

        else if (results.value == 0X800F0420 || results.value == 0X800F8420 || results.value == 0XC1C13C4C || results.value == 0X634A7668){  //Verrataan ir-decoderin tulosta VASEN nappiin

            if (turning == -2){                          // Jos kääntyminen on jo max-arvossa, ei tehdä muuta kuin tulostus serial monitoriin.
                Serial.println("LEFT TURNING ALREADY MAXED");
            }
            else {                                      // Muuten vähennetään turning-muuttujasta yksi ja päivitetään nopeus.
                turning--;
                turnValuesUpdate();
            }

            Serial.println("LEFT BUTTON PRESSED");      // Tulostetaan serial-monitoriin tieto painetusta napista.
            delay(100);                                 // Pieni viive, jotta yhdestä painalluksesta ei rekisteröityisi tahattomia lisäpainalluksia.
            
        }

        else if (results.value == 0X800F0422 || results.value == 0X800F8422 || results.value == 0XEC9BC835 || results.value == 0X11BBA79){  //Verrataan ir-decoderin tulosta OK nappiin

            turning = 0;                                // STOP, eli kaikki arvot nolliin ja päivitetään nopeus sekä kääntyminen.
            movement = 0;
            speedValuesUpdate();
            turnValuesUpdate();

            Serial.println("OK BUTTON PRESSED");        // Tulostetaan serial-monitoriin tieto painetusta napista.
            delay(100);                                 // Pieni viive, jotta yhdestä painalluksesta ei rekisteröityisi tahattomia lisäpainalluksia.
            
        }

        else if (results.value == 0X800F040D || results.value == 0X800F840D || results.value == 0X27CFDF92 || results.value == 0X134FED4E){  //Verrataan ir-decoderin tulosta WINDOWS nappiin

            if (flashingLightsState == 0){               // Jos valot eivät ole päällä, käynnistetään valot funktiolla
                startLedFlashing();
                flashingLightsState = 1;
            }
            else{                                       // Jos valojen vilkutus on jo käynnissä, sammutetaan valot funktiolla
                stopLedFlashing();
                flashingLightsState = 0;
            }

            Serial.println("LED-BUTTON PRESSED");       // Tulostetaan serial-monitoriin tieto painetusta napista.
            delay(100);                                 // Pieni viive, jotta yhdestä painalluksesta ei rekisteröityisi tahattomia lisäpainalluksia.
        }

        else {      //Mikäli tulos ei ole mikään määritellyistä, ei tehdä mitään ja tulostetaan serialimonitoriin teksti
            Serial.println("Not recognized");
            Serial.println(results.value, HEX);

               
        }

    key_value = results.value;
    receiver.resume();                                  // IR-vastaanotin palautetaan kuuntelemaan uusia komentoja

    //Serial.println(results.value, HEX);
    //delay(10);
    }
}


  
void stopLedFlashing()         //Timerin rekisterien nollaus, valot lakkaavat vilkkumasta
  {
  TCCR1A = 0;           
  TCCR1B = 0;
  digitalWrite(led_pin, LOW);
  }


void startLedFlashing()        //Timerien rekisterit initialized, valot lähtevät vilkkumaan
  {
  //TCCR1A = 0;
  //TCCR1B = 0;
  bitSet(TCCR1B, CS12);     // 256 prescaler
  OCR1A = 62500; 		    // 16MHz / 256 = 62500
  bitSet(TCCR1A, COM1A0);   // Toggle pin OC1A (9)
  digitalWrite(led_pin, HIGH);
  }

/*
    Ohjaus taisi toimia niin, että ohjattava pinni toimii PWM-pulssisuhteella ja toinen asetetaan HIGH asentoon.
    Käänteinen järjestys pinneille, jos moottorin suuntaa halutaan vaihtaa.

    Tämä tarkoittaa, että analogWrite on molemmissa 255 kun ollaan paikoillaan. 
    Mitä alemmaksi forward_pin lukema laitetaan, sitä kovempaa mennään eteenpäin , JOS backwards-pin on HIGH arvossa 255.
*/

void speedValuesUpdate()       //Funktiossa tarkastetaan movement-muuttujan arvo ja muokataan moottorinohjauspinnien arvot sen mukaisesti.
{
    if (movement == 3){
        analogWrite(forward_pin, 128);
        analogWrite(backward_pin, 255);
    }
    else if (movement == 2){
        analogWrite(forward_pin, 168);
        analogWrite(backward_pin, 255);
    }
    else if (movement == 1){
        analogWrite(forward_pin, 198);
        analogWrite(backward_pin, 255);
    }
    else if (movement == 0){
        analogWrite(forward_pin, 255);
        analogWrite(backward_pin, 255);
    }
    else if (movement == -1){
        analogWrite(forward_pin, 255);
        analogWrite(backward_pin, 198);
    }
    else if (movement == -2){
        analogWrite(forward_pin, 255);
        analogWrite(backward_pin, 168);
    }
    else if (movement == -3){
        analogWrite(forward_pin, 255);
        analogWrite(backward_pin, 128);
    }

}

void turnValuesUpdate()        //Funktiossa tarkastetaan turning-muuttujan arvo ja muokataan moottorinohjauspinnien arvot sen mukaisesti.
{
    if (turning == 2){
        analogWrite(right_pin, 0);
        analogWrite(left_pin, 255);
    }
    else if (turning == 1){
        analogWrite(right_pin, 128);
        analogWrite(left_pin, 255);
    }
    else if (turning == 0){
        analogWrite(right_pin, 255);
        analogWrite(left_pin, 255);
    }
    else if (turning == -1){
        analogWrite(right_pin, 255);
        analogWrite(left_pin, 128);
    }
    else if (turning == -2){
        analogWrite(right_pin, 255);
        analogWrite(left_pin, 0);
    }

}
