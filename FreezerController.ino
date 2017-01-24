//leds
int porta_rele1 = A0;
int Pin1 = 2;
int Pin2 = 3;
int Pin3 = 4;
int Pin4 = 5;
int Pin5 = 6;
int Pin6 = 7;

//sensor lm35
const int LM35 = A5;
float temperatura;
int index;
float janela[100];
float janelaSpam[20];
int timeSpam;
float lastTemperature;
float * readNValues(int n)
{
  for (int i = 0; i < n; i++)
  {
    janela[i] = temperatura = (float(analogRead(LM35)) * 5 / (1023)) / 0.01;
    delay(10);
  }
  return janela;
}
int printJanela() {
  for (int i = 0; i < 10; i++)
  {
    Serial.print("janela");
    Serial.println(janela[i]);
  }
  return 0;
}

float media(float vetor[], int n)
{
  float media = 0;
  for (int i = 0; i < n; i++)
  {
    media += vetor[i];
  }
  return float(media / n);
}
float makeSample()
{
  int n = 100;
  readNValues(n);
  return media(janela, n);
}
int offLights()
{ digitalWrite(Pin1, LOW);
  digitalWrite(Pin2, LOW);
  digitalWrite(Pin3, LOW);
  digitalWrite(Pin4, LOW);
  digitalWrite(Pin5, LOW);
  digitalWrite(Pin6, LOW);
  return 0;
}
int lightLed(float heat)
{

  delay(200);
  if (heat >= 32) {
    digitalWrite(Pin1, HIGH);
    heat -= 32;
  }
  if (heat >= 16) {
    digitalWrite(Pin2, HIGH);
    heat -= 16;
  }
  if (heat >= 8) {
    digitalWrite(Pin3, HIGH);
    heat -= 8;
  }
  if (heat >= 4) {
    digitalWrite(Pin4, HIGH);
    heat -= 4;
  }
  if (heat >= 2) {
    digitalWrite(Pin5, HIGH);
    heat -= 2;
  }
  if (heat >= 1) {
    digitalWrite(Pin6, HIGH);
    heat -= 1;
  }
  return 0;
}
void setup() {
  
  index = 0;
  timeSpam = 20;
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(Pin1, OUTPUT);
  pinMode(Pin2, OUTPUT);
  pinMode(Pin3, OUTPUT);
  pinMode(Pin4, OUTPUT);
  pinMode(Pin5, OUTPUT);
  pinMode(Pin6, OUTPUT);
  pinMode(porta_rele1, OUTPUT); 
  Serial.begin(9600);
  for (int i = 0; i < 10; i++)
  {
    janela[i] = 0;
  }
}

void loop() {
  float timeAv;
  float a = makeSample();
  float dT = 0;
  janelaSpam[index] = a;
  lastTemperature = janelaSpam[index - 1];
  dT = janelaSpam[index] - lastTemperature;
  if (dT < 0)
  {
    digitalWrite(LED_BUILTIN, LOW);
  }
  else {
    digitalWrite(LED_BUILTIN, HIGH);
  }
  index++;
  index = index % timeSpam;
  Serial.print("Temperatura: ");
  Serial.println(a, 3);
  if (index == timeSpam - 1)
  {
    timeAv = media(janelaSpam, timeSpam);
    Serial.print("Temperatura Ultimos 20 minutos: ");
    Serial.println(timeAv, 3);
    offLights();
    if (timeAv>30)
      {
        digitalWrite(porta_rele1, HIGH); //Desliga rele 1
      }
    else
    {
      digitalWrite(porta_rele1, LOW);  //Liga rele 1
      }
  }
  lightLed(timeAv);
  delay(200);
}
