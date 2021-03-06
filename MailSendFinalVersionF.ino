 //#include <vector.h>
#include <ESP_Mail_Client.h>
#include <SoftwareSerial.h>
#include <Arduino.h>
#if defined(ESP32)
bool ecrire = false;
String donnesEcrire;
int debut = 0;
boolean email=true;
int debute;
#define ESP_Mail_DEFAULT_SD_FS SD
#define CARD_TYPE_SD 1
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include <ESP_Mail_Client.h>
/*
#define WIFI_SSID "Wifi-PAYSANNAT" //"WIFI" // 
#define WIFI_PASSWORD  "BIONEXX_2019"// "Pass@TECH_2019" */

#define WIFI_SSID "WIFI"
#define WIFI_PASSWORD "Pass@TECH_2019"
#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465
hw_timer_t * timer = NULL;
volatile SemaphoreHandle_t timerSemaphore;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
#define RXD2 16
#define TXD2 17
char* nomF;
String donnees;
String apresTete = "--------------------------------------------------------------------------------------------------------------------";
String avantTete = " Date                  Heures       Masse[Kg]     Temperature[°C]                    Pression[Bar]";
char charnom[30];
File monFichier;
String compare;
String nowdate;
ESP_Mail_Session session;
int jour, mois, an, heur, sec, minu;
/* The sign in credentials */

#define AUTHOR_EMAIL "bionexxbriquet2021@gmail.com"
#define AUTHOR_PASSWORD "bionexx2021"
/* Recipient's email*/
int k;
#define RECIPIENT_EMAIL "emmanueleric4919@gmail.com"
 #define RECIPIENT_EMAIL2   "energie1@bionexx.com"
/* The SMTP Session object used for Email sending */
SMTPSession smtp;
/* Callback function to get the Email sending status */
String m, t, p, d, h;
String donnes[0];
char io[30];
void smtpCallback(SMTP_Status status);
SMTP_Message message;
SoftwareSerial sw(16, 17);
int heures = 60000000;
void setup() {
  
  SD.begin();
  Serial.begin(115200);//Serial.begin(115200);
  Serial.println("reception des donnes ");
  Serial2.begin(115200, SERIAL_8N1, 16, 17);

  // envoiDate();
  //Debut du parametrage mail

  Serial.print("Connecting to AP");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(200);
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  MailClient.Time.setClock(3, 0);
  affichageTime();
  Serial.println(renomerData());//"/21-10-2021.txt"
  //Serial.println(MailClient.Time.getHour());
int o=calculDebut();
//initAlarm(o);//1 heures
initAlarm(5*60*1000000);//teste pour 5 minutes
//initAlarm(5*60*100000);
}
void loop() {
 monAlarm();
/*
if(email){
 //l'ancient é-mail est envoyé normalement
  monAlarm(30); //minutes
}else{
  //email non envoyé , renvoyé après chaque 20minutes 
  Serial.println(" renvoie d'email aprés 5 min");
  delay(1000);
   monAlarm(5);
  }
  
  
  /*
    if (Serial2.available()) {
       Serial.println("ecriture...........");
     setDat();
      donnees = Serial2.readString();
        filtreDonnes(donnees);
      delay(10);
    ecrisCard(donnees);


    } else {
    Serial.println("else ...........");
    /* setDat();
    Synchrone();*/

  /* delay(1000);
    }*/
}
/* Callback function to get the Email sending status */
void smtpCallback(SMTP_Status status) {
  /* Print the current status */
  Serial.println(status.info());
  /* Print the sending result */
  if (status.success()) {
    Serial.println("----------------");
    ESP_MAIL_PRINTF("Message sent success: %d\n",
                    status.completedCount());
    ESP_MAIL_PRINTF("Message sent failled: %d\n",
                    status.failedCount());
    Serial.println("----------------\n");
    struct tm dt;
    for (size_t i = 0; i < smtp.sendingResult.size(); i++) {
      /* Get the result item */
      SMTP_Result result = smtp.sendingResult.getItem(i);
      time_t ts = (time_t)result.timestamp;
      localtime_r(&ts, &dt);
      ESP_MAIL_PRINTF("Message No: %d\n", i + 1);
      ESP_MAIL_PRINTF("Status: %s\n", result.completed ?
                      "success" : "failed");
      ESP_MAIL_PRINTF("Date/Time: %d/%d/%d %d:%d:%d\n",
                      dt.tm_year + 1900, dt.tm_mon + 1, dt.tm_mday, dt.tm_hour,
                      dt.tm_min, dt.tm_sec);
      ESP_MAIL_PRINTF("Recipient: %s\n", result.recipients);
      ESP_MAIL_PRINTF("Subject: %s\n", result.subject);
    }
    Serial.println("----------------\n");
  }
}
void lecturedata() {
  if (Serial.available()) {
    String valeur = "";

    Serial.println(valeur = Serial2.readString());

  } else {
    Serial.println("......");
  }



}

