float kp = 0.50, ki = 0.001, kd = 2.00;
int dt;
unsigned long t, t_prev = 0;
float e, e_prev = 0.0, inte, inte_prev = 0.0;
float Vmax = 12.0, Vmin = -12.0, V = 0.1, ref, fb;
const byte PWMPin = 11, DirPin1 = 13, DirPin2 = 12;
#define refPin A0
#define FBPin A2
void WriteDriverVoltage(float V, float Vmax)      {
  int PWMval = int(255 * abs(V) / Vmax);
  if (PWMval > 255)  PWMval = 255;
  if (V > 0)                     {
    digitalWrite(DirPin1, HIGH);
    digitalWrite(DirPin2, LOW);  }
  else if (V < 0)                {
    digitalWrite(DirPin1, LOW);
    digitalWrite(DirPin2, HIGH); }
  else                           {
    digitalWrite(DirPin1, LOW);
    digitalWrite(DirPin2, LOW)   }
  analogWrite(PWMPin, PWMval);                    }
void setup()                                      {
  Serial.begin(115200);
  pinMode(DirPin1, OUTPUT);
  pinMode(DirPin2, OUTPUT);
  pinMode(PWMPin, OUTPUT);
  pinMode(FBPin, INPUT);
  pinMode(refPin, INPUT);                         }

void loop()                                       {
  ref = analogRead(refPin);
  fb =analogRead(FBPin);
  t = millis();
  dt = t - t_prev ;

  e = ref - fb;
  if (abs(e) < 7.0) e = 0.0;

  inte = inte_prev + (dt * (e + e_prev) / 2);
  V = kp*e + ki*inte + (kd*(e - e_prev)/dt) ;

  if (V > Vmax)            {
      V = Vmax;
      inte = inte_prev;    }
    if (V < Vmin)          {
      V = Vmin;
      inte = inte_prev;    }
  WriteDriverVoltage(V, Vmax);

    Serial.print(ref); Serial.print(" \t");
    Serial.print(fb); Serial.print(" \t ");
    Serial.print(e);  Serial.println(" \t ");

   t_prev = t;
   inte_prev = inte;
   e_prev = e;
   delay(10);                                     }