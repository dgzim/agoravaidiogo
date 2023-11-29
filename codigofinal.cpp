#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define SensorUm 27

String UrlEncoder(String str){  // função para adaptar as palavras em URL;
  String encodedString = "";    // Ex:
  char c;                       // "Frase de teste" -> "Frase+de+teste";
  char code0;
  char code1;
  char code2;
  for (int i = 0; i < str.length(); i++)
  {
    c = str.charAt(i);
    if (c == ' ')
    {
      encodedString += '+';
    }
    else if (isalnum(c))
    {
      encodedString += c;
    }
    else
    {
      code1 = (c & 0xf) + '0';
      if ((c & 0xf) > 9)
      {
        code1 = (c & 0xf) - 10 + 'A';
      }
      c = (c >> 4) & 0xf;
      code0 = c + '0';
      if (c > 9)
      {
        code0 = c - 10 + 'A';
      }
      code2 = '\0';
      encodedString += '%';
      encodedString += code0;
      encodedString += code1;
      // encodedString+=code2;
    }
    yield();
  }
  return encodedString;
}


//--------------------------------------------------------------------------------
// protótipo de função

void Whatsappmessage(String message); // função declarada no final do código, 
//A API DO BOT NÃO ACEITA CARACTER ESPECIAL

//-----------------------------------------------------------------------------------------------------
// Dados da rede
const char* SSID = "LIVE TIM_9510_2G";
const char* SENHA = "k7644um747";
//-----------------------------------------------------------------------------------------------------
// Celular e chave da API

//-----------------------------------------------------------------------------------------------------
void setup()
{
  Serial.begin(115200);        // Iniciando a porta serial 115200
  WiFi.begin(SSID, SENHA);     // Iniciando a biblioteca WiFi
  Serial.println("Conecting"); // Mostra pelo serial que está conectando
  while (WiFi.status() != WL_CONNECTED)
  { // Aguarda a conexão
    delay(500);
    Serial.println("."); // Enquanto não conectar escreve "....."
  }
  Serial.println("");
  Serial.print("Conectado no IP: ");
  Serial.println(WiFi.localIP()); // Mostra em qual IP Espe está conectado

  Whatsappmessage("Hello World"); // Envia HELLO WORLD pra dar sorte
}
//-----------------------------------------------------------------------------------------------------
void loop()
{
  delay(100);
  int leitura = analogRead(SensorUm);
  Serial.println(leitura);
  if(leitura < 400){
    Whatsappmessage("Evacuar");
  }
  if(leitura >630  && leitura < 999){
    //Serial.println("Está tudo normal");
    ///Se usar essa parte vai consumir muita memória do ESP fazendo prints infinitos;
  }
  if(leitura >999){
    Serial.println("Sensor Desconectado");
    Whatsappmessage("Sensor Desconectado");
  }
}

void Whatsappmessage(String message)
{
  // Dados a serem enviados usando o método post no Protocolo HTTP
  String url = "https://api.callmebot.com/whatsapp.php?phone=numerodetelefone&text="+UrlEncoder(message)+"&apikey=chave";
  Serial.println(url);

  HTTPClient http;
  http.begin(url);  // iniciando protocolo http

  uint16_t httpResponseCode = http.POST(url);// usando método post com um ponteiro de 16bits
  if (httpResponseCode == 200)                // verificando código de status HTTP
  {
    Serial.println("Deu boa");
  }
  else
  {
    Serial.println("Deu ruim");
    Serial.print("Código HTTP");
    Serial.println(httpResponseCode);   //Callback do código para verificar o que aconteceu, se der errado verificar a linha de código 100;
  }
  http.end();// encerrando o protocolo pra começar outro depois
}
