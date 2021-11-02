
#include <SPI.h>

#include <SD.h>

#include <DS3231.h>
#include<stdio.h>
#include<string.h>
#include <ds1307.h>
#include <Wire.h>
#include <SoftwareSerial.h>
File myFile;
ds1307 DS1307;
boolean envoie=false;
RTCDateTime dt;
int  dateAjour[6];
int debut=0;
boolean volatile heures = false;
boolean volatile ab = false;
DS3231 clock;
uint8_t buffer[7];
bool save = false;
#include <Key.h>
#include <Keypad.h>
//RTC_DS1307 rtc;
#include <LiquidCrystal.h>
//volatile RTC_DS1307 rtc;

LiquidCrystal lcd (23, 22, 24, 25, 26, 27);
const int chipSelect = 53;
char m[16];
char z;
char don[100];
String tay, g;
int mesDate[5];
volatile boolean com = false;
char mobile[16];
int nbr1, a;
int nbr2;
char val;
int valF;
char d[16];
char h[16];
float valfP;
int i = 0;
String M = "", T = "", P = "", V = "";
char lettrenombre [4];
String lettrenombre2 [3];
char massChar[4] = "";
int compteur = 0;
int io;
int clickS = 1;
const int interruption = 21;
const int Binit = 37;
const int LED = 38;                                                       //, apres .
const char key[4][3] = {{'1', '2', '3'}, {'4', '5', '6'}, {'7', '8', '9'}, {'.', '0', ','}};
byte p[4] = {28, 29, 30, 31};
byte pinc[4] = {32, 33, 34, 35};
boolean etatBinit;
int nbrfinal;
char ao[16];
char br[16];
char tp[16];
char pr[16];
String dateFichier;
char dateFC[16];
int temperature;
double pression;
int masse;
boolean fasy = false;
const int btS = 18;
const int btE = 19; //bouttons reset
String enT;
const int btT = 40;
const int btP = 41;//const int btP = 41;
const int btM = 3;//const int btM = 42;
int massChar2;
char volatile stat = HIGH;
volatile byte state = HIGH;
char avantFaran[30];
String vol;
String  valeur;
String faran=" ";
char tex[4][4] = {
  {'1', '2', '3', ' '},
  {'4', '5', '6', ' '},
  {'7', '8', '9', ' '},
  {' ', '0', '.', ' '}
};
String donnes [30][5];
boolean ok = false;
boolean volatile f = true;
boolean volatile etat = true;
char *commandes;
Keypad kep = Keypad(makeKeymap(tex), p, pinc, 4, 4);
int valold;
bool etatP = digitalRead(btP);
bool etatM = digitalRead(btM);
bool etatT = digitalRead(btT);
String lettre[2];
String enT2;
boolean volatile saisirData = false;
void setup() {
  
  Serial.begin(9600);
  Serial2.begin(115200);
  pinMode(Binit, INPUT);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
  lcd.begin(20, 4);

  pinMode(btP, INPUT_PULLUP);
  pinMode(btT, INPUT_PULLUP);
  pinMode(btM, INPUT_PULLUP);
  pinMode(btE, INPUT_PULLUP);
  pinMode(btS, INPUT_PULLUP);
  //   sw.begin(9600);
  char  message [10];
  ///affichagePermanentDonnes(65, 98, 30); //active

  //affichageDynamique("Entrer le nombre de briquet");
 affichagePermanentDonnes(returnM(), returnT(), returnP()); //obligatoire
     attachInterrupt(digitalPinToInterrupt(btE), rst, LOW);
    attachInterrupt(digitalPinToInterrupt(btS), sauverDonnes, LOW);
    attachInterrupt(digitalPinToInterrupt(btM), avantSaisitD, FALLING);

  //clock.setDateTime(Serial2.read());
  //attachInterrupt(digitalPinToInterrupt(btSaisit), appelSaisit, LOW);
 // attachInterrupt(digitalPinToInterrupt(btS), afficherDonnerF, CHANGE);
  //DateInit();
  /* 
    clock.setDateTime(__DATE__, __TIME__);
    initAlarm2();
    //creationFichier();*/
    clock.begin();
 while(!Serial2.available()){
  decoupString();
 }
 

    

}
void rst() {
  valfP = 0.0;
  nbrM(0);
  valfP = 0.0;
  nbrT(0);

  lcd.clear();
  valfP = 0.0;
  etat = true;
  f = false;
  heures = true;
  delay(20);
  saisitD(true);


}

