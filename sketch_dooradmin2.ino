int GreenPin = 13;
int RedPin = 9;
char input[25];
int card[10];
void setup() 
{
  pinMode(GreenPin,OUTPUT);
  pinMode(RedPin,OUTPUT);
  Serial.begin(9600);
  Serial.println("connection established");
}

void loop() 
{
  char c;
  int ndx=0;
  while (Serial.available() >0)
  {
    c = Serial.read();
    input[ndx] = c;
    ndx++;
  }
  input[ndx] = '\0';
//  for(int i =0; input[i]!= '\0'; i++)
//     {
//      Serial.print(input[i]);
//     }
     delay(1000); 
  char tempstr[25];
  strncpy(tempstr,input,25);
    if (tempstr[0] == 'R')
    {
      REMOTEACCESS();
    }
   else if (tempstr[0] == 'A')
    {
       ADDCARD();
    }    
 if (tempstr[0] == 'S')
  {
     FAKETEST();      
  }
 
}
  void REMOTEACCESS()
  {
    delay(500);
    Serial.println("Green is on");
    digitalWrite(GreenPin,HIGH);
    delay(3000);
    digitalWrite(GreenPin,LOW);
  }

  void ADDCARD()
  {
  int i = 0;
  delay(1000);
  Serial.println("CARDLIST ADDCARD FUNCTION");
  //delay(1000);
//   for(int K =0; input[K]!= '\0'; K++)
//  {
//   Serial.print(input[K]);
//  }
  
 char * token = strtok(input,",");
 while(token != NULL)
  {
   token = strtok(NULL,",");
   card[i]= atoi(token);
   i++;
  }
   for (int j = 0; card[j] != '\0'; j++)
   {
     Serial.println(card[j]);
   }
   
}  

void FAKETEST()
{
  delay(1000);
  Serial.println("FAKE TEST");
  int testcard;
  char * token = strtok(input,",");
  token = strtok(NULL,",");
  testcard= atoi(token);
  Serial.println(testcard);
   
  for (int j = 0; card[j] != '\0'; j++)
   {
     if(card[j] == testcard)
     {
      digitalWrite(GreenPin,HIGH);
      digitalWrite(RedPin,LOW);
      delay(3000);
      digitalWrite(GreenPin,LOW);
      return;
      }
   }  digitalWrite(RedPin,HIGH);
      digitalWrite(GreenPin,LOW);
      delay(3000);
      digitalWrite(RedPin,LOW);
}    
    
   
  