void setDat() {
  jour = MailClient.Time.getDay();
  mois = MailClient.Time.getMonth();
  an = MailClient.Time.getYear();
  heur = MailClient.Time.getHour();
  minu = MailClient.Time.getMin();
  sec = MailClient.Time.getSec();
}
char  *renomerData() {
  jour = MailClient.Time.getDay();
  mois = MailClient.Time.getMonth();
  an = MailClient.Time.getYear();
  delay(1);
  sprintf(charnom, "/%d-%d-%d.csv", jour, mois, an);
  nomF = charnom;
  return nomF;
}


void envoiDate() {
  sprintf(io, "%d,%d,%d,%d,%d,%d", an, mois, jour, heur, minu, sec);
  Serial2.print(io);
}

void affichageTime() {
  setDat();
  Serial.print("affichage de date");
  sprintf(io, "%d,%d,%d,%d,%d,%d", an, mois, jour, heur, minu, sec);
  // Serial.println(io);
  Serial2.println(io);
  delay(100);

}
void lectureDesDonnes() {
  if (Serial2.available()) {
    donnes[0] = Serial2.readString();
    Serial.println("la dernier valeur est ");
    Serial.println(donnes[0]);
    delay(100);
  } else {
    // Serial.println("...........");
    envoiDate();
    delay(100);
  }
}

void initCard() {
  if (!SD.begin()) {
    Serial.println("Card Mount Failed");
    return;
  } else {
    Serial.println("initialisation de la carte ,bien faite !..");

  }

}