void avantSaisitD() {
  heures = true;
  saisirData = true;

}

void D4() {

  clock.begin();
  //clock.setDateTime(__DATE__, __TIME__);
  clock.setDateTime(2021, 10, 11, 11, 8, 1);
}
void testKey() {
  char customKey = kep.getKey();

  if (customKey) {
    Serial.println(customKey);
  }

}

void testLcd() {
  lcd.begin(20, 4);
  lcd.setCursor(5, 0);
  lcd.print("premier ligne");
  lcd.setCursor(0, 1);
  lcd.print("faraoa ligne");
  lcd.setCursor(0, 2);
  lcd.print("troisieme ligne.........");
  lcd.setCursor(0, 3);
  lcd.print("quatrieme ligne.........");

}
void initAlarm2() {
  clock.armAlarm1(false);
  clock.armAlarm2(false);
  clock.clearAlarm1();
  clock.clearAlarm2();
  delay(1);
  clock.setAlarm2(0, 0, 1, 0, DS3231_MATCH_M);
  /*clock.setAlarm1(0, 11, 56, 00, DS3231_MATCH_H_M_S); //every 11 hour
    clock.setAlarm1(0, 23, 56,0, DS3231_MATCH_H_M_S); //every**/
  clock.setAlarm1(0, 0, 0, 20, DS3231_MATCH_S);
}
void D4get() {
  dt = clock.getDateTime();
  sprintf(h, "H:%d:%d:%d", dt.hour, dt.minute, dt.second);
}




char lecture() {
  char l = kep.getKey();

  /*affScrol("Bienvenue");*/
  lcd.noAutoscroll();
  lcd.setCursor(10, 0);
  /*lcd.blink();
    lcd.display();*/
  if (l) {

    //return l;
    lcd.print(l);
    delay(1);
    return l;
  }

}
int enregistre() {

  ok = true;
}


/*void Date() {
  DateTime n = rtc.now();
            //     sprintf(h, "H:%d:%d", n.getHours(), n.getMinutes());
  }/*
  void DateInit() {
  rtc.begin();
  if ( rtc.isrunning()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }else{
      rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
   DateTime n = rtc.now();
  sprintf(h,"H:%d:%d", n.hour(), n.minute());
  }*/


void ako() {
  state = !state;
  nbrM(0);
  nbrT(0);
  nbrP(0);
}

void aload() {
  char a[16];
  lcd.clear ();
  lcd.setCursor(0, 1);
  sprintf(a, "saisir nombre");
  lcd.print(a);
  lcd.blink();
  delay(500);
}
void affichagePermanent1(char mi[16]) {
  /* lcd.clear();*/
  lcd.home();
  lcd.display();
  sprintf(mobile, mi);
  lcd.setCursor(abs((16 - strlen(mi)) / 2), 0);
  lcd.noAutoscroll();
  lcd.print(mobile);
  delay(1000);
} void affichagePermanent2(int nbr) {
  /* lcd.clear();*/
  lcd.home();
  lcd.display();
  lcd.setCursor(0, 0);
  lcd.noAutoscroll();
  lcd.print(nbr);

}
void affichagePermanentAvant(int a) {
  /* lcd.clear();*/
  lcd.home();
  lcd.display();
  lcd.setCursor(0, 1);
  sprintf(ao, "old:%d", a);
  lcd.print(ao);
  lcd.setCursor(9, 1);
  lcd.print("nw :");

}


