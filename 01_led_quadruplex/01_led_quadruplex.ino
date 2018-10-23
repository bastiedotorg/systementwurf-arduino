const int LED = 9;
const int SENS1 = 1;
const int SENS2 = 2;
const int SENS3 = 3;
int a, b, c;

long fakultaet (long i) {
  long sum = 1;
  while (i > 0) {
    sum = sum * i;
    i--;
  }
}

void setup() {
  // put your setup code here, torun once:
  a = 0;
  b = 0;
  c = 0;
  pinMode(LED, OUTPUT);
  pinMode(SENS1, INPUT);
  pinMode(SENS2, INPUT);
  pinMode(SENS3, INPUT);
  Serial.begin(9600);
}

void loop() {
  long d;  
  // put your main code here, to run repeatedly:
  a = digitalRead(SENS1);
  b = digitalRead(SENS2);
  c = digitalRead(SENS3);
  d = fakultaet(20);
  digitalWrite(LED, HIGH);
  delay(500);
  digitalWrite(LED, LOW);
  delay(500);
  Serial.println("Next");
}
