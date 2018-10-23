const int LED = 9;
const int SENS1 = 1;
const int SENS2 = 2;
const int SENS3 = 3;
int a, b, c;
const int PINPROG1 = 4;
const int PINPROG2 = 5;
const int LANE1 = 2;
const int LANE2 = 3;
const int LANE3 = 4;

const int OLANE1 = 5;
const int OLANE2 = 6;
const int OLANE3 = 7;

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
      digitalWrite(OLANE2, HIGH);
      digitalWrite(OLANE3, HIGH);

      Serial.println("Syn successful!");
    }
  }
  digitalWrite(OLANE1, HIGH);
  digitalWrite(OLANE2, HIGH);
  digitalWrite(OLANE3, HIGH);
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
    } else {
      duration = 50;
    }
  }

  Serial.println("Syn ended!");
}

void reset_sync(void) {
  digitalWrite(OLANE1, LOW);
  digitalWrite(OLANE2, LOW);
  digitalWrite(OLANE3, LOW);
}

void setup() {
  // put your setup code here, torun once:
  a = 0;
  b = 0;
  c = 0;
  Serial.begin(9600);

  pinMode(LED, OUTPUT);

  pinMode(OLANE1, OUTPUT);
  pinMode(OLANE2, OUTPUT);
  pinMode(OLANE3, OUTPUT);
  pinMode(LANE1, INPUT);
  pinMode(LANE2, INPUT);
  pinMode(LANE3, INPUT);

  //  read_pinprog();
  digitalWrite(OLANE1, LOW);
  digitalWrite(OLANE2, LOW);
  digitalWrite(OLANE3, LOW);
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

void loop() {
  long d;
  bool okay;
  // put your main code here, to run repeatedly:
  digitalWrite(LED, LOW);
  xsynchro(25, 35);
  delay(5);
  reset_sync();

  //d = fakultaet(20);
  blink_led(5);
  i++;
}