void affichagePermanentDonnes(int m, int t, int p) {
  char  message [10];
  memset(lettrenombre, 0, 30);
  lcd.home();
  lcd.display();
  lcd.setCursor(0, 2);
  sprintf(br, "Br[KG]:%d", m);
  lcd.print(br);
  lcd.setCursor(12, 2);
  sprintf(tp, "T[C]:%d", t);
  lcd.print(tp);
  lcd.setCursor(0, 3);
  sprintf(pr, "P[bar]:%d", p);
  lcd.print(pr);

}


void affichageDynamique(char mo[30]) {
  lcd.setCursor(14, 0);
  lcd.display();
  lcd.autoscroll();
  lcd.leftToRight();
  sprintf(mobile, mo);
  for (int i = 0; i < strlen(mo); i++) {



    lcd.print(mo[i]);

    delay(100);
    //}
  }
  delay(1);
  //lcd.setCursor(1,1);
}


String recupE() {
  return lettrenombre2[1];
}


char getDate() {

  return d;
}
char getHeure() {

  return h;
}
void upDate(char i) {

  h[16] = i;
}
void upHeure(char i) {

  d[16] = i;
}


int  Nouvnombre(int x, int z) {

  char l = kep.getKey();

  if (l != NO_KEY ) {
    // attachInterrupt(digitalPinToInterrupt(btE), effaceSaisit, FALLING);
    lettrenombre[compteur] = l; // [compteur++] iz taloa
    compteur++;
    lcd.setCursor(x, z);

    if (l != ' ')
    {


      lcd.print(lettrenombre);
      /// tay=lettrenombre;
    } else {


      delay(1);

      //returnold();

      lettrenombre2[1] = lettrenombre;
      String p = lettrenombre;
      valF = p.toInt();
    /*  Serial.print("mandeha.......");
      Serial.println(valF);*/
      valfP = atof(lettrenombre);
      /* delay(1);


        delay(1);

        setold(p.toInt());

        memset(lettrenombre, 0, 30);
        nbrfinal = 0;
        valold = 0;
        delay(2);*/


    }


    return valF;

  }


}








int  returnold()
{
  return valold;
}

void setold(int e) {
  valold = e;

}

void  nbrT(int y) {

  temperature = y;
}
int returnT() {

  return temperature;
}

void  nbrP(double y) {

  pression = y;
}
double returnP() {

  return pression;
}

void  nbrM(int y) {

  masse = y;
}
int returnM() {

  return masse;
}
void mS(String l) {
  M = l;
}
String rmS() {
  return M;
}
void tS(String l) {
  T = l;
}
String rtS() {
  return T;
}
void pS(String l) {
  P = l;
}
String rpS() {
  return P;
}





void setV(String m) {
  V = m;
}

String  returnV() {
  return V;
}



void setMchar() {
  massChar2 = 55;
}
char returnMchar() {

  return massChar;
}
/*
  int setValeur(int  o){
  lettre=o;

  }
  int returnValeur(){

  return lettre;
  }
*/

