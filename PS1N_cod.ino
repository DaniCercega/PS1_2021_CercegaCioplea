
 
#include <Wire.h>
 
 
#define BUTTON_OK 2
#define BUTTON_CANCEL 3
#define BUTTON_PREV 4
#define BUTTON_NEXT 5
 
typedef struct Parametrii{
  double temp = 36.6;
  int sec_INC = 10;
  int sec_MEN = 20;
  int sec_RAC = 10;
  double kp = 5;
  double ki = 0.1;
  double kd = 0.1;
} Parametrii;
 
Parametrii pars;
 
enum Buttons {
  EV_OK,
  EV_CANCEL,
  EV_NEXT,
  EV_PREV,
  EV_NONE,
  EV_MAX_NUM
};
 
enum Menus {
  MENU_MAIN = 0,
  MENU_KP,
  MENU_KD,
  MENU_KI,
  MENU_TEMP,
  MENU_SEC_INC,
  MENU_SEC_MEN,
  MENU_SEC_RAC,
  MENU_MAX_NUM
};
 
 
double temp = pars.temp;
double kp = pars.kp;
double kd = pars.kd;
double ki = pars.ki;
int sec_INC = pars.sec_INC;
int sec_MEN = pars.sec_MEN;
int sec_RAC = pars.sec_RAC;
 
Menus scroll_menu = MENU_MAIN;
Menus current_menu =  MENU_MAIN;
 
void state_machine(enum Menus menu, enum Buttons button);
Buttons GetButtons(void);
void print_menu(enum Menus menu);
 
typedef void (state_machine_handler_t)(void);
 
void print_menu(enum Menus menu)
{
  switch(menu)
  {
    case MENU_KP:
      Serial.print("KP actual: ");
      Serial.print(pars.kp);
      Serial.print("   KP nou: ");
      Serial.print(kp);
      Serial.println();
      break;
    case MENU_KD:
      Serial.print("KD = ");
      Serial.print(kd);
      Serial.println();
      break;
    case MENU_KI:
      Serial.print("KI = ");
      Serial.print(ki);
      Serial.println();
      break;
    case MENU_TEMP:
      Serial.print("TEMP = ");
      Serial.print(temp);
      Serial.println();
      break;
    case MENU_SEC_INC:
      Serial.print("SEC_INC = ");
      Serial.print(sec_INC);
      Serial.println();
      break;
    case MENU_SEC_MEN:
      Serial.print("SEC_MEN = ");
      Serial.print(sec_MEN);
      Serial.println();
      break;
    case MENU_SEC_RAC:
      Serial.print("SEC_RAC = ");
      Serial.print(sec_RAC);
      Serial.println();
      break;
    case MENU_MAIN:
      //Serial.println("PS 2020");
      break;
    default:
      Serial.println("PS 2020");
      break;
  }
  if(current_menu != MENU_MAIN)
  {
    Serial.println("modifica");
  }
}
 
void enter_menu(void)
{
  current_menu = scroll_menu;
}
 
void go_home(void)
{
  scroll_menu = MENU_MAIN;
  current_menu = scroll_menu;
}
 
void go_next(void)
{
  scroll_menu = (Menus) ((int)scroll_menu + 1);
  scroll_menu = (Menus) ((int)scroll_menu % MENU_MAX_NUM);
}
 
void go_prev(void)
{
  scroll_menu = (Menus) ((int)scroll_menu - 1);
  scroll_menu = (Menus) ((int)scroll_menu % MENU_MAX_NUM);
}
 
void save_kp(void){kp = pars.kp;}
 
void inc_kp(void){kp+=0.1;}
 
void dec_kp(void){kp-=0.1;}
 
void save_kd(void){kd = pars.kd;}
 
void inc_kd(void){kd+=0.1;}
 
void dec_kd(void){kd-=0.1;}
 
void save_ki(void){ki = pars.ki;}
 
void inc_ki(void){ki+=0.1;}
 
void dec_ki(void){ki-=0.1;}
 
void save_temp(void){temp = pars.temp;}
 
void inc_temp(void){temp++;}
 
void dec_temp(void){temp--;}
 
void save_sec_INC(void){sec_INC = pars.sec_INC;}
 
void inc_sec_INC(void){sec_INC++;}
 
void dec_sec_INC(void){sec_INC--;}
 
void save_sec_MEN(void){sec_MEN = pars.sec_MEN;}
 
void inc_sec_MEN(void){sec_MEN++;}
 
void dec_sec_MEN(void){sec_MEN--;}
 
void save_sec_RAC(void){sec_RAC = pars.sec_RAC;}
 
void inc_sec_RAC(void){sec_RAC++;}
 
void dec_sec_RAC(void){sec_RAC--;}
 
