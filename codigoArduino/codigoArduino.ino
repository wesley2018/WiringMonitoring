//INCLUSÃO DAS BIBLIOTECAS NECESSÁRIAS PARA A EXECUÇÃO DO CÓDIGO
#include <SPI.h>
#include <Client.h>
#include <Ethernet.h>
#include <Server.h>
#include <Udp.h>
#include <dht.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; 
byte ip[] = { 192, 168, 0, 105 };  
byte gateway[] = { 192, 168, 0, 1 }; 
byte subnet[] = { 255, 255, 255, 0 }; 
EthernetServer server(80); //CASO OCORRA PROBLEMAS COM A PORTA 80, UTILIZE OUTRA (EX:8082,8089)
byte sampledata=50;


#define dht_dpin A2 //PINO DATA DO SENSOR DHT11 LIGADO NA PORTA ANALÓGICA A2
dht DHT; // DECLARAÇÃO DA VARIÁVEL DO TIPO DHT

String umidade; //CRIA UMA STRING CHAMADA "umidade"
String temperatura; //CRIA UMA STRING CHAMADA "temperatura"


void setup(){

  Ethernet.begin(mac, ip, gateway, subnet); // INICIALIZA A CONEXÃO ETHERNET
  Serial.begin(9600);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);  
}
void loop(){

  DHT.read11(dht_dpin); // LÊ AS INFORMAÇÕES DO SENSOR
  umidade = String(DHT.humidity); //  STRING "umidade" RECEBE O VALOR MEDIDO
  temperatura = String(DHT.temperature); // STRING "temperatura" RECEBE O VALOR MEDIDO
   
delay(1);//Delay para o Arduino não perder o dado da Serial  

String readString;
String numeroRetorno = "12345";

EthernetClient client = server.available(); 
  if (client) {     
    while (client.connected()) {             
        if (client.available()) {         
        char c = client.read();
               
        if(readString.length()<100){
          readString += c;
        }
                
        //Se encontrou o final da palavra que o android enviou
        
        if( c == '\n'){
          
              // Aqui vai comparar a palavra montada
              
              if(readString=="temperatura\n"){
                Serial.println("Palavra temperatura ");
                client.print(temperatura);
                client.stop();
                delay(1);
              }
    
              if(readString=="umidade\n"){
                Serial.println("Palavra umidade ");
                client.print(umidade);
                client.stop();
                delay(1);
              }
    
              if (readString=="lampadaacesa\n"){
                Serial.println("Acionando lampada!");                       
                digitalWrite(4, HIGH);
                client.print(numeroRetorno); 
                client.stop();      
                delay(1);   
              }
              
              if (readString=="lampadaapagada\n"){
                Serial.println("Desligando lampada!");                      
                digitalWrite(4, LOW);  
                client.print(numeroRetorno);
                client.stop();     
                delay(1);  
              }
              
              if (readString=="ventiladorligado\n"){
                Serial.println("Acionando ventilador!");           
                digitalWrite(5, HIGH);   
                client.print(numeroRetorno);
                client.stop();     
                delay(1);       
              }
              
              if (readString=="ventiladordesligado\n"){
                Serial.println("Desligando ventilador!");          
                digitalWrite(5, LOW);  
                client.print(numeroRetorno); 
                client.stop();     
                delay(1); 
              } 

              if(readString=="lam\n"){
                Serial.print("lampada: ");
                Serial.println(digitalRead(4));
                
                if(1 == digitalRead(4)){
                  client.print("11111");
                  client.stop();
                  delay(1);
                }
                if(0 == digitalRead(4)){
                  client.print("22222");
                  client.stop();
                  delay(1);
                }
                
              }

              if(readString=="ven\n"){
                Serial.print("ventilador: ");
                Serial.println(digitalRead(5));
                
                if(1 == digitalRead(5)){
                  client.print("33333");
                  client.stop();
                  delay(1);
                }
                if(0 == digitalRead(5)){
                  client.print("44444");
                  client.stop();
                  delay(1);
                }
              }

              if(readString=="conex\n"){
                client.print("55555");
                client.stop();
                delay(1);
              }

              //Se a palavra que o Android enviou não é nenhuma das acima, então fecha a conexão
              
              else{
                client.stop();
                delay(1);                      
              }
              
        } // final do bloco da comparação a palavra montada           
       } // final de cliente disponível          
     } // final de cliente conectado
   } // final de se existe cliente
 } // final do loop