void saisitD(boolean ab) {
  io = 2;
  int k;
   attachInterrupt(digitalPinToInterrupt(btS), sauverDonnes, LOW);
  affichagePermanentDonnes(returnM(), returnT(), returnP());
  if (digitalRead(btM) == LOW) {

    while (kep.getKey() != ' ')
    {


      delay(1);
      Nouvnombre(7, 2); //Nouvnombre(7, 0);
      if ((digitalRead(btT) == LOW))
      {

        /* //*----------------*
          lcd.clear();

          /// recupE();
          lcd.setCursor(0,0);
          lcd.print("final");
          lcd.setCursor(0,1);
          lcd.print(rmS());
          delay(1000);
          //*---------------*
        */

        delay(1);
        Nouvnombre(17, 2);//     Nouvnombre(13, 0);
        nbrM(valF);
        //Serial.println(lettrenombre);

        //returnM();
        break;
      } else if (digitalRead(btP) == LOW) //btT
      {
        delay(1);
        Nouvnombre(7, 3);//Nouvnombre(10, 1);
        break;
      }


    }

  } else if (digitalRead(btT) == LOW) {
    while (kep.getKey() != ',' || kep.getKey() != '.') {
      delay(1);
      //String b=
      Nouvnombre(17, 2);//   Nouvnombre(13, 0);
      if ((digitalRead(btP) == LOW))
      {
        nbrT(valF);

        // break;
        Nouvnombre(7, 3);//Nouvnombre(10, 1);
        break;

      } else if ((digitalRead(btM) == LOW)) {
        delay(1);
        break;
        delay(1);
        Nouvnombre(7, 2);//     Nouvnombre(7, 0);
        nbrT(valF);
      }

    }
  } else if (digitalRead(btP) == LOW) {

    //afficherDonnerF();


    while (kep.getKey() != ',' || kep.getKey() != '.') {
      delay(1);
      Nouvnombre(7, 3);//     Nouvnombre(10, 1);
      //nbrP(valF); original

      nbrP(valfP);

      if ((digitalRead(btM) == LOW))
      {
        // pS(returnV());
        delay(1);
        break;
        Nouvnombre(7, 2); //    Nouvnombre(7, 0);

      } else if ((digitalRead(btT) == LOW)) {
        delay(1);
        break;
        Nouvnombre(17, 2);//Nouvnombre(13, 0);
        // nbrP(valF); original
        nbrP(valfP);
      }/*else if(digitalRead(btS)==LOW){

     delay(1);
      afficherDonnerF();
        }*/

    }
    //MAJ
    /* } else if (digitalRead(btP) == LOW) {

       //afficherDonnerF();


       while (kep.getKey() != ',' || kep.getKey() != '.') {
         delay(1);
         Nouvnombre(7, 3);//     Nouvnombre(10, 1);
         //nbrP(valF); original

         nbrP(valfP);

        if ((digitalRead(btM) == LOW))
         {
           // pS(returnV());
           delay(1);
           break;
           Nouvnombre(7, 2); //    Nouvnombre(7, 0);

         } else if ((digitalRead(btT) == LOW)) {

           delay(1);
           break;
           Nouvnombre(17, 2);//Nouvnombre(13, 0);
           // nbrP(valF); original
           nbrP(valfP);
         }
       }*/


   // ab = false;
  }
}





void saisitD2() {
  if (digitalRead(btM) == LOW) {
    while (true) {
      if (digitalRead(btT) == LOW) {
        delay(1);
        setup();

        //saisitD();
        break;

      } else {
        delay(1);
        Nouvnombre(7, 0);
      }
    }

  } else if (digitalRead(btT) == LOW) {
    while (true)
    { Times();
      if (kep.getKey() == ' ' || kep.getKey() == '.') {
        break;
      }

      else {
        delay(1);
        Nouvnombre(13, 0);
      }
    }
  }
}


void effaceSaisit() {
  char  message [10];

  nbrM(0);
  nbrT(0);
  nbrP(0);
  valfP = 0.0;
  lcd.clear();
  affichagePermanentDonnes(returnM(), returnT(), returnP());
  etat = false;
  heures = false;
}




void sauverDonnes() {
  //envoieEsp32();
  afficherDonnerF();
  delay(100);
  //save=true;

}




void checkAlarms()
{
  RTCAlarmTime a1;
  RTCAlarmTime a2;

  if (clock.isArmed1())
  {
    a1 = clock.getAlarm1();

    if (clock.getAlarmType1() == DS3231_MATCH_H_M_S)
    {
      lcd.setCursor(0, 0);
      lcd.print("alarm par heure");
      delay(100);
    }
  } else
  { lcd.clear();
    //mise en veuille des module
    delay(10);
    lcd.setCursor(0, 0);
    lcd.print(" non alarm par heure");
  }

  if (clock.isArmed2())
  {
    a2 = clock.getAlarm2();
    if (clock.getAlarmType2() == DS3231_EVERY_MINUTE)
    {
      lcd.clear();
      delay(10);
      lcd.setCursor(1, 1);
      lcd.print(" alarm par minute");

    } else
    { lcd.clear();
      delay(1);
      lcd.setCursor(1, 1);
      lcd.print(" non alarm par minute");
    }
  }

}