state_machine_handler_t* sm[MENU_MAX_NUM][EV_MAX_NUM] =
{ //events: OK , CANCEL , NEXT, PREV
  {enter_menu, go_home, go_next, go_prev},            // MENU_MAIN
  {go_home, save_kp, inc_kp, dec_kp},                 // MENU_KP
  {go_home, save_kd, inc_kd, dec_kd},                 // MENU_KP
  {go_home, save_ki, inc_ki, dec_ki},                 // MENU_KP
  {go_home, save_temp, inc_temp, dec_temp},             // MENU_TEMP
  {go_home, save_sec_INC, inc_sec_INC, dec_sec_INC},       // MENU_SEC_INC
  {go_home, save_sec_MEN, inc_sec_MEN, dec_sec_MEN},       // MENU_SEC_MEN
  {go_home, save_sec_RAC, inc_sec_RAC, dec_sec_RAC}        // MENU_SEC_RAC
};
 
void state_machine(enum Menus menu, enum Buttons button)
{
  sm[menu][button]();
}
 
Buttons GetButtons(void)
{
  enum Buttons ret_val = EV_NONE;
  if (digitalRead(BUTTON_OK))
  {
    ret_val = EV_OK;
  }
  else if (digitalRead(BUTTON_CANCEL))
  {
    ret_val = EV_CANCEL;
  }
  else if (digitalRead(BUTTON_NEXT))
  {
    ret_val = EV_NEXT;
  }
  else if (digitalRead(BUTTON_PREV))
  {
    ret_val = EV_PREV;
  }
 
  //Serial.print("Butonul apasat este: ");
  //Serial.print(ret_val);
  //Serial.println();
  return ret_val;
}
 
 
const int sensor=A1; // Assigning analog pin A1 to variable 'sensor'
 
int sampleRate = 1000; // timp esantionare (in milisecunde

void timer1()
{
  DDRB |= 1 << PB5; // dev pin
  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1B |= (1 << WGM12) | (1 << CS12) | _BV(CS10) ;
  TIMSK1 |= (1 << OCIE1A) ;
  OCR1A = 15624;

  sei();
}
 
void setup() {
  Serial.begin(9600);
  // Butoane:
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  // Pinul legat la tranzistor:
  pinMode(6,OUTPUT); 
  pinMode(sensor,INPUT); // Configuring pin A1 as input
  digitalWrite(2, LOW); // pull-down
  digitalWrite(3, LOW); // pull-down
  digitalWrite(4, LOW); // pull-down
  digitalWrite(5, LOW); // pull-down
  timer1();
}
 
double eroare= 0;
 
double suma_erori= 0;
 
double eroare_anterioara = 0;
 
double derivativa = 0;
 
 
double dt = sampleRate/1000; // timp esantionare (in secunde)
 
double setpoint = 30;
 
int output;
 
float tempc;  //variable to store temperature in degree Celsius
float vout;  //temporary variable to hold sensor reading
 
void loop() {
  //tcDisable(); //This function can be used anywhere if you need to stop/pause the timer
  //tcReset(); //This function should be called everytime you stop the timer
}
 

int uptime =0;

int moving_sp;
int remaining = 0;
void calculare_moving_sp (void)
{
  
  
 
  //Serial.println(uptime);
  int timp_inc = pars.sec_INC;
  int timp_men = pars.sec_MEN;
  int timp_rac = pars.sec_RAC;
  float temp = pars.temp;

  if(remaining == 0)
    remaining = timp_inc - uptime;
  
  if (uptime < timp_inc)
  {
    //Serial.println(" TInc:");
    //Serial.println(uptime);
    //Serial.println(timp_inc);
    Serial.println(moving_sp);
    moving_sp = temp * (timp_inc-remaining)/timp_inc;
    remaining = timp_inc - uptime;
  }
  else if (uptime <= (timp_inc + timp_men))
  {
    //Serial.println(uptime);
    //Serial.println(timp_inc + timp_men);
    Serial.println(moving_sp);
    remaining = (timp_inc + timp_men) - uptime;
  }
  else if (uptime <= (timp_inc + timp_men + timp_rac))
  {
    //Serial.println(uptime);
    //Serial.println(timp_inc + timp_men + timp_rac);
    Serial.println(moving_sp);
    moving_sp = temp - temp * (timp_rac - remaining)/timp_rac;
    remaining = (timp_inc + timp_men + timp_rac) - uptime;
  }
  else{
    Serial.println("Oprit: ");
    uptime=0;
    remaining=0;
    //o luam de la inceput
  }
}

 
 
ISR(TIMER1_COMPA_vect)
{
    volatile Buttons event = GetButtons();
  if (event != EV_NONE)
  {
    state_machine(current_menu, event);
  }
  print_menu(scroll_menu);
  calculare_moving_sp ();
  vout=analogRead(sensor);
  tempc=(vout*300)/1023;
  
 
  eroare = moving_sp - tempc;
 
  suma_erori= suma_erori + eroare * dt;
 
  derivativa = (eroare - eroare_anterioara) / dt ;
 
  output = (pars.kp * eroare) + (pars.ki * suma_erori ) + (pars.kd * derivativa);
   
  eroare_anterioara = eroare;
 
  //Serial.println(uptime);
  uptime++;
  //Serial.println(eroare);
  //Serial.println(suma_erori);
  //Serial.println(derivativa);
  Serial.println(output);
  analogWrite(6,output);
}
