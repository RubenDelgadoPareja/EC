void setup()
{
  Serial.begin(9600);
  pinMode(8, OUTPUT);
}
 
void loop()
{ 
  for (unsigned i = 0; i < 5000; ++i)
  {
    Serial.println(i);
    tone(8, i);
    delay(5);
  }
}