void Times() {
 
  delay(10);
  dt = clock.getDateTime();
  sprintf(h, "%d:%d:%d", dt.hour, dt.minute, dt.second);
  sprintf(d, "%d/%d/%d", dt.day, dt.month, dt.year);
  heures = false;
  lcd.setCursor(5, 0);
  lcd.print(d);
  lcd.setCursor(5, 1);
  lcd.print(h);
  delay(1000);
  lcd.clear();



}

void loop() {

Times();
 saisitD(true);
//if(envoie){
  /*testeEnvoie(); //affectation des valeurs avant envoie
  delay(1);
  Serial2.println(faran); //envoie des donnes 
  //Serial.println(faran);
   
 envoie=false;
  
  }else{
   
    
    }


  
  /*if (fasy) {
    Times();
  }
  else {
    decoupString();
  }*/
  
  /* if (heures) {

      saisitD(true);




    }
    else {
      Times();


    }
    /* if (heures) {

       Times();
       lcd.setCursor(2, 0);
       lcd.print(h);

       heures = false;
      }
  */

  //testB();
  /*lcd.clear();
    lcd.setCursor(0,0);
    lcd.print ("test key");*/

  /*  commande();

    if(clock.isAlarm1()){
    com=true;
    }else if(clock.isAlarm2()){
       lcd.clear();
       lcd.setCursor(0,0);
    delay(10);
    lcd.print("alarm 2");
    delay(1000);

      }
  */
}
void commande() {
  if (com) {
    lcd.clear();
    delay(1);
    while (com) {
      // saisitD();



    }
  } else {
    Times();
  }


}


int NouvnombreF(int x, int y) {

  char l = kep.getKey();
  valold = 0;
  if (l != NO_KEY) {
    lettrenombre[compteur] = l;
    compteur++;
    lcd.setCursor(x, y);
    lcd.print(lettrenombre);
  }
  if (l == ' ') {
    //valold=atoi('1');

    memset(lettrenombre, 0, 30);
  } else if (l == '.') {
    valold = atoi(lettrenombre);
    delay(1);
    lcd.setCursor(13, 1);
    nbrP(valold);
    memset(lettrenombre, 0, 30);
    valold = 0;
    setup();

  }

  return valold;
  // stat=!stat;
}
void testB() {
  if (digitalRead(btM) == LOW) {

    Serial.println("boutton M");
  } else if (digitalRead(btP) == LOW)
  { Serial.println("boutton P");
  } else if  (digitalRead(btT) == LOW) {
    Serial.println("boutton T");
  } else {
    Serial.println("......");

  }

}
void valeurbt() {
  NouvnombreF(7, 0);
  if (digitalRead(btM) == LOW)
  { nbrM(NouvnombreF(7, 0));
    delay(1);
    lcd.setCursor(13, 0);
    NouvnombreF(13, 0);
    //setup();

  } else if ((digitalRead(btT) == LOW))
  { nbrT(NouvnombreF(13, 0));
    delay(1);
    //  setup();
    NouvnombreF(5, 0);
  } else if ((digitalRead(btP) == LOW)) {

    nbrP(NouvnombreF(5, 0));
    delay(1);
    //setup();
    NouvnombreF(7, 0);
  }


}
//ecriture
void ecrire(int m, int t, float p) {
  donnes [io][1] = m;
  donnes[io][2] = t;
  donnes[io][3] = p;

  delay(1);
  Serial.println(donnes[io][5]);

  Serial.println(io);
  io++;
}