void listDir(fs::FS &fs, const char * dirname, uint8_t levels) {
  initCard();
  Serial.printf("Listing directory: %s\n", dirname);

  File root = fs.open(dirname);
  if (!root) {
    Serial.println("Failed to open directory");
    return;
  }
  if (!root.isDirectory()) {
    Serial.println("Not a directory");
    return;
  }
}
void writeFile(fs::FS &fs, const char * path, const String message1) {
  initCard();
  Serial.printf("Writing file: %s\n", path);
  setDat();
  sprintf(io, "%d/%d/%d  %d:%d:%d", jour, mois,an, heur, minu, sec);
 /// String message = io + message1;
  String message =  message1;
  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  if (file.println(avantTete)) {
    file.println(message);
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}
void appendFile(fs::FS &fs, const char * path, const String message1) {
  Serial.printf("Appending to file: %s\n", path);
  setDat();
  sprintf(io, "%d/%d/%d  %d:%d:%d", jour, mois, an, heur, minu, sec);
 // String message = io + message1 + "\n" ;
  String message =message1 + "\n" ;
  File file = fs.open(path, FILE_APPEND);
  if (!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  if ( file.println(message)) {
    /* ;file.println(apresTete)
      String ferm=apresTete;
       file.println(ferm);*/
    Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}

void ecrisCard(String don) {
  char* nom = renomerData();

  if (SD.exists(nom)) {
    //append file
    int i = 0;
    if (don.equalsIgnoreCase(compare)) {

    } else {

      appendFile(SD, nom, don);
      Serial.print("appdent du fichier terminer....");

    }


  } else {

    writeFile(SD, nom, don) ; //ecriture du nouveau dossie
    Serial.println("donner ecris ..");
  }

  don = compare;
  ecrire = false;
}
void filtreDonnes(String daf) {
  String aiie = daf.substring(0, 1);                                                                                                                                                                                              
  String aiie2 = daf.substring(0, 0);                                                                                                                                                                                              
  Serial.println("notre donnes est :");
  Serial.println(daf);
  
}
void readFile(fs::FS &fs, const char * path) {
  Serial.printf("Reading file: %s\n", path);

  File file = fs.open(path);
  if (!file) {
    Serial.println("Failed to open file for reading");
    return;
  }

  Serial.print("Read from file: ");
  while (file.available()) {
    Serial.write(file.read());
  }
  file.close();
}
void initEmail(String a) {
  smtp.debug(1);
  char pp[56];
  a.toCharArray(pp, 56);
  /* Set the callback function to get the sending results */
  smtp.callback(smtpCallback);
  /* Declare the session config data */

  /* Set the session config */

  session.server.host_name = SMTP_HOST;
  session.server.port = SMTP_PORT;
  session.login.email = AUTHOR_EMAIL;
  session.login.password = AUTHOR_PASSWORD;
  session.login.user_domain = "";
  /* Declare the message class */

  // Set the message headers *
  message.sender.name = "Compteur de Briquet";
  message.sender.email = AUTHOR_EMAIL;
  message.subject = "Compte rendu de briquet le :";
  message.addRecipient("Emmanuel", RECIPIENT_EMAIL);
   message.addRecipient("PATRICE", RECIPIENT_EMAIL2);
}
void envoyeEmail() {
  String d = Date();
  initEmail(d);
  String textMsg = "compte Rendu de Briquet le :" + d;
  message.text.content = textMsg.c_str();
  message.text.charSet = "us-ascii";
  message.text.transfer_encoding =
    Content_Transfer_Encoding::enc_7bit;
  message.priority =
    esp_mail_smtp_priority::esp_mail_smtp_priority_low;
  message.response.notify = esp_mail_smtp_notify_success |
                            esp_mail_smtp_notify_failure | esp_mail_smtp_notify_delay;
  pieceJoint();
  if (!smtp.connect(&session))
    return;
  /* Start sending Email and close the session */

  if (!MailClient.sendMail(&smtp, &message))
    {Serial.println("Error sending Email, " +
                   smtp.errorReason());
email=false;
//renvoyer l'email apres 10 
     initAlarm(10*60*1000000);
    }else{
      email=true;
     separation();
       int h =calculDebut();
    initAlarm(h);
      }


}

String Date() {
  char g[200];
  setDat();
  sprintf(g, "%d/%d/%d", jour, mois, an);
  nowdate = g;

  return nowdate;
}
void pieceJoint() {
  SMTP_Attachment att;
  message.addAttachment(att);
  message.resetAttachItem(att);
  att.descr.filename = renomerData() ;
  att.descr.mime = "/";
  att.file.path = renomerData();
  att.file.path = renomerData();
  att.file.storage_type = esp_mail_file_storage_type_sd;
  att.descr.transfer_encoding =
    Content_Transfer_Encoding::enc_base64;
  /* Add attachment to the message */
  message.addAttachment(att);
}

void IRAM_ATTR onTimer() {
  // Increment the counter and set the time of ISR
  portENTER_CRITICAL_ISR(&timerMux);
  portEXIT_CRITICAL_ISR(&timerMux);
  xSemaphoreGiveFromISR(timerSemaphore, NULL);

}
void initAlarm(int h) {
  timerSemaphore = xSemaphoreCreateBinary();

  // Use 1st timer of 4 (counted from zero).
  // Set 80 divider for prescaler (see ESP32 Technical Reference Manual for more
  // info).
  timer = timerBegin(0, 80, true);

  // Attach onTimer function to our timer.
  timerAttachInterrupt(timer, &onTimer, true);

  // Set alarm to call onTimer function every second (value in microseconds).
  // Repeat the alarm (third parameter)
  timerAlarmWrite(timer, h, true);
  timerAlarmEnable(timer);
}
void monAlarm() {
  //int g=g2*60*1000000; 
  if (xSemaphoreTake(timerSemaphore, 0) == pdTRUE) {
    portENTER_CRITICAL(&timerMux);
    portEXIT_CRITICAL(&timerMux);
    //envoye d'e-mail
    Serial.println("execution de l'envoie......");
    envoyeEmail();
   /* int h =calculDebut();
    initAlarm(h);*/

  }
  else {
    if (Serial2.available()) {
      Serial.println("ecriture...........");
      setDat();
      donnees = Serial2.readString();
      ecrisCard(donnees);


    } else {
      setDat();
      Serial.println("else ...........");
delay(1000);


    }
  }
}
int Depart24() {
  int hr = 23 - MailClient.Time.getHour();
  int g = hr * 60 - MailClient.Time.getMin();
  k = g * 60;
   Serial.println("il nous reste minutes avant le 24h :");
   Serial.println(g);
  return k;
}
int Depart12() {
  int hr = 12 - MailClient.Time.getHour();
  int g = hr * 60 - MailClient.Time.getMin();
  k = g * 60;
  Serial.println("il nous reste minutes avant le 12h");
    Serial.println(g);
  return k;
}
int Maintenant() {
  int hr = 11 * 60 + 50;
  k = hr * 60;
  return k;
}
int calculDebut(){
  if( MailClient.Time.getHour()<12){
   int y= Depart12();
    Serial.println(" depart 12");
      Serial.println(y);
      debute=y*1000000;//en microseconde
    }else{
   int y=   Depart24();
    Serial.println(" depart 24");
      Serial.println(y);
      debute=y*1000000; //en microseconde 
      }
  return debute;
  }
  void separation(){
    char* nom = renomerData();
    appendFile(SD,nom,apresTete);
    }
   
