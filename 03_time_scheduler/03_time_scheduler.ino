const int LED = 8;
const int SENS1 = 1;
const int SENS2 = 2;
const int SENSOR1 = A7;
int a, b, c;
const int PINPROG1 = 4;
const int PINPROG2 = 5;
const int LANE1 = A1;
const int LANE2 = A2;
const int LANE3 = A3;

const int OLANE1 = 5;

const int SYN = 8;

int lane_id = 99;
int duration = 500;

long fakultaet (long i) {
  long sum = 1;
  while (i > 0) {
    sum = sum * i;
    i--;
  }
}

void read_pinprog(void) {
  int pin1 = 0;
  int pin2 = 0;
  pin1 = digitalRead(PINPROG1);
  pin2 = digitalRead(PINPROG2);
  lane_id = pin1 + (pin2 << 1);
  Serial.print("My ID: ");
  Serial.println(lane_id);
}

long long GLOBAL_TIME;

void reset_time(void) {
  GLOBAL_TIME = millis();
}

long get_time(void) {
  return millis() - GLOBAL_TIME;
}


void xsynchro(int presyn_in, int syn_in) {
  long presyn = presyn_in;
  long syn = syn_in;
  int syn_msgs = 0;

  while (presyn > 0) {
    syn_msgs = (int) digitalRead(LANE1) + (int) digitalRead(LANE2) + (int) digitalRead(LANE3);
    Serial.write(syn_msgs);
    presyn--;
    delay(5);
    if (syn_msgs > 1) {
      presyn = 0;
      syn = 0;
      digitalWrite(OLANE1, HIGH);


      Serial.println("Syn successful!");
      digitalWrite(SYN, HIGH);
    }
  }
  digitalWrite(OLANE1, HIGH);

  while (syn > 0) {
    syn_msgs = (int) digitalRead(LANE1) + (int) digitalRead(LANE2) + (int) digitalRead(LANE3);
    Serial.write(syn_msgs);
    syn--;
    delay(5);
    if (syn_msgs > 1) {
      presyn = 0;
      syn = 0;
      duration = 500;
      Serial.println("Syn successful!");
      digitalWrite(SYN, HIGH);
    } else {
      digitalWrite(SYN, LOW);
    }
  }
  Serial.println("Syn ended!");
}

void reset_sync(void) {
  digitalWrite(OLANE1, LOW);

}

void setup() {
  // put your setup code here, torun once:
  a = 0;
  b = 0;
  c = 0;
  Serial.begin(9600);

  pinMode(LED, OUTPUT);

  pinMode(OLANE1, OUTPUT);

  pinMode(LANE1, INPUT);
  pinMode(LANE2, INPUT);
  pinMode(LANE3, INPUT);
  pinMode(SYN, OUTPUT);
  pinMode(SENSOR1, INPUT);
  //  read_pinprog();
  digitalWrite(OLANE1, LOW);

  digitalWrite(SYN, LOW);

  delay(100);
  xsynchro(150, 200);
}
int i = 0;

void blink_led(int times) {
  int a = 0;
  for (a = 0; a < times; a++) {
    digitalWrite(LED, HIGH);
    delay(250);
    digitalWrite(LED, LOW);
    delay(250);
  }
}

typedef struct job {
  long start_time;
  long stop_time;
  void (* job_function)();
} T_JOB;

void job_xsynchro(void) {
  xsynchro(25, 35);
}

void led_on(void) {
  digitalWrite(LED, HIGH);
}

void led_off(void) {
  digitalWrite(LED, LOW);
}

long a_sens = 0;

void read_sensor(void) {
  a_sens = analogRead(SENSOR1);
  Serial.print("Analog:");
  Serial.println(a_sens);
}

void send_xlane(void) {
  analogWrite(OLANE1, a_sens);
}

void read_xlane(void) {
  int xlane1 = analogRead(LANE1);
  int xlane2 = analogRead(LANE2);
  int xlane3 = analogRead(LANE3);
  long diff = (xlane1+xlane2+xlane3+a_sens-min(xlane1, min(xlane2, min(xlane3, a_sens)))-max(xlane1, max(xlane2, max(xlane3, a_sens))))/2;
  Serial.print("Xlane: ");
  Serial.print(xlane1);
  Serial.print(" ");
  Serial.print(xlane2);
  Serial.print(" ");
  Serial.print(xlane3);
  Serial.print(" ");
  Serial.print(a_sens);
  Serial.print(" ");
  Serial.println(diff);
}
#define NUM_JOBS 12
const T_JOB jobs[NUM_JOBS] = {
  {.start_time = 0, .stop_time = 0, .job_function = &job_xsynchro },
  {.start_time = 100, .stop_time = 0, .job_function = &reset_time },
  {.start_time = 200, .stop_time = 0, .job_function = &reset_sync },
  {.start_time = 400, .stop_time = 0, .job_function = &led_on },
  {.start_time = 500, .stop_time = 0, .job_function = &led_off },
  {.start_time = 1500, .stop_time = 0, .job_function = &read_sensor },
  {.start_time = 2500, .stop_time = 0, .job_function = &led_off },
  {.start_time = 3500, .stop_time = 0, .job_function = &send_xlane },
  {.start_time = 4500, .stop_time = 0, .job_function = &read_xlane },
  {.start_time = 5500, .stop_time = 0, .job_function = &led_on },
  {.start_time = 6500, .stop_time = 0, .job_function = &led_off },
  {.start_time = 7500, .stop_time = 0, .job_function = &reset_sync }
};


void runScheduler(void) {
  int service;
  for (service = 0; service < NUM_JOBS; service++) {
    while (get_time() < jobs[service].start_time) {
      delay(1);
    }
    jobs[service].job_function();
    while (get_time() < jobs[service].stop_time) {
      delay(1);
    }
  }
}

void loop() {
  runScheduler();
}