//creation des fichier
void creationFichier() {
  while (!Serial) {

  }
  Serial.println("Initializing SD card...");

  if (!SD.begin(53)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");
  /*sprintf(dateFC, "%d/%d/%d.txt", dt.day, dt.month, dt.year);
    dateFichier = dateFC;


    // si le fichier existe
    SD.exists(dateFichier){
    myFile = SD.open(dateFichier, FILE_WRITE);
    //+sans creation des entete des donnes
    if (myFile) {
    //creation des entete des donnes
    Serial.print("Writing to test.txt...");
    myFile.println("testing 1, 2, 3.");
    // close the file:
    myFile.close();
    Serial.println("done.");
    } else {
    Serial.println("error opening test.txt");
    }
    }else{
    //creation d'entete dabord

    if (myFile) {
    //creation de ligne pour le tableau
      printf(don,"  %d      %d                 %d                   %d",returnM(),returnT(),returnP());
      String donf=dateFichier + don;
    myFile.println(enT2);
    myFile.println(donf); //ecriture des donnes final
    // close the file:
    myFile.close();
    Serial.println("done.");
    } else {
    Serial.println("erreur de l'ecriture du fichier");
    }
    }
  */
}
void envoieEsp32() {
  sprintf(avantFaran, "d:%d&h:%d&M:%d&T:%d&P:%d", d, h, returnM(), returnT(), returnP());
  faran = avantFaran;
  //sw.print(faran);
  delay(1);
  lcd.clear();
  lcd.print("donnes envoyeé");
  delay(1000);
  //}
}

void testeEnvoie() {
  //a noter ajout des date : jj/mm/aa h:m:s
  String vide = "";
  vide.concat(valfP);
  //sprintf(avantFaran, "%d/%d/%d     %d:%d        %d             %d        ", d, h, returnM(), returnT()) ;
 sprintf(avantFaran, "%s            %s         %d             %d                  ", d, h, returnM(), returnT()) ;
 // sprintf(avantFaran, "                     %d             %d                  ", returnM(), returnT()) ;
  faran = avantFaran+ String( "              ") +vide +"\n";

}
void upTime() {
  if (Serial2.available()) {
    String valeur = "";
    //valeur=Serial2.readString();
    valeur = "a,b,c,d,e,f";
    delay(1);
  char charval[valeur.length() + 1];
    valeur.toCharArray(charval, valeur.length() + 1);
    for (int i = 0; i <= valeur.length(); i++) {
      char *commandes = strtok(charval, ",");
    }

    Serial.println(commandes);

    //clock.setDateTime(mesDate[0],mesDate[1],mesDate[2],mesDate[3],mesDate[4],mesDate[5]);
    delay(10);
    //sprintf(h, "%d:%d:%d", dt.hour, dt.minute, dt.second);
    //Serial.print(commande);

  }
  Serial.println("liaison en serie non etablie......");
  //Serial.println(valeur);

}


void decoupString() {
  if (Serial2.available()) {
    char texte[255] ; String mota;
    mota = Serial2.readString();
    //sprintf(te,"%d",mota);
    char * p;
    mota.toCharArray(texte, 255);
    p = strtok(texte, ",");
    int i = 0;
    for (i; i < 6; i++) {
      dateAjour[i] = atoi(p);
      Serial.println(dateAjour[i]);
      p = strtok(NULL, ",");
    }
    clock.begin();
    clock.setDateTime(dateAjour[0], dateAjour[1], dateAjour[2], dateAjour[3], dateAjour[4], dateAjour[5]);
    fasy = true;
    //Serial2.end();

    return 0;
  }
  Serial.println("dans ----------------");
}
void appelSaisit() {
  saisirData = true;

}
void afficherDonnerF() {
  lcd.clear();
  delay(1);
  lcd.setCursor(0, 0);
  lcd.print("M:");
  lcd.setCursor(4, 0);
  lcd.print(returnM());
  lcd.setCursor(9, 0);
  lcd.print("P:");
  lcd.setCursor(13, 0);
  lcd.print(returnP());
  lcd.setCursor(0, 1);
  lcd.print("T:");
  //lcd.setCursor(4, 1);
  lcd.print(returnT());
  lcd.setCursor(9, 1);
  testeEnvoie(); //affectation des valeurs avant envoie
  delay(1);
  Serial2.println(faran);
 /*if (debut!=1){
  Serial2.println(faran); //envoie des donnes 
 }else{
  debut=1;
  }*/
detachInterrupt(digitalPinToInterrupt(btS));
  delay(1);
  fasy = true;
  envoie=true;
  /*clickS++;

    delay(100);
    //etat=false;*/
}


