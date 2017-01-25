/* Prototypage du déplacement de l'écran avec l'utilisation des capteurs Ultrason HC-SR04 */

// définition des pins de l'Arduino utilisés
int trig1 = 10;
int echo1 = 11; 
int trig2 = 12;
int echo2 = 13;
int trig3 = 4;
int echo3 = 5;
int trig4 = 6;
int echo4 = 7;
const int led1 = 2; // Pour simuler l'ordre au moteur de faire monter l'écran
const int led2 = 3; // Pour simuler l'ordre au moteur de faire descendre l'écran

// déclaration de variables pour stocker la durée de l'impulsion des capteurs en retour et la distance calculée en cm
long lecture_echo; 
long cm, cm1, cm2, cm3, cm4;
// cm1 et cm2 correspondent aux capteurs fixes 1 et 2 en bas de la borne
// cm3 et cm4 correspondent aux capteurs mobiles 3 et 4 sur l'habillage de l'écran

int message = 0;
bool statutPresence = false;
int cptPresence = 0;
int cptAbsence = 0;

enum mouvement_ecran {AUCUN, CALCUL, MONTER, DESCENDRE};
mouvement_ecran deplacement = AUCUN;
const int nb_mesure_par_boucle = 10; // Définit la précision et la fiabilité des mesures
const int DISTANCE_MIN = 80; // Distance en cm (paramètrable) à partir de laquelle on considère qu'il n'y a personne devant la borne
const int DISTANCE_MAX = 1000; // Distance en cm (paramètrable) de l'obstacle le plus éloigné possible de la borne (un mur)

void setup() 
{ 
  pinMode(trig1, OUTPUT); // Configure le pin digital 10 comme une sortie
  digitalWrite(trig1, LOW); // Le voltage du pin 10 est mis à 0 Volt
  pinMode(echo1, INPUT); // Configure le pin digital 11 comme une entrée
  
  pinMode(trig2, OUTPUT);
  digitalWrite(trig2, LOW);
  pinMode(echo2, INPUT);

  pinMode(trig3, OUTPUT);
  digitalWrite(trig3, LOW);
  pinMode(echo3, INPUT);
  
  pinMode(trig4, OUTPUT);
  digitalWrite(trig4, LOW);
  pinMode(echo4, INPUT);

  pinMode(led1, OUTPUT);
  digitalWrite(led1, LOW);

  pinMode(led2, OUTPUT);
  digitalWrite(led2, LOW);
  
  Serial.begin(9600); // Configure le taux de données transmises en bit par seconde pour communiquer avec le Raspberry
}

void loop() 
{ 
  cm1 = 0; cm2 = 0; cm3 = 0; cm4 = 0;
  // On réalise nb_mesure_par_boucle mesures de distance pour chaque capteur
  for (int i=1; i<=nb_mesure_par_boucle; i++){
    
    cm = DISTANCE_MAX + 1;
    while (cm>DISTANCE_MAX){
      digitalWrite(trig1, HIGH); // Le voltage du pin 10 de sortie est mis à 5V
      delayMicroseconds(10);  // Le programme est mis en pause pendant 10 micro-secondes pendant lesquelles une impulsion est émise
      digitalWrite(trig1, LOW); // Le voltage du pin 10 de sortie est mis à 0V
      lecture_echo = pulseIn(echo1, HIGH); // On stocke la durée que met l'entrée à atteindre un voltage de 5V en retour d'impulsion
      cm = lecture_echo / 58; // On divise la durée de l'impulsion par 58 pour calculer la distance en cm
    }
    cm1 = cm1 + cm;
    
    cm = DISTANCE_MAX + 1;
    while (cm>DISTANCE_MAX){
      digitalWrite(trig2, HIGH);
      delayMicroseconds(10);
      digitalWrite(trig2, LOW);
      lecture_echo = pulseIn(echo2, HIGH);
      cm = lecture_echo / 58;
    }
    cm2 = cm2 + cm;
    
    cm = DISTANCE_MAX + 1;
    while (cm>DISTANCE_MAX){
      digitalWrite(trig3, HIGH);
      delayMicroseconds(10);
      digitalWrite(trig3, LOW);
      lecture_echo = pulseIn(echo3, HIGH);
      cm = lecture_echo / 58;
    }
    cm3 = cm3 + cm;

    cm = DISTANCE_MAX + 1;
    while (cm>DISTANCE_MAX){
      digitalWrite(trig4, HIGH);
      delayMicroseconds(10);
      digitalWrite(trig4, LOW);
      lecture_echo = pulseIn(echo4, HIGH);
      cm = lecture_echo / 58;
    }
    cm4 = cm4 + cm;
  }

  // On calcule la moyenne des mesures pour chaque capteur
  cm1 = cm1/nb_mesure_par_boucle; cm2 = cm2/nb_mesure_par_boucle; cm3 = cm3/nb_mesure_par_boucle; cm4 = cm4/nb_mesure_par_boucle;
  
  // On allume la Led 1 si l'écran doit monter, la led 2 si l'écran doit descendre
  if (statutPresence == false){
    if ((cm1<=DISTANCE_MIN || cm2<=DISTANCE_MIN)){
      // on détecte une présence devant la borne
      if (cptPresence<3){
         cptPresence++;
         cptAbsence = 0;
      }
      else {
         // on estime qu'une personne est devant la borne, on lance l'ajustement de l'écran
         statutPresence = true;
         cptPresence = 0;
         deplacement = CALCUL;
         Serial.print(true); // on envoit l'information au Raspberry
      }
    }
  }
  else {
    if ((cm1>DISTANCE_MIN && cm2>DISTANCE_MIN)){
      // on ne detecte plus de présence devant la borne
      if (cptAbsence<3){
        cptAbsence++;
        cptPresence = 0;
        }
      else {
        // on estime qu'il n'y a plus personne devant la borne
        statutPresence = false;
        cptAbsence = 0;
       }
    }
  }

  if (statutPresence){
      if (cm3<=DISTANCE_MIN && cm4<=DISTANCE_MIN){
          if (deplacement==CALCUL || deplacement==MONTER){
            deplacement=MONTER;
          }
          else {
            deplacement = AUCUN;
          }
          
      }
      else if (cm3>DISTANCE_MIN || cm4>DISTANCE_MIN){ //implicite
          if (deplacement==CALCUL || deplacement==DESCENDRE){
            deplacement=DESCENDRE;
          }
          else {
            deplacement = AUCUN;
          }
        }
  }
  else deplacement = AUCUN;

  // On allume la Led 1 si l'écran doit monter, la led 2 si l'écran doit descendre
  switch (deplacement){
    case AUCUN:
        digitalWrite(led1, LOW);
        digitalWrite(led2, LOW);
        break;
    case MONTER:
        //on indique que l'écran doit monter
        digitalWrite(led1, HIGH);
        digitalWrite(led2, LOW);
        break;
    case DESCENDRE:
        //on indique que l'écran doit descendre
        digitalWrite(led1, LOW);
        digitalWrite(led2, HIGH);
        break;
  }
  
  delay(1000); // Le programme est mis en pause pendant 1000 millisecondes (1s)
}
