//CONCLUIDO EM 01/02/2026
 /* CÓDIGO PARA  CONTROLE DE UM PORTAO ELETRONICO E LIGAR E DESLIGAR 8 LAMPADAS
 * USO FIM DE CURSO POR ÍMÁ E AUTOMÁTICO POR GRAVAÇÃO DE PERCUSO 
 * 
 
 */

 /*
/* =================================================================================================
   SISTEMA DE CONTROLE DE PORTÃO + 8 LÂMPADAS
   AUTOR: VALMIR
   PROJETO: CONTROLADOR EMBARCADO COM MEMÓRIA EEPROM, MENU POR LED, E RAMPA DE DESACELERAÇÃO

   -------------------------------------------------------------------------------------------------
   DESCRIÇÃO GERAL
   -------------------------------------------------------------------------------------------------
   Este sistema controla:
   - Um portão eletrônico com abertura, fechamento, pausa e reversão.
   - Até 20 controles RF memorizados.
   - Até 8 lâmpadas por controle (mapeamento por matriz).
   - Gravação automática do percurso de abertura, fechamento e rampa.
   - Menu de configuração usando apenas 1 botão e 1 LED.
   - Memória permanente em EEPROM.

   O LED da LAMPADA1 é usado como:
   - Feedback de posição.
   - Confirmação.
   - Erro.
   - Status do sistema.

   -------------------------------------------------------------------------------------------------
   ESTADOS PRINCIPAIS
   -------------------------------------------------------------------------------------------------
   1) MODO NORMAL
      - Controles acionam o portão ou lâmpadas conforme a configuração.
      - LED indica estado do portão.
      - Nenhuma gravação é possível se o portão não estiver configurado.

   2) MODO APRENDER (CONTROLES)
      - Entrado ao clicar o botão APRENDER.
      - Cada clique avança a posição (1 a 20).
      - O LED pisca conforme a posição.
      - Se a posição estiver vazia: LED fica LIGADO ao final.
      - Se já estiver gravada: LED fica APAGADO ao final.

   3) MODO MENU (MAPEAR LÂMPADAS)
      - Só pode entrar se já existir percurso gravado.
      - Segure o botão APRENDER por 4 segundos.
      - LED apaga → menu ativo.
      - A navegação começa após 1,5s (proteção de ruído).
      - Cada clique avança a lâmpada (1 a 8).
      - LED pisca conforme a lâmpada selecionada.

   -------------------------------------------------------------------------------------------------
   COMO CONFIGURAR O PERCURSO DO PORTÃO
   -------------------------------------------------------------------------------------------------
   1) ZERAR PERCURSO
      - Clique o botão APRENDER até chegar na posição 0.
      - LED apaga.
      - Segure até LED piscar → percurso zerado.

   2) GRAVAR ABERTURA
      - Portão fechado.
      - Clique o botão APRENDER uma vez.
      - Portão abre e grava o tempo até o fim de curso.

   3) GRAVAR FECHAMENTO
      - Após abrir, o sistema fecha automaticamente após 4s.
      - Grava o tempo até o fim de curso fechar.

   4) GRAVAR RAMPA
      - Após fechar, o portão abre sozinho.
      - Para automaticamente.
      - Depois fecha sozinho.
      - No final grava a rampa de desaceleração.

   Portão 100% configurado.

   -------------------------------------------------------------------------------------------------
   COMO GRAVAR UM CONTROLE RF
   -------------------------------------------------------------------------------------------------
   1) Clique o botão APRENDER.
   2) O LED pisca conforme a posição (1 a 20).
   3) Quando estiver na posição desejada:
      - Pressione o botão do controle RF.
      - LED apaga.
      - Controle gravado.

   -------------------------------------------------------------------------------------------------
   COMO APAGAR UM CONTROLE
   -------------------------------------------------------------------------------------------------
   - Vá até a posição do controle.
   - Segure o botão APRENDER.
   - LED pisca → posição apagada.

   -------------------------------------------------------------------------------------------------
   COMO CONFIGURAR UMA LÂMPADA PARA UMA TECLA
   -------------------------------------------------------------------------------------------------
   1) Selecione a tecla no modo aprender.
   2) Segure o botão por 4s → entra no menu.
   3) Aguarde 1,5s.
   4) Cada clique avança a lâmpada (1 a 8).
   5) LED pisca conforme a lâmpada.
   6) Segure por 2s → confirma.
   7) LED pisca rápido → configuração salva.
   8) Gravar o controle → clique na tecla desejada
   9) controle gravdo.


   -------------------------------------------------------------------------------------------------
   COMPORTAMENTO DOS CONTROLES
   -------------------------------------------------------------------------------------------------
   - Nenhuma tecla é fixa para portão ou lâmpadas.
   - Cada controle pode ser configurado livremente pelo menu.
   - O comportamento depende exclusivamente do mapeamento salvo.

   - Se a tecla NÃO tiver nenhuma lâmpada configurada:
       → controla o portão (abrir / fechar / pausa / reversão).

   - Se a tecla TIVER uma ou mais lâmpadas configuradas:
       → NÃO movimenta o portão.
       → apenas liga/desliga as lâmpadas associadas.

   - A decisão é feita em tempo real, lendo a matriz lampadas[tecla][coluna].
   - Portão e iluminação nunca atuam juntos na mesma tecla.

   -------------------------------------------------------------------------------------------------
   PROTEÇÕES IMPLEMENTADAS
   -------------------------------------------------------------------------------------------------
   - Bloqueio de varredura na entrada do menu.
   - Cancelamento de piscas antigos.
   - Validação de faixa.
   - EEPROM isolada por função.
   - Separação humano x máquina.
   - Antirruído por tempo.

   -------------------------------------------------------------------------------------------------
   FRASE DO PROJETO
   -------------------------------------------------------------------------------------------------
   "EU VEJO 8. A MÁQUINA VÊ 7. E OS DOIS ENTENDEM."

   =================================================================================================
*/

 
//CODIGO EM USO REAL.
//ATUALIZADO EM 28/07/2025 COM ADIÇÃO DO RECUSRO RAMPA
//RAMPA 100% CORRIGIDA. MELHORAMENTO NO CONTROLE DE LAMPADA1.
//GRAVAR PERCURSO PELO BOTÃO APRENDER
//APAGA EEPROM INDIVIDUAL.
unsigned long lampadas[20][8] = {
    {0, 0, 0, 0, 0, 0, 0, 0}, // Configuração para TECLA1
    {0, 0, 0, 0, 0, 0, 0, 0}, // Configuração para TECLA2
    {0, 0, 0, 0, 0, 0, 0, 0}, // Configuração para TECLA3
    {0, 0, 0, 0, 0, 0, 0, 0}, // Configuração para TECLA4
    {0, 0, 0, 0, 0, 0, 0, 0}, // Configuração para TECLA5
    {0, 0, 0, 0, 0, 0, 0, 0}, // Configuração para TECLA6
    {0, 0, 0, 0, 0, 0, 0, 0}, // Configuração para TECLA7
    {0, 0, 0, 0, 0, 0, 0, 0}, // Configuração para TECLA8
    {0, 0, 0, 0, 0, 0, 0, 0}, // Configuração para TECLA9
    {0, 0, 0, 0, 0, 0, 0, 0}, // Configuração para TECLA10
    {0, 0, 0, 0, 0, 0, 0, 0}, // Configuração para TECLA11
    {0, 0, 0, 0, 0, 0, 0, 0}, // Configuração para TECLA12
    {0, 0, 0, 0, 0, 0, 0, 0}, // Configuração para TECLA13
    {0, 0, 0, 0, 0, 0, 0, 0}, // Configuração para TECLA14
    {0, 0, 0, 0, 0, 0, 0, 0}, // Configuração para TECLA15
    {0, 0, 0, 0, 0, 0, 0, 0}, // Configuração para TECLA16
    {0, 0, 0, 0, 0, 0, 0, 0}, // Configuração para TECLA17
    {0, 0, 0, 0, 0, 0, 0, 0}, // Configuração para TECLA18
    {0, 0, 0, 0, 0, 0, 0, 0}, // Configuração para TECLA19
    {0, 0, 0, 0, 0, 0, 0, 0}, // Configuração para TECLA20
};//L1,L2,L3,L4,L5,L6,L7,L8
//===================================================================
  //-----------VARIÁVEIS MENU LAMPADAS--------------------------------
//===================================================================
boolean BT_ESTADO_ATUAL_M_LP = LOW;
boolean BT_ESTADO_ANTERIOR_M_LP = LOW;
unsigned long T_ENTER_MENU = 0;
unsigned long T_CONFIRMAR = 0;
unsigned long T_LIBERA_CLIQUE_VARR = 0;
boolean FLAG_ATRASO_CLIQUE = 0;

boolean ENTER_MODO_MENU = 0;
boolean LIBERA_MODO_MENU = 0;
boolean ENTER_MODO_CONFIRMA = 0;
boolean LIBERA_MODO_CONFIRMA_E_NAV = 0;
boolean MENU_LEBERADO = 0;
boolean CONFIRMADO = 0;


boolean ATIVA_PISCA_MENU = 0;
unsigned long TEMPO_PISCA_MENU = 0;
byte ESTADO_LED_MEU = 0;
int NUM_PISCA_MENU = 0;
int VELOCIDADE_PISCA_MENU = 0;
boolean ESTADO_FINAL_MENU = 0;
int VARREDURA_INDICE = 0;
byte CLIQUE_VARREDURA = 0;
byte BLQ_BOT_APRENDER = 0;
  
int POS_VARREDURA_LAMP = 0;
int POS_VARREDURA_TECLA = 0;
int AMOSTRA_POS_INDICE = 0;
#define BASE_LAMP 300
 boolean AUX_MODO_CONFIRMA = 0;


//========================================================================
//====================================================================


#include <EEPROM.h>

#define BOT_ABRIR A3
#define BOT_FECHAR A4

#define out 7
#define BOT  A2
#define LED  A0
#define ABRIR 5
#define FECHAR 4
#define LAMPADA1 12
#define LAMPADA2 11
#define LAMPADA3 10
#define LAMPADA4 A1
#define LAMPADA5 13
#define LAMPADA6 6
#define LAMPADA7 9
#define LAMPADA8 8

// Variáveis globais
unsigned long DEBOUNCE = 0;
unsigned long DEBOUNCE_L = 0;

unsigned long ATUALIZACAO = 0;
unsigned long ATUALIZACAO_TECLAS = 0;
 unsigned long TEMPO =0;
 int DEBOUNCE_DELAY = 0;
      
int PERCURSO_ABRIR = 0; //ARMAZENA O VALOR DO TEMPO DO PERCURSO PARA ABRIR O PORÃO
int PERCURSO_REV_ABRIR = 0;
int PERCURSO_REV_FECHAR = 0;
int COMPARA_PERCURSO_ABRIR = 0;
 
int PERCURSO_FECHAR = 0; //ARMAZENA O VALOR DO TEMPO DO PERCURSO PARA FECHAR O PORÃO
bool FIXA_DESACELARACAO = 0;
int TAMANHO_RAMPA = 0;//GUARDA O TAMANHO DA RAMPA E AUXILIA NA 
                       //NA SEPARAÇÃO DE GRAVAÇÃO E APLICAÇÃO DA RAMPA

boolean TIMER_RAMPA = 0;
static int DESACELARA = 0;

int COMPARA_PERCURSO_FECHAR = 0;

int APRENDER = 0;
byte ALTERNAR = 0;//FAZ O STATUS DE ABERTO OU FECHADO
boolean GRAVAR_PERCURSO_ABIR = 0;
boolean GRAVAR_PERCURSO_FECHAR = 0;
//int RETARDO = 0;
boolean NEXT = 0;
boolean NEXT1 = 0;
boolean NEXT2 = 0;
boolean NEXT3 = 0;
boolean RAMPA_RV_ABRIR = 0;

long INICIO_RTD  = 0;
long INICIO_RTD1 = 0;
long INICIO_RTD2 = 0;
long INICIO_RTD3 = 0;


unsigned long TECLA1,TECLA2,TECLA3,TECLA4,TECLA5,TECLA6,TECLA7,TECLA8,TECLA9,TECLA10,TECLA11,TECLA12,TECLA13,TECLA14,TECLA15,TECLA16,
              TECLA17,TECLA18,TECLA19,TECLA20;//ARMAZENA TECLAS

int ENDERECO =0 ; 
boolean COMANDO_RF = 0;
boolean MODO_APRENDER = 0;
static bool ativo = true; 
int LIGA_DESL = 1;
long TEMPO_CLEAR = 0; 

boolean BT_ESTADO_ATUAL = HIGH;
boolean BT_ESTADO_ANTERIOR = HIGH;
//======================================================
//VARIÁVEIS PARA VERIFICAÇÃO DE LIGAR AS LAMPADAS E ABRIR/FECHAR O PORTÃO, TODAS ZERADA, ABRE PORTÃO COM O COMANDO DE ABRIR/FECHAR
// SENDO UM, LIGA A RESPECTIVA LAMPADA SETADA NA LÓGICA
boolean LAMPADA1_PORTAO = 0;
boolean LAMPADA2_PORTAO = 0;
boolean LAMPADA3_PORTAO = 0;
boolean LAMPADA4_PORTAO = 0;
boolean LAMPADA5_PORTAO = 0;
boolean LAMPADA6_PORTAO = 0;
boolean LAMPADA7_PORTAO = 0;
boolean LAMPADA8_PORTAO = 0;
//================================
boolean STATUS_ABERTO   = 0; //ARMAZENA VALOR DE INDICAÇÃO DE PORTÃO ABERTO, LED ST FICA PISCANDO DIRETO
boolean STATUS          = 0;//ARMAZENA VALOR DE INDICAÇÃO DE PORTÃO FECHADO, LED ST FICA LIGADO DIRETO

boolean DISPARO_TEMPO_FC = 0; // AO MANDAR FECHAR PORTÃO, DISPARA TEMPO PARA COMPARAR COM O PERCURSO SALVO E FAZER PARADA AUTOMÁTICA
boolean DISPARO_TEMPO_AB = 0;// AO MANDAR ABRIR PORTÃO, DISPARA TEMPO PARA COMPARAR COM O PERCURSO SALVO E FAZER PARADA AUTOMÁTICA
//===========================================================
//VARIÁVEIS PARA VERIFICAÇÃO DE STATUS DAS LAMPDAS

boolean LIGA_DESL_L1 =0;
boolean LIGA_DESL_L2 =0;
boolean LIGA_DESL_L3 =0;
boolean LIGA_DESL_L4 =0;
boolean LIGA_DESL_L5 =0;
boolean LIGA_DESL_L6 =0;
boolean LIGA_DESL_L7 =0;
boolean LIGA_DESL_L8 =0;
//=================================================
boolean ABRIR_PORTAO = 0; 
boolean FECHAR_PORTAO = 0;
boolean LAMPADA1_OP2 = 0; //ATIVA OPÃO 2 DA LAMPADA1, PARA SINALIZAÇÕES DO SITEMA
boolean LIGADA_CONTROLE = 0;
boolean L_LIGADA_C = 0;
long TEMPO_APAGA_L = 0;
bool APAGA_AB = 0;
byte REV_ABRIR = 0;
 boolean TRAVA1 = 0;
byte REV_FECHAR = 0;
 boolean TRAVA = 0;
//int PISCA_ATERIOR = -1;
//bool piscando = false;
//int piscaAnterior = -1;



int NUM_PISCA = 0;
bool ATIVA_PISCA = 0;
unsigned long TEMPO_PISCA = 0;
int VELOCIDADE_PISCA = 400;
bool ESTADO_LED = 0;
bool ESTADO_FINAL = LOW;

#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();


// --- Variáveis Globais ---
int time  = 0x00,                              //Variável para o tempo de disparo
    angle = 0x0A;                              //Variável para o ângulo de disparo
    float ANGULO = 7.33;


// --- Rotina de Interrupção ---
volatile bool gerarPulso = false;
unsigned long tempoInicio = 0;
bool pulsoEmAndamento = false;
int contador = 0;

void interrupt() {
  gerarPulso = true;
}


void setup() {
   
   APRENDER = EEPROM.read(200);
   ALTERNAR = EEPROM.read(205);
   FIXA_DESACELARACAO = EEPROM.read(208);
   
  Serial.begin(115200);
  mySwitch.enableReceive(0);  // Receiver on interrupt 0 => that is pin #2

   pinMode(ABRIR, OUTPUT);
   pinMode(FECHAR, OUTPUT);
   pinMode(LED, OUTPUT);
   pinMode(out, OUTPUT);
   pinMode(BOT_ABRIR ,INPUT_PULLUP);
   pinMode(BOT_FECHAR,INPUT_PULLUP);
   pinMode(BOT,INPUT_PULLUP);
   pinMode(LAMPADA1, OUTPUT);
   pinMode(LAMPADA2, OUTPUT);
   pinMode(LAMPADA3, OUTPUT);
   pinMode(LAMPADA4, OUTPUT);
   pinMode(LAMPADA5, OUTPUT);
   pinMode(LAMPADA6, OUTPUT);
   pinMode(LAMPADA7, OUTPUT);
   pinMode(LAMPADA8, OUTPUT);

   

    for(byte T = 0; T < 20; T++){
  carregarTecla(T);
}

}

void loop() { 
 BOTAO_MENU();
if(BLQ_BOT_APRENDER == 0 ){
   EFEITO_CONF_APRENDER();
  }
if(BLQ_BOT_APRENDER == 1 ){
  EFEITO_CONF_MENU();
}
 PORTAO();     
 FIM_DE_AUTOMATICO() ;
 BOTOES();
 APAGAR_EEPORM(TEMPO_CLEAR );
 

static unsigned long DBOUNCE_RECEIVER_RF = 0;
if(millis()-DBOUNCE_RECEIVER_RF > 150){//PARA UM RESPONSIVIDADE BOA COM OS CONTROLES
DBOUNCE_RECEIVER_RF = millis();
CONTROLE_RF();
  
}

//delay(300);
 if (LIGA_DESL == 0 && LAMPADA1_PORTAO == 1) {
    if(millis()-DEBOUNCE_L>400){
      DEBOUNCE_L = millis();   
      LIGA_DESL_L1 ^=1; // Inverte o estado
       
      if(LIGA_DESL_L1 == HIGH){
        LIGADA_CONTROLE = 1;
      }
      else{ LIGADA_CONTROLE = 0;}
}
 }
if( LIGADA_CONTROLE == HIGH){//SE LAMPADA1 ESTIVER LIGADA PELO CONTROLE, PORTÃO NAO APAGA ELA AO FECHAR

    L_LIGADA_C  = 1;
  }
  else{  L_LIGADA_C = 0;}

       
 
    if (LIGA_DESL == 0 && LAMPADA2_PORTAO == 1) {
    if(millis()-DEBOUNCE_L>400){
      DEBOUNCE_L = millis();   
      LIGA_DESL_L2 ^=1; // Inverte o estado      
}
 }

  if (LIGA_DESL == 0 && LAMPADA3_PORTAO == 1) {
    if(millis()-DEBOUNCE_L>400){
      DEBOUNCE_L = millis();   
      LIGA_DESL_L3 ^=1; // Inverte o estado      
}
 }

  if (LIGA_DESL == 0 && LAMPADA4_PORTAO == 1) {
    if(millis()-DEBOUNCE_L>400){
      DEBOUNCE_L = millis();   
      LIGA_DESL_L4 ^=1; // Inverte o estado      
}
 }

  if (LIGA_DESL == 0 && LAMPADA5_PORTAO == 1) {
    if(millis()-DEBOUNCE_L>400){
      DEBOUNCE_L = millis();   
      LIGA_DESL_L5 ^=1; // Inverte o estado      
}
 }

  if (LIGA_DESL == 0 && LAMPADA6_PORTAO == 1) {
    if(millis()-DEBOUNCE_L>400){
      DEBOUNCE_L = millis();   
      LIGA_DESL_L6 ^=1; // Inverte o estado      
}
 }

  if (LIGA_DESL == 0 && LAMPADA7_PORTAO == 1) {
    if(millis()-DEBOUNCE_L>400){
      DEBOUNCE_L = millis();   
      LIGA_DESL_L7 ^=1; // Inverte o estado      
}
 }

  if (LIGA_DESL == 0 && LAMPADA8_PORTAO == 1) {
    if(millis()-DEBOUNCE_L>400){
      DEBOUNCE_L = millis();   
      LIGA_DESL_L8 ^=1; // Inverte o estado      
}
 }

  if(millis()- ATUALIZACAO_TECLAS > 200){
   ATUALIZACAO_TECLAS = millis();
   COMANDO_CONTROLES(COMANDO_RF);  

  } 

  
 digitalWrite(ABRIR,ABRIR_PORTAO);
 digitalWrite(FECHAR,FECHAR_PORTAO);
 digitalWrite(LED,STATUS); 
 digitalWrite(LAMPADA2,LIGA_DESL_L2);
 digitalWrite(LAMPADA8,LIGA_DESL_L8);
  digitalWrite(LAMPADA7,0);
 if( LAMPADA1_OP2 == 1){digitalWrite(LAMPADA1,LIGA_DESL_L1);}
 
  
   
   //BLUETOOT();
   
if(millis()-ATUALIZACAO >400 ){ 
  ATUALIZACAO = millis();
  //EFEITO_CONF_APRENDER();
   /*
   Serial.print("TECLA1=");
   Serial.println(TECLA1);
    
   Serial.print("TECLA2=");
   Serial.println(TECLA2);
    
   Serial.print("TECLA3=");
   Serial.println(TECLA3);

   Serial.print("TECLA4=");
   Serial.println(TECLA4);


   Serial.print("TECLA5=");
   Serial.println(TECLA5);

   Serial.print("TECLA6=");
   Serial.println(TECLA6);

   Serial.print("TECLA4=");
   Serial.println(TECLA4);

   Serial.print("TECLA7=");
   Serial.println(TECLA7);
   Serial.print("TECLA8=");
   Serial.println(TECLA8);
  
  Serial.print("LIGA_DESL_L1=");
  Serial.println(LIGA_DESL_L1);
  Serial.print("LIGA_DESL_L2=");
  Serial.println(LIGA_DESL_L2);
 

   Serial.print(" DISPARO_TEMPO_FC=");
  Serial.println(DISPARO_TEMPO_FC );
  Serial.print(" DISPARO_TEMPO_AB=");
  Serial.println(DISPARO_TEMPO_AB );

  Serial.print("ALTERNAR=");
  Serial.println(ALTERNAR);

   Serial.print(" ABRIR_PORTAO=");
  Serial.println( ABRIR_PORTAO);
   Serial.print(" FECHAR_PORTAO=");
  Serial.println( FECHAR_PORTAO);
  


  


  Serial.print("LIGA_DESL_L1=");
  Serial.println(LIGA_DESL_L1);
  Serial.print("LIGA_DESL_L2=");
  Serial.println(LIGA_DESL_L2);
  Serial.print("LAMPADA1_OP2 =");
  Serial.println(LAMPADA1_OP2 );

  Serial.print(  "APRENDER =");
  Serial.println( APRENDER );
  Serial.print(  "MODO_APRENDER =");
  Serial.println( MODO_APRENDER );
  //STATUS PORTÃO 
   
    Serial.print("LIGA_DESL_L1=");
    Serial.println(LIGA_DESL_L1);
    Serial.print("LIGA_DESL_L2=");
    Serial.println(LIGA_DESL_L2);


    Serial.print("LAMPADA1_PORTAO=");
    Serial.println(LAMPADA1_PORTAO);
    Serial.print("LAMPADA2_PORTAO=");
    Serial.println(LAMPADA2_PORTAO);
    Serial.print("LAMPADA3_PORTAO=");
    Serial.println(LAMPADA3_PORTAO);
    Serial.print("RETARDO=");
    Serial.println(RETARDO);
   */

// Serial.print("LIGA_DESL_L1=");
// Serial.println(LIGA_DESL_L1);
// Serial.print("LIGA_DESL_L2=");
 //Serial.println(LIGA_DESL_L2);

 //Serial.print("TECLA9 =");
 //Serial.println(TECLA9);
 
 //Serial.print("TECLA9 =");
 //Serial.println(TECLA9);

 //Serial.print("TECLA10 =");
 //Serial.println(TECLA10);
 
 //Serial.print("TECLA11 =");
 //Serial.println(TECLA11);
 
 //Serial.print("POS_VARREDURA_LAMP =");
 //Serial.println(POS_VARREDURA_LAMP  );
 //Serial.print(" VARREDURA_INDICE=");
 //Serial.println( VARREDURA_INDICE);
 
 
 //Serial.print("POS_VARREDURA_TECLA=");
// Serial.println(POS_VARREDURA_TECLA);
// Serial.print(  "APRENDER=");
 //Serial.println( APRENDER);
 
 
 //Serial.print(" AMOSTRA_POS_INDICE =");
 //Serial.println( AMOSTRA_POS_INDICE );
  
 //Serial.print("PERCURSO_REV_FECHAR=");
//Serial.println(PERCURSO_REV_FECHAR);

 //Serial.print("PERCURSO_REV_ABRIR=");
 //Serial.println(PERCURSO_REV_ABRIR);
  
   
  //Serial.print("POS_VARREDURA_TECLA=");
  //Serial.println(POS_VARREDURA_TECLA);
 // Serial.print("LIGA_DESL_L2=");
 // Serial.println(LIGA_DESL_L2);
 //Serial.print("TAMANHO_RAMPA=");
 //Serial.println(TAMANHO_RAMPA); 

 Serial.print("LIGA_DESL_L1=");
 Serial.println(LIGA_DESL_L1);
 Serial.print("LIGA_DESL_L2=");
 Serial.println(LIGA_DESL_L2);
 Serial.print("LIGA_DESL_L3=");
 Serial.println(LIGA_DESL_L3);
 Serial.print("LIGA_DESL_L4=");
 Serial.println(LIGA_DESL_L4);
 Serial.print("LIGA_DESL_L5=");
 Serial.println(LIGA_DESL_L5);
 Serial.print("LIGA_DESL_L6=");
 Serial.println(LIGA_DESL_L6);
 Serial.print("LIGA_DESL_L7=");
 Serial.println(LIGA_DESL_L7);
 Serial.print("LIGA_DESL_L8=");
 Serial.println(LIGA_DESL_L8);


 
 //Serial.print(  "MODO_APRENDER =");
// Serial.println( MODO_APRENDER );

 //Serial.print(" VARREDURA_INDICE=");
 //Serial.println( VARREDURA_INDICE);
 
 
 //Serial.print("POS_VARREDURA_LAMP =");
// Serial.println(POS_VARREDURA_LAMP  );
  
 //Serial.print("CLIQUE_VARREDURA =");
 //Serial.println(CLIQUE_VARREDURA );
 
 //Serial.print("ENTER_MODO_MENU =");
 //Serial.println(ENTER_MODO_MENU);

 //.print("ENTER_MODO_CONFIRMA =");
 //Serial.println(ENTER_MODO_CONFIRMA);

 if(MENU_LEBERADO==1){
 Serial.println("OK!, MENU_LEBERADO, ESCOLHA A LÂMPADA E CONFIRME!");

 }
 if(CONFIRMADO ==1){
 Serial.println("OK! CONFIRMADO...");
 
 }

 Serial.print("PERCURSO_REV_FECHAR=");
 Serial.println(PERCURSO_REV_FECHAR);

 Serial.print("PERCURSO_REV_ABRIR=");
 Serial.println(PERCURSO_REV_ABRIR);
  
 Serial.print(" COMPARA_PERCURSO_FECHAR=");
 Serial.println( COMPARA_PERCURSO_FECHAR);
 Serial.print(" COMPARA_PERCURSO_ABRIR=");
 Serial.println( COMPARA_PERCURSO_ABRIR);
 Serial.print(  "PERCURSO_ABRIR =");
 Serial.println( PERCURSO_ABRIR );
 Serial.print(  "PERCURSO_FECHAR =");
 Serial.println( PERCURSO_FECHAR );
 Serial.print("TAMANHO_RAMPA=");
 Serial.println(TAMANHO_RAMPA); 


 
 if(ALTERNAR == 1){ 
 STATUS ^= 1; 
 }
  else{ STATUS= 1; }  
}
     
    ENDERECO = 1;
    EEPROM.get(ENDERECO ,TECLA1);              // Grava TECLA1 nos endereços 1, 2, 3 e 4
    
    EEPROM.get(ENDERECO + 4, TECLA2);          // Grava TECLA2 nos endereços 1, 2, 3 e 4
    
    EEPROM.get(ENDERECO + 8, TECLA3);          // Grava TECLA3 nos endereços 1, 2, 3 e 4
    
    EEPROM.get(ENDERECO +12, TECLA4);          // Grava TECLA4 nos endereços 1, 2, 3 e 4

    EEPROM.get(ENDERECO +16, TECLA5);          // Grava TECLA4 nos endereços 1, 2, 3 e 4
    
    EEPROM.get(ENDERECO +20, TECLA6);          // Grava TECLA4 nos endereços 1, 2, 3 e 4
    
    EEPROM.get(ENDERECO +24, TECLA7);          // Grava TECLA4 nos endereços 1, 2, 3 e 4
    
    EEPROM.get(ENDERECO +28, TECLA8);          // Grava TECLA4 nos endereços 1, 2, 3 e 4

    EEPROM.get(ENDERECO +32, TECLA9);          // Grava TECLA4 nos endereços 1, 2, 3 e 4

    EEPROM.get(ENDERECO +36, TECLA10);          // Grava TECLA4 nos endereços 1, 2, 3 e 4

    EEPROM.get(ENDERECO +40, TECLA11);          // Grava TECLA4 nos endereços 1, 2, 3 e 4
    
    EEPROM.get(ENDERECO +44, TECLA12);          // Grava TECLA4 nos endereços 1, 2, 3 e 4

    EEPROM.get(ENDERECO +48, TECLA13);          // Grava TECLA4 nos endereços 1, 2, 3 e 4

    EEPROM.get(ENDERECO +52, TECLA14);          // Grava TECLA4 nos endereços 1, 2, 3 e 4
    
    EEPROM.get(ENDERECO +56, TECLA15);          // Grava TECLA4 nos endereços 1, 2, 3 e 4
    
    EEPROM.get(ENDERECO +60, TECLA16);          // Grava TECLA4 nos endereços 1, 2, 3 e 4
    
    EEPROM.get(ENDERECO +64, TECLA17);          // Grava TECLA4 nos endereços 1, 2, 3 e 4
    
    EEPROM.get(ENDERECO +68, TECLA18);          // Grava TECLA4 nos endereços 1, 2, 3 e 4
   
    EEPROM.get(ENDERECO +72, TECLA19);          // Grava TECLA4 nos endereços 1, 2, 3 e 4
    
    EEPROM.get(ENDERECO +76, TECLA20);          // Grava TECLA4 nos endereços 1, 2, 3 e 4


unsigned long VALOR_APRENDER[]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
unsigned long TECLAS[] = { TECLA1,TECLA2,TECLA3,TECLA4,TECLA5,TECLA6,TECLA7,TECLA8,TECLA9,TECLA10,TECLA11,TECLA12,
                           TECLA13,TECLA14,TECLA15,TECLA16,TECLA17,TECLA18,TECLA19,TECLA20};


   if (TEMPO_CLEAR < 1) {

 // if (APRENDER < 1 || APRENDER > 20) {
   // // nada
  //} else {

    static int ultimoAprender = -1;

    for (int i = 0; i < 20; i++) {
      if (MODO_APRENDER == 1 &&
          APRENDER == VALOR_APRENDER[i] &&
          APRENDER != ultimoAprender) {

        ultimoAprender = APRENDER;
        FEEDBACK_APRENDER(1, 200, APRENDER);    
        ESTADO_LED = 0;
        TEMPO_PISCA = millis();

        ESTADO_FINAL = (TECLAS[i] == 0) ? HIGH : LOW;

        if (TECLAS[i] > 0) MODO_APRENDER = 0;
      }
    }
  }
}

   
 
      

void CONTROLE_TRIAC() {
  // Calcula o tempo de atraso com base no ângulo
  angle = 20;  // valor de base (ajustável)
  time = (angle * ANGULO) / 180;

  // Converte o tempo para microssegundos
  unsigned long tempoPulso = (unsigned long)(time) * 2000; // 2ms por unidade de time

  // Aguarda o tempo usando micros() (sem travar o sistema inteiro)
  unsigned long inicio = micros();
  while (micros() - inicio < tempoPulso) {
    // Espera precisa sem bloquear o sistema
  }

  // Gera pulso no pino 'out'
  digitalWrite(out, HIGH);
  delayMicroseconds(1000); // 1ms de pulso
  digitalWrite(out, LOW);
}
void CONTROLE_RF(){  
  if (mySwitch.available()) {
 if(MODO_APRENDER==1){
  if (APRENDER == 1) {
 limparPerfilPortao();

      MODO_APRENDER = 0;
   digitalWrite(LAMPADA1,LOW);
   // APRENDER = 0;   
    TECLA1 = mySwitch.getReceivedValue();  // Recebe o valor do controle
    ENDERECO = 1;                          // Começa no endereço 1
    EEPROM.put(ENDERECO, TECLA1);          // Grava TECLA1 nos endereços 1, 2, 3 e 4
                          // Atualiza ENDERECO para o próximo bloco (4 bytes)
}

if (APRENDER == 2) {
 limparPerfilPortao();

   MODO_APRENDER = 0;
   digitalWrite(LAMPADA1,LOW);
   
    //APRENDER = 0;       
    TECLA2 = mySwitch.getReceivedValue();  // Recebe o valor de outra tecla
    EEPROM.put(ENDERECO + 4, TECLA2);          // Grava TECLA2 no próximo bloco de memória
                       // Atualiza ENDERECO para o próximo bloco (4 bytes)
}

if (APRENDER == 3) {

  limparPerfilPortao();   
   MODO_APRENDER = 0;
   digitalWrite(LAMPADA1,LOW);
  // APRENDER = 0;      
   TECLA3 = mySwitch.getReceivedValue();  // Recebe o valor de outra tecla
   EEPROM.put(ENDERECO +8, TECLA3);          // Grava TECLA3 no próximo bloco de memória
                         // Atualiza ENDERECO para o próximo bloco (4 bytes)
}

if (APRENDER == 4) {
   limparPerfilPortao();

  MODO_APRENDER = 0;
  digitalWrite(LAMPADA1,LOW);
  // APRENDER = 0;     
   TECLA4 = mySwitch.getReceivedValue();  // Recebe o valor de outra tecla
   EEPROM.put(ENDERECO + 12, TECLA4);          // Grava TECLA4 no próximo bloco de memória
                          // Atualiza ENDERECO para o próximo bloco (4 bytes)
}
if (APRENDER == 5) {
   limparPerfilPortao();

  MODO_APRENDER = 0;
  digitalWrite(LAMPADA1,LOW);
  // APRENDER = 0;     
   TECLA5 = mySwitch.getReceivedValue();  // Recebe o valor de outra tecla
   EEPROM.put(ENDERECO + 16, TECLA5);          // Grava TECLA4 no próximo bloco de memória
                          // Atualiza ENDERECO para o próximo bloco (4 bytes)
}

if (APRENDER == 6) {
   limparPerfilPortao();

  MODO_APRENDER = 0;
  digitalWrite(LAMPADA1,LOW);
  // APRENDER = 0;     
   TECLA6 = mySwitch.getReceivedValue();  // Recebe o valor de outra tecla
   EEPROM.put(ENDERECO + 20, TECLA6);          // Grava TECLA4 no próximo bloco de memória
                          // Atualiza ENDERECO para o próximo bloco (4 bytes)
}

if (APRENDER == 7) {
   limparPerfilPortao();

  MODO_APRENDER = 0;
  digitalWrite(LAMPADA1,LOW);
  // APRENDER = 0;     
   TECLA7 = mySwitch.getReceivedValue();  // Recebe o valor de outra tecla
   EEPROM.put(ENDERECO + 24, TECLA7);          // Grava TECLA4 no próximo bloco de memória
                          // Atualiza ENDERECO para o próximo bloco (4 bytes)
}

if (APRENDER == 8) {
   limparPerfilPortao();

  MODO_APRENDER = 0;
  digitalWrite(LAMPADA1,LOW);
  // APRENDER = 0;     
   TECLA8 = mySwitch.getReceivedValue();  // Recebe o valor de outra tecla
   EEPROM.put(ENDERECO + 28, TECLA8);          // Grava TECLA4 no próximo bloco de memória
                          // Atualiza ENDERECO para o próximo bloco (4 bytes)
}

if (APRENDER == 9) {
   limparPerfilPortao();

  MODO_APRENDER = 0;
  digitalWrite(LAMPADA1,LOW);
  // APRENDER = 0;     
   TECLA9 = mySwitch.getReceivedValue();  // Recebe o valor de outra tecla
   EEPROM.put(ENDERECO + 32, TECLA9);          // Grava TECLA4 no próximo bloco de memória
                          // Atualiza ENDERECO para o próximo bloco (4 bytes)
}

if (APRENDER == 10) {
   limparPerfilPortao();

  MODO_APRENDER = 0;
  digitalWrite(LAMPADA1,LOW);
  // APRENDER = 0;     
   TECLA10 = mySwitch.getReceivedValue();  // Recebe o valor de outra tecla
   EEPROM.put(ENDERECO + 36, TECLA10);          // Grava TECLA4 no próximo bloco de memória
                          // Atualiza ENDERECO para o próximo bloco (4 bytes)
}

if (APRENDER == 11) {
   limparPerfilPortao();

  MODO_APRENDER = 0;
  digitalWrite(LAMPADA1,LOW);
  // APRENDER = 0;     
   TECLA11 = mySwitch.getReceivedValue();  // Recebe o valor de outra tecla
   EEPROM.put(ENDERECO + 40, TECLA11);          // Grava TECLA4 no próximo bloco de memória
                          // Atualiza ENDERECO para o próximo bloco (4 bytes)
}

if (APRENDER == 12) {
   limparPerfilPortao();

  MODO_APRENDER = 0;
  digitalWrite(LAMPADA1,LOW);
  // APRENDER = 0;     
   TECLA12 = mySwitch.getReceivedValue();  // Recebe o valor de outra tecla
   EEPROM.put(ENDERECO + 44, TECLA12);          // Grava TECLA4 no próximo bloco de memória
                          // Atualiza ENDERECO para o próximo bloco (4 bytes)
}

if (APRENDER == 13) {
   limparPerfilPortao();

  MODO_APRENDER = 0;
  digitalWrite(LAMPADA1,LOW);
  // APRENDER = 0;     
   TECLA13 = mySwitch.getReceivedValue();  // Recebe o valor de outra tecla
   EEPROM.put(ENDERECO + 48, TECLA13);          // Grava TECLA4 no próximo bloco de memória
                          // Atualiza ENDERECO para o próximo bloco (4 bytes)
}
if (APRENDER == 14) {
   limparPerfilPortao();

  MODO_APRENDER = 0;
  digitalWrite(LAMPADA1,LOW);
  // APRENDER = 0;     
   TECLA14 = mySwitch.getReceivedValue();  // Recebe o valor de outra tecla
   EEPROM.put(ENDERECO + 52, TECLA14);          // Grava TECLA4 no próximo bloco de memória
                          // Atualiza ENDERECO para o próximo bloco (4 bytes)
}

if (APRENDER == 15) {
   limparPerfilPortao();

  MODO_APRENDER = 0;
  digitalWrite(LAMPADA1,LOW);
  // APRENDER = 0;     
   TECLA15 = mySwitch.getReceivedValue();  // Recebe o valor de outra tecla
   EEPROM.put(ENDERECO + 56, TECLA15);          // Grava TECLA4 no próximo bloco de memória
                          // Atualiza ENDERECO para o próximo bloco (4 bytes)
}

if (APRENDER == 16) {
   limparPerfilPortao();

  MODO_APRENDER = 0;
  digitalWrite(LAMPADA1,LOW);
  // APRENDER = 0;     
   TECLA16 = mySwitch.getReceivedValue();  // Recebe o valor de outra tecla
   EEPROM.put(ENDERECO + 60, TECLA16);          // Grava TECLA4 no próximo bloco de memória
                          // Atualiza ENDERECO para o próximo bloco (4 bytes)
}

if (APRENDER == 17) {
   limparPerfilPortao();

  MODO_APRENDER = 0;
  digitalWrite(LAMPADA1,LOW);
  // APRENDER = 0;     
   TECLA17 = mySwitch.getReceivedValue();  // Recebe o valor de outra tecla
   EEPROM.put(ENDERECO + 64, TECLA17);          // Grava TECLA4 no próximo bloco de memória
                          // Atualiza ENDERECO para o próximo bloco (4 bytes)
}


if (APRENDER == 18) {
   limparPerfilPortao();

  MODO_APRENDER = 0;
  digitalWrite(LAMPADA1,LOW);
  // APRENDER = 0;     
   TECLA18 = mySwitch.getReceivedValue();  // Recebe o valor de outra tecla
   EEPROM.put(ENDERECO + 68, TECLA18);          // Grava TECLA4 no próximo bloco de memória
                          // Atualiza ENDERECO para o próximo bloco (4 bytes)
}

if (APRENDER == 19) {
   limparPerfilPortao();

  MODO_APRENDER = 0;
  digitalWrite(LAMPADA1,LOW);
  // APRENDER = 0;     
   TECLA19 = mySwitch.getReceivedValue();  // Recebe o valor de outra tecla
   EEPROM.put(ENDERECO + 72, TECLA19);          // Grava TECLA4 no próximo bloco de memória
                          // Atualiza ENDERECO para o próximo bloco (4 bytes)
}

if (APRENDER == 20) {
   limparPerfilPortao();

  MODO_APRENDER = 0;
  digitalWrite(LAMPADA1,LOW);
  // APRENDER = 0;     
   TECLA20 = mySwitch.getReceivedValue();  // Recebe o valor de outra tecla
   EEPROM.put(ENDERECO + 76, TECLA20);          // Grava TECLA4 no próximo bloco de memória
                          // Atualiza ENDERECO para o próximo bloco (4 bytes)
}
 }
    // Mapear o valor recebido para teclas
    if (mySwitch.available()) {
        if (mySwitch.getReceivedValue() == TECLA1) {
           configurarLuz(0);
        } else if (mySwitch.getReceivedValue() == TECLA2) {
             configurarLuz(1);
        } else if (mySwitch.getReceivedValue() == TECLA3) {
            configurarLuz(2);
        } else if (mySwitch.getReceivedValue() == TECLA4) {
            configurarLuz(3);
        } else if (mySwitch.getReceivedValue() == TECLA5) {
            configurarLuz(4);
        } else if (mySwitch.getReceivedValue() == TECLA6) {
            configurarLuz(5);
        } else if (mySwitch.getReceivedValue() == TECLA7) {
            configurarLuz(6);
        } else if (mySwitch.getReceivedValue() == TECLA8) {
            configurarLuz(7);
        } else if (mySwitch.getReceivedValue() == TECLA9) {
            configurarLuz(8);
        } else if (mySwitch.getReceivedValue() == TECLA10) {
            configurarLuz(9);
        }  else if (mySwitch.getReceivedValue() == TECLA11) {
            configurarLuz(10);
        } else if (mySwitch.getReceivedValue() == TECLA12) {
            configurarLuz(11);
        } else if (mySwitch.getReceivedValue() == TECLA13) {
            configurarLuz(12);
        } else if (mySwitch.getReceivedValue() == TECLA14) {
            configurarLuz(13);
        } else if (mySwitch.getReceivedValue() == TECLA15) {
            configurarLuz(14);
        } else if (mySwitch.getReceivedValue() == TECLA16) {
            configurarLuz(15);
        } else if (mySwitch.getReceivedValue() == TECLA17) {
            configurarLuz(16);
        } else if (mySwitch.getReceivedValue() == TECLA18) {
            configurarLuz(17);
        } else if (mySwitch.getReceivedValue() == TECLA19) {
            configurarLuz(18);
        } else if (mySwitch.getReceivedValue() == TECLA20) {
            configurarLuz(19);
        }
           
        
 }

 
   mySwitch.resetAvailable();

  
  }
    }

    void configurarLuz(int tecla) {
    COMANDO_RF = 1;
    LIGA_DESL = 0;//gatilho geral de todo os controles
    LAMPADA1_OP2 = 1;//ATIVAR O DIGITALWRITE DA LAMPADA1 PARA FUNCIONAMNETO DE ILUMINAÇÃO
                     // SE LAMPADA1_OP2 FOR 0,LAMPADA1 FICA NA OPÇAO2, QUE É USADA COMO SINALIZAÇÃO
                     // DE CONFIGURAÇÃO E MEMÓRIA.
    for (int i = 0; i < 8; i++) {
        // Atualiza as variáveis de lâmpada dinamicamente
        if (i == 0) LAMPADA1_PORTAO = lampadas[tecla][i];
        if (i == 1) LAMPADA2_PORTAO = lampadas[tecla][i];
        if (i == 2) LAMPADA3_PORTAO = lampadas[tecla][i];
        if (i == 3) LAMPADA4_PORTAO = lampadas[tecla][i];
        if (i == 4) LAMPADA5_PORTAO = lampadas[tecla][i];
        if (i == 5) LAMPADA6_PORTAO = lampadas[tecla][i];
        if (i == 6) LAMPADA7_PORTAO = lampadas[tecla][i];
        if (i == 7) LAMPADA8_PORTAO = lampadas[tecla][i]; 

    }
}      
 void BOTOES(){//GERENCIAMENTO DOS BOTÕES
  int CLICK = 0;


 BT_ESTADO_ATUAL = digitalRead(BOT);

 
 if( BT_ESTADO_ANTERIOR == LOW && BT_ESTADO_ATUAL==HIGH){  
  if(PERCURSO_FECHAR > 1 && PERCURSO_ABRIR > 1){
    MODO_APRENDER = 1;//ENTREADO NO MODO DE GRAVAR CONTROLES
         LAMPADA1_OP2 = 0;
            ativo = true;  
            digitalWrite(LAMPADA1,LOW); 
   if( BLQ_BOT_APRENDER ==0){ //DESPLUGA BTAPRENDER QUANDO ESTIVAR EM MENU  
    APRENDER ++; //AVANÇA PROCESSO DE GRAVAÇÃO DE CONTROLE
    EEPROM.update(200,APRENDER);//MEORIZA ÚLTIMA POSIÇÃO GRAVADA OU VAZIA 
  }  
   }
    }
 BT_ESTADO_ANTERIOR = BT_ESTADO_ATUAL;

  
  if( BT_ESTADO_ANTERIOR == LOW){ // COM VARIÁVEIS DE PERCURSOS VAZIAS, BOTÃO CLICADO, E EM 4 SEG, PROCESSO DE GRVAÇÃO DE
                                  //PERCURSO É DISPARADO ATÉ A GRAVAÇÃO DA RAMPA E FINALIZANDO NO FIM DE CURSO FECHAR.
 
 if( PERCURSO_ABRIR < 1 && ALTERNAR == 0){//DISPARO INICÍCIO DE GRVAÇÃO DE PERCURSO PELO BOTÃO APRNEDER
         NEXT = 1;//ATIVA CONTAGEM DE 4 SEG, PARA DISPARO DE GRAVAÇÃO DE PERCURSO
         INICIO_RTD = millis();
         APRENDER = 0;
 }
    
   CLICK = 1;
  
   BOT_APRENDER(CLICK);
   
    if(BLQ_BOT_APRENDER == 0){delay(400);}
  }
  else{TEMPO_CLEAR = 0; CLICK = 0;}
  
  if(NEXT == 1){ 
    if(millis()- INICIO_RTD > 4000){// 4 SEG, PARA DISPARO DE GRAVAÇÃO DE PERCURSO ABRIR.  
    
          GRAVAR_PERCURSO_ABIR  = 1;//DISPARA PROCESSO DE GRAVAÇÃO
          ABRIR_PORTAO = 1;// LIGA MOTOR PARA FAZER O PERCURSO ABRIR
          FECHAR_PORTAO = 0;           
          NEXT = 0;
    }
   
 }
 
  
if(APRENDER> 20){
  APRENDER = 0;
 
  ATIVA_PISCA = 0;
  NUM_PISCA = 0;
  ESTADO_LED = 0;
  
  digitalWrite(LAMPADA1, LOW);
  EEPROM.update(200,APRENDER);
  MODO_APRENDER = 0;
}
unsigned long tempoAtual = millis();
unsigned long debounceDelay = 800; // Tempo de debounce ajustável
unsigned long ultimoTempoBotao = 0;

  // Verifica se já passou tempo suficiente desde a última ativação
  if (tempoAtual - ultimoTempoBotao > debounceDelay) {
   if(digitalRead(BOT_ABRIR)==LOW){//BOTÃO FIM DE CURSO AO ABRIR O PORTÃO
    ultimoTempoBotao = tempoAtual; // Atualiza o tempo de última ativação
    ALTERNAR = 1;
    EEPROM.update(205,ALTERNAR);
     
    ABRIR_PORTAO = 0;
    GRAVAR_PERCURSO_ABIR  = 0;//CONCLUI GRAVAÇÃO DO PERCURSO ABRIR NA EEPORM, AQUI FINALIZA A GRAVAÇÃO, GARANTINDO O VALOR REAL DO PERCURSO
    
     DISPARO_TEMPO_AB=0;
     PERCURSO_REV_FECHAR = 0;
     PERCURSO_REV_ABRIR = 0;
     if(PERCURSO_FECHAR < 1 && ALTERNAR == 1){// DISPARO AUTOMATICO DE GRAVALÇAO DE PERCURSO FECHAR

        NEXT1 = 1;//ATIVA CONTAGEM DE TEMPO DE 4 SEG, PARA DISPARO DE GRAVAÇÃO DE PERCURSO FECHAR
        INICIO_RTD1 = millis();      
    
  }        
    }
              if(NEXT1 == 1){ 
              if(millis()- INICIO_RTD1 > 4000){// APÓS DETECTADO FIM DE CURSO ABRIR, PORTÃO CONCLUIU ABERTURA,
                                               //  E EM 4 SEGUNDO É DADO O DISPARO AUTOMATICO DA GRAVAÇÃO DE PERCURSO FECHAR.
              INICIO_RTD1 = millis(); 
              GRAVAR_PERCURSO_FECHAR = 1;// INICIO DO DISPARO DE GRAVAÇÃO PERCURSO FECHAR

              FECHAR_PORTAO = 1;// LIGAR MOTOR AO FECHAR NA GRAVAÇÃO DO PERCURSO
              ABRIR_PORTAO = 0; 
              NEXT1 = 0;
              }
                }          
    
  if(digitalRead(BOT_FECHAR)==LOW){//BOTÃO FIM DE CURSO AO FECHAR O PORTÃO
     ultimoTempoBotao = tempoAtual; // Atualiza o tempo de última ativação
     ALTERNAR = 0;// MEMORIZA O STATUS DO PORTÃO FECHADO
    EEPROM.update(205,ALTERNAR);
    
    FECHAR_PORTAO = 0;//AQUI DSLIGA O RELE DE FECHAMENTO DO PORTÃO
    GRAVAR_PERCURSO_FECHAR = 0; //CONCLUSÃO DE GRVAÇÃO DE PERCURSO FECHAR
                                //AUXILIA NA GRAVAÇÃO DO PERCURSO, AQUI FINALIZA A GRAVAÇÃO, GARANTINDO O VALOR REAL DO PERCURSO FECHAR

    COMPARA_PERCURSO_FECHAR = 0;
    EEPROM.put(460,COMPARA_PERCURSO_FECHAR);
    ANGULO = 7.33;
     TIMER_RAMPA = 0;
  
    DISPARO_TEMPO_FC = 0;
    REV_FECHAR = 0;
    PERCURSO_REV_FECHAR = 0;
    PERCURSO_REV_ABRIR = 0;

    if(TAMANHO_RAMPA < 1 && PERCURSO_FECHAR> 1 && FIXA_DESACELARACAO == 0){//DISPARO AUTOMATICO PARA GRAVAÇÃO DA RAMPA DE DESACELERAÇÃO AO FECHAR
     NEXT2 = 1; //PORTÃO FECHADO, E GRAVADO PERCUSO FECHAR, ATIVA CONTAGEM DE 4 SEGUNDO PARA DISPARO AUTOMATICO DE ABERTURA, 
                //PORTÃO ABRE JÁ PARA AUTOMÁTICO, POIS OS PERCURSOS FECHAR E ABRIR JA FORAM CONFIGURADOS. ENTÃO VOLTA AUTOMATICAMENTE
                //PARA CANFIGURAR E GRAVAR O PERIODO DA DESACELERÇÃO NO FINAL DO FECHAMENTO
     
      INICIO_RTD2 = millis();
    }
   
      if(TAMANHO_RAMPA> 1){//SAIR MODO GRAVAÇÃO DE RAMPA PARA O USO PRÁTICO COM RAMPA GRAVADA
    FIXA_DESACELARACAO = 1;
    EEPROM.update(208,FIXA_DESACELARACAO);
}    
  }
    } 
 if( NEXT2 == 1){
    
    if(millis()- INICIO_RTD2 > 4000){//ESPERA DE 4 SEG PARA PORTÃO VOLTAR A ABRIR, APOS A CONCLUSÃO DE PERCURSO FECHAR
                 INICIO_RTD2 = millis(); 
                 DISPARO_TEMPO_AB = 1;
                 ABRIR_PORTAO = 1; // MOTOR  LIGA PARA ABRIR O PORTÃO
                 FECHAR_PORTAO = 0;
                 NEXT2 = 0;
      }
        }
          }


    void COMANDO_CONTROLES(int receber){//FUNÇÃO QUE TRATA O COMANDO DOS CONTROLES
                                        // AO CLICAR NO CONTROLE, EU PRECISO QUE  "LIGA_DESL" SAIA DE 1 PARA 0 E VOLTE PARA 1
                                        //AO PRECIONAR O BOTÃO.
     
        LIGA_DESL = 1;
      
      if( LIGA_DESL == 1){

         COMANDO_RF = 0;
      }
      
     else {LIGA_DESL = 0;}
      
 
      }
     
    void FEEDBACK_APRENDER(boolean ATIVA, int VELOC_PISCA, int QTD_PISCADA){
 ATIVA_PISCA = ATIVA;
 NUM_PISCA = QTD_PISCADA*2;
 
 VELOCIDADE_PISCA = VELOC_PISCA;

 TEMPO_PISCA = millis();
  
 }

 void EFEITO_CONF_APRENDER(){

  if (ATIVA_PISCA) {

    if (millis() - TEMPO_PISCA >= VELOCIDADE_PISCA) {

      TEMPO_PISCA = millis();
      ESTADO_LED = ! ESTADO_LED;
      digitalWrite(LAMPADA1, ESTADO_LED);

      if (NUM_PISCA > 0) NUM_PISCA--;

      if (NUM_PISCA == 0) {
        ATIVA_PISCA = 0;
        TEMPO_PISCA = 0;        
        digitalWrite(LAMPADA1, ESTADO_FINAL);
      }
    }
  }
}

void FEEDBACK_MENU(boolean ATIVA_MENU, int VELOC_PISCA_MENU,int QUANTIDADE_PISCADA,boolean FINAL_LED ){
 
 ATIVA_PISCA_MENU = ATIVA_MENU;
 TEMPO_PISCA_MENU = millis();
 VELOCIDADE_PISCA_MENU = VELOC_PISCA_MENU;
 NUM_PISCA_MENU =  QUANTIDADE_PISCADA * 2 ;
  ESTADO_FINAL_MENU = FINAL_LED;

  
 }
 
void EFEITO_CONF_MENU(){

 if (ATIVA_PISCA_MENU) {
if (millis() - TEMPO_PISCA_MENU >= VELOCIDADE_PISCA_MENU) {

      TEMPO_PISCA_MENU = millis();

      ESTADO_LED_MEU ^= 1;                 // inverte
      digitalWrite(LAMPADA1, ESTADO_LED_MEU);

      if (NUM_PISCA_MENU > 0) NUM_PISCA_MENU--;

      if (NUM_PISCA_MENU == 0) {
        ATIVA_PISCA_MENU = 0;
        digitalWrite(LAMPADA1,  ESTADO_FINAL_MENU);
      }
    }
  }
}
    
void BOTAO_MENU(){ // LOGICA DE ENTRAR E NAVEGAR E CONFIRMAR INTENÇÃO NO MENU

 
  static boolean BT_ESTADO_ANTERIOR_M_LP = HIGH;

  BT_ESTADO_ATUAL_M_LP = digitalRead(BOT);

  // =========================================
  // CONTROLE DO BOTÃO
  // =========================================
  if(BT_ESTADO_ATUAL_M_LP == LOW ){//CLIQUE DE ENTADA DO MENU

    if(LIBERA_MODO_MENU == 0){
      ENTER_MODO_MENU  = 1; //AUTORIZA ENTRADA DO MENU
    }

    if(LIBERA_MODO_CONFIRMA_E_NAV == 1){//AUTORIZA ENTRADA DO BT CLIQUES CURTO DE NAVEGAÇÃO, E ENTRADA PARA CONFIRMAÇÃO
      ENTER_MODO_CONFIRMA = 1;// MODO CONFIRMA AUTORIZADO
    }

  } else {

    ENTER_MODO_MENU  = 0;
    ENTER_MODO_CONFIRMA = 0;
  }
   
  // =========================================
  // CLIQUE CURTO → INCREMENTA AO SOLTAR
  // =========================================
  if (BT_ESTADO_ANTERIOR_M_LP == LOW && BT_ESTADO_ATUAL_M_LP == HIGH && FLAG_ATRASO_CLIQUE == 0) {
  
    if (LIBERA_MODO_CONFIRMA_E_NAV == 1 && MENU_LEBERADO == 1 ) {     
   
    AUX_MODO_CONFIRMA = 0;//LIBERA CLIQUE DE CONFIRMAX
     VARREDURA_INDICE++;

if (VARREDURA_INDICE > 8) VARREDURA_INDICE = 0;

// amostra congelada
AMOSTRA_POS_INDICE = VARREDURA_INDICE;

if (AMOSTRA_POS_INDICE < 1 || AMOSTRA_POS_INDICE > 8) return;
  FEEDBACK_MENU(1, 250, AMOSTRA_POS_INDICE, HIGH);
  

    }
  }
   
    
      if (VARREDURA_INDICE > 8) {
        VARREDURA_INDICE = 0;
         
        FEEDBACK_MENU(0,0,0,HIGH);
      }
  // =========================================
  // ENTRAR NO MENU (4s pressionado)
  // =========================================
  if (ENTER_MODO_MENU == 1 && MODO_APRENDER == 1 ) {

    if (T_ENTER_MENU == 0) {
      T_ENTER_MENU = millis();
    }

    if (millis() - T_ENTER_MENU > 4000) {
      FEEDBACK_APRENDER(0,0,0);
      LIBERA_MODO_MENU = 1;
      
      T_ENTER_MENU = 0;//ZERA TEMPO
      MENU_LEBERADO = 1; //MENU ATIVADO
      BLQ_BOT_APRENDER = 1;//BLOQUEIA FUNÇÃO PRIMÁRIA DO BOTÃO DENTRO DO MENU, LIBERANDO O BOTÃO PARA O MENU
      VARREDURA_INDICE = 0; //ENTRA NO MENU ZERADO PERA ESTABILIDADE DA VARREDURA
      AMOSTRA_POS_INDICE = 0;//MENU ENTRA COM AMSOTRA JA ZERADA PARA ESTABILIDADE DO FEEFBECK
      FLAG_ATRASO_CLIQUE = 1;//INICIA ATRASO PARA VERREDURA NÃO INCIAR NA ENTRADA DO MENU
      T_LIBERA_CLIQUE_VARR = millis();
      digitalWrite(LAMPADA1, LOW);
      AUX_MODO_CONFIRMA = 1;//TRAVAR ENTRADA CORFIRMA NO MESMO CLIQUE DE ENTRADA DE MENU.
    }

  } else {
    T_ENTER_MENU = 0;
  }

if(FLAG_ATRASO_CLIQUE == 1){

if(T_LIBERA_CLIQUE_VARR ==0){
T_LIBERA_CLIQUE_VARR = millis();
  
}

if(millis()-T_LIBERA_CLIQUE_VARR>1500){//EVITAR QUE A SELEÇÃO DO INDICE INICIA NA ENTRADA DO MENU
FLAG_ATRASO_CLIQUE = 0;//LIBERA CLIQUES CURTOS PARA NAVEGAÇÃO NO ARRAY A 1.5S DA ENTRADA DO MENU
LIBERA_MODO_CONFIRMA_E_NAV = 1;//LIBERA A ENTRADA DA NAVEGAÇÃO DO INDICE, E A ENTRADA DE CONFIRMAÇÃO
T_LIBERA_CLIQUE_VARR = 0;//LIBERA O BOTÃO PARA CLIQUES CURTOS NA NAVEGAÇÃO

  
}
  
   }
  // =========================================
  // CONFIRMAR (2s pressionado)
  // =========================================
 
  static boolean JA_CONFIRMOU = 0;
  if(ENTER_MODO_CONFIRMA == 1 && AUX_MODO_CONFIRMA == 0 ){
                                                      //SÓ ENTRAR CONFIRMA SE JA ESTIVAR ALGO SELECIONADO.
    if(T_CONFIRMAR == 0){
      T_CONFIRMAR = millis();
    }

  if(millis() - T_CONFIRMAR > 2000){  
  FEEDBACK_MENU(1,100,4,1);
  CONFIRMADO = 1;
  LIBERA_MODO_CONFIRMA_E_NAV = 0;
  T_CONFIRMAR = 0;
  JA_CONFIRMOU = 1;

  // ==============================
  // ENDEREÇAMENTO
  // ==============================
  POS_VARREDURA_LAMP = AMOSTRA_POS_INDICE;    
  POS_VARREDURA_TECLA = APRENDER;

  if(POS_VARREDURA_TECLA < 1 || POS_VARREDURA_TECLA > 20) return;
  if(POS_VARREDURA_LAMP  < 1 || POS_VARREDURA_LAMP  > 8)  return;

  byte T = POS_VARREDURA_TECLA - 1;

  // ==============================
  // 1) LIMPA TECLA RF
  // ==============================
  int enderecoBase = ENDERECO + (T * 4);
  for (int i = enderecoBase; i < enderecoBase + 4; i++) {
    EEPROM.update(i, 0);
  }

  switch (POS_VARREDURA_TECLA) {
    case 1: TECLA1 = 0UL; break;
    case 2: TECLA2 = 0UL; break;
    case 3: TECLA3 = 0UL; break;
    case 4: TECLA4 = 0UL; break;
    case 5: TECLA5 = 0UL; break;
    case 6: TECLA6 = 0UL; break;
    case 7: TECLA7 = 0UL; break;
    case 8: TECLA8 = 0UL; break;
    case 9: TECLA9 = 0UL; break;
    case 10: TECLA10 = 0UL; break;
    case 11: TECLA11 = 0UL; break;
    case 12: TECLA12 = 0UL; break;
    case 13: TECLA13 = 0UL; break;
    case 14: TECLA14 = 0UL; break;
    case 15: TECLA15 = 0UL; break;
    case 16: TECLA16 = 0UL; break;
    case 17: TECLA17 = 0UL; break;
    case 18: TECLA18 = 0UL; break;
    case 19: TECLA19 = 0UL; break;
    case 20: TECLA20 = 0UL; break;
  }

  // ==============================
  // 2) LIMPA MAPA DA TECLA
  // ==============================
  limparLinha(T);
  EEPROM.update(BASE_LAMP + T, 0);

  // ==============================
  // 3) ESCREVE NOVO BIT
  // ==============================
  lampadas[T][POS_VARREDURA_LAMP - 1] = 1;

  // ==============================
  // 4) SALVA NA EEPROM
  // ==============================
  salvarTecla(T);

 
    }
    
  

  }  else if (ENTER_MODO_CONFIRMA == 0) {
  T_CONFIRMAR = 0;
  JA_CONFIRMOU = 0; // libera para próxima vez
 
}
if(JA_CONFIRMOU ==0 && CONFIRMADO == 1){ digitalWrite(LAMPADA1, HIGH);}//LED FEEDBACK DA CONTINUAÇÃO DO PROCESSO DE GRAVAÇÃO LIGADO
                                                                    // CONTROLE GRAVADO, LED APAGA
  // =========================================
  // ATUALIZA ESTADO ANTERIOR
  // =========================================
  BT_ESTADO_ANTERIOR_M_LP = BT_ESTADO_ATUAL_M_LP;

  
  // =========================================
  // SAIDA MODO GRAVAÇÃO E MENU....
  // =========================================

 if(MODO_APRENDER == 0 && CONFIRMADO == 1){

      MENU_LEBERADO = 0;//FECHA O MENU
      BLQ_BOT_APRENDER = 0;//LIBERA O SISTEMA PARA O USO NORMAL
      CONFIRMADO = 0; //FECHA O CICLO DE CONFIRMAÇÃO
      LIBERA_MODO_MENU = 0;//LIBERA REENTRADA DO MENU
      AUX_MODO_CONFIRMA = 1;//TRAVA CONFIRMAR
     
  }
}
byte packLampadas(byte TECLA){
  byte M = 0;
  for(int I = 0; I < 8; I++){
    if(lampadas[TECLA][I]) M |= (1 << I);
  }
  return M;
}

void salvarTecla(byte TECLA){
  EEPROM.update(BASE_LAMP + TECLA, packLampadas(TECLA));
}

void carregarTecla(byte TECLA){
  byte M = EEPROM.read(BASE_LAMP + TECLA);
  for(int I = 0; I < 8; I++){
    lampadas[TECLA][I] = (M >> I) & 1;
  }
}
void limparLinha(byte T){
  for(byte i = 0; i < 8; i++){
    lampadas[T][i] = 0;
  }
}

void PORTAO() {      
  
    if(PERCURSO_ABRIR < 1 && PERCURSO_FECHAR < 1){//ESTABILZAR PROGRAMA NO PRIMEIRO PERCURSO

   DEBOUNCE_DELAY = 500; ///DEBOUNCE PARA GRAVAÇÃO ESTÁVEL   
                         //ESTABILIZAÇÃO NA GRAVAÇÃO DE PERCURSOS. SEM REPIQUES DE DESLIGAMENTOS
  }
  
  else if(PERCURSO_ABRIR > 50 && PERCURSO_FECHAR >50){

   DEBOUNCE_DELAY = 1; //DEBOUNCE DO USO GERAL
  }
    
    if(millis()-DEBOUNCE>DEBOUNCE_DELAY){
      DEBOUNCE = millis();    
    if (LIGA_DESL == 0 && todasLampadasApagadas()) {
        
        if (ALTERNAR == 1) { // Fechar portão   
            

            if (PERCURSO_FECHAR > 10){// AQUI SÓ É LIBARADO QUANDO PERCURSO FECHAR ESTIVER GRAVADO
           if( MENU_LEBERADO == 0 )   delay(300);
          if(TRAVA == 0){ //CASO CLICAR NOVAMENTE ESTANDO  REV_FECHAR == 3, NÃO CONTA MAIS EVITA AVANÇAR PARA 4 E PORTÃO NÃO PARA;
            REV_FECHAR ++;
          }
           
           if( REV_FECHAR == 1){//FECHAMENTO NORMAL SEM QUE HJA INTERRUPÇÃO
             DISPARO_TEMPO_FC = 1;          
             FECHAR_PORTAO = 1;
             ABRIR_PORTAO = 0;
           }

            if( REV_FECHAR == 2){//PAUSA FECHAMENTO
             DISPARO_TEMPO_FC = 0;
             COMPARA_PERCURSO_FECHAR = 0;
             EEPROM.put(460, COMPARA_PERCURSO_FECHAR);            

             FECHAR_PORTAO = 0;
             ABRIR_PORTAO = 0;
           }

           if( REV_FECHAR == 3){//INVERTE FECHAMENTO
            DISPARO_TEMPO_AB = 1;
             
             FECHAR_PORTAO = 0;
             ABRIR_PORTAO = 1;
              TRAVA = 1;//TRVAR CONTROLE DE ESTAGIO NO ESTGIO 3.
              ANGULO = 7.33;
           }
            
               }         
                         
                   }
        
        else if (ALTERNAR == 0) { // Abrir portão
           
          LIGA_DESL_L1 =1;//LIGA LAMPADA1 AO ABRIR O PORTÃO

            if (PERCURSO_ABRIR > 10){
     if( MENU_LEBERADO == 0 )   delay(300);
              if(TRAVA1== 0 ){REV_ABRIR++;}
              
              if(REV_ABRIR == 1 ){
                DISPARO_TEMPO_AB = 1;
                 ABRIR_PORTAO = 1;
                 FECHAR_PORTAO = 0;

                
              }
          
            
              if(REV_ABRIR == 2 ){
                 DISPARO_TEMPO_AB = 0;
                 ABRIR_PORTAO = 0;
                 FECHAR_PORTAO = 0;
                 COMPARA_PERCURSO_ABRIR = 0;
                 EEPROM.put(450, COMPARA_PERCURSO_ABRIR);

                
              }

                 if(REV_ABRIR == 3){
                
                 DISPARO_TEMPO_FC = 1;
                 ABRIR_PORTAO = 0;
                 FECHAR_PORTAO = 1;
                 TRAVA1 = 1;      
             
           
                    }
                }    
            }
        }
    }
}
bool todasLampadasApagadas() {

    return (LAMPADA1_PORTAO == 0 && LAMPADA2_PORTAO == 0 && 
            LAMPADA3_PORTAO == 0 && LAMPADA4_PORTAO == 0 && 
            LAMPADA5_PORTAO == 0 && LAMPADA6_PORTAO == 0 && 
            LAMPADA7_PORTAO == 0 && LAMPADA8_PORTAO == 0);
}

void FIM_DE_AUTOMATICO() {// FAZ PORTÃO PARAR SEM PRECISAR DE REED SWEET QUANDO O PERCURSO ESTIVER MEMORIZADO                   
    if (millis() - TEMPO > 9) {
        TEMPO = millis();

//============================//PROCESSO DE ABERTURA//=============================================================================
//==================================================================================================================================
        if (DISPARO_TEMPO_AB == 1) {
           
            PERCURSO_REV_ABRIR ++;
           
            COMPARA_PERCURSO_ABRIR++; // FAZ COMPARAÇÃO COM A VARIAVEL DO VALOR DO PERCURSO GRAVADO
            EEPROM.put(450, COMPARA_PERCURSO_ABRIR);
            CONTROLE_TRIAC(); // CHAMAR FUNÇÃO PARA ATIVAR DISPAROS DO TRIAC
        }
         if( REV_FECHAR == 3 && DISPARO_TEMPO_AB == 1){//PERCURSO DE ONDE INICIA ATÉ ONDE PAUSA
           PERCURSO_REV_FECHAR --;//VOLTA DE ONDE PAROU E PARA A ABERTURA DE VOLTA DA PAUSA FECHAR
          
         }

//==============================//PROCESSO DE GRAVAÇÃO DE PRECURSO ABRIR//====================================================
//====================================================================================================================================
        else if (GRAVAR_PERCURSO_ABIR  == 1) {
            CONTROLE_TRIAC(); // CHAMAR FUNÇÃO PARA ATIVAR DISPAROS DO TRIAC
            PERCURSO_ABRIR++; // VARIAVEL QUE ARMAZENA O VALOR DO TAMANHO DO PERCURDO GRAVADO AO ABRIR O PORTAO
            EEPROM.put(550, PERCURSO_ABRIR);
           
        }

//===============================//PROCESSO DE FECHAMENTO DO PORTÃO//=============================================================================
//=====================================================================================================================


        else if (DISPARO_TEMPO_FC == 1) {
            CONTROLE_TRIAC();// CHAMAR FUNÇÃO PARA ATIVAR DISPAROS DO TRIAC
           if(TIMER_RAMPA == 1){
           
            DESACELARA ++;
           
            if(DESACELARA >=3){//DESACELERA CONTADOR COMPARA_PERCURSO_FECHAR, CONFORME DESACELERAÇÃO DO PORTÃO
                               //GARANTE QUE PORTÃO FECHE NORMALMENTE DESACELERADO, SEM BATER FORTE
             PERCURSO_REV_FECHAR ++;// MARCA PERCURSO PARA FAZER PAUSA, E VOLTAR PARA O FINAL DA ABERTURA                   
            COMPARA_PERCURSO_FECHAR++; 
            EEPROM.put(460, COMPARA_PERCURSO_FECHAR);
            DESACELARA = 0;
        }
        
          }
          else{COMPARA_PERCURSO_FECHAR++; // FECHAMENTO NA VELOCIDADE NORMAL
            EEPROM.put(460, COMPARA_PERCURSO_FECHAR);         
             PERCURSO_REV_FECHAR ++;// MARCA PERCURSO PARA FAZER PAUSA, E VOLTAR PARA O FINAL DA ABERTURA
            
            
             }
             
              }
              if(REV_ABRIR == 3 && DISPARO_TEMPO_FC == 1){//ATIVAR REVERSÃO ABRIR
                CONTROLE_TRIAC();// CHAMAR FUNÇÃO PARA ATIVAR DISPAROS DO TRIAC
                ANGULO = 115;//PARA VOLTAR EM RAMPA
                RAMPA_RV_ABRIR = 1;
                PERCURSO_REV_ABRIR--;
                
              }
          
            
//==================//PROCESSO DE GRAVAÇÃO DE PRECURSO FECHAR//====================================================
//==================================================================================================================
      else if ( GRAVAR_PERCURSO_FECHAR == 1){              
            CONTROLE_TRIAC();//CHAMAR FUNÇÃO PARA ATIVAR DISPAROS DO TRIAC
             PERCURSO_FECHAR ++;// VARIAVEL QUE ARMAZENA O VALOR DO TAMANHO DO PERCURDO GRAVADO AO FECHAR O PORTAO
             EEPROM.put(560,PERCURSO_FECHAR);
           }
//===============================================================================================================
        
//=====================================//PROCESSO DE GRAVAÇÃO DA RAMPRA//====================================================== 
 //=============================================================================================================================    
 

          if (FIXA_DESACELARACAO == 0) { // GRAVAÇÃO DA RAMPA
    float LIMITE = PERCURSO_FECHAR * 0.88f; // começa a desacelerar nos últimos 8%

    if (COMPARA_PERCURSO_FECHAR >= LIMITE && COMPARA_PERCURSO_FECHAR < PERCURSO_FECHAR && PERCURSO_FECHAR > 1 && PERCURSO_ABRIR > 1) {

        ANGULO = 115;  // força a desaceleração do motor
        TIMER_RAMPA = 1;
       
        if (TAMANHO_RAMPA == 0) {
            TAMANHO_RAMPA = PERCURSO_FECHAR - COMPARA_PERCURSO_FECHAR;
            EEPROM.put(610, TAMANHO_RAMPA);
        }
           }       
              }       
                 }   

//==============================//PROCESSO DE PARADA AUTOMATICA NA ABERTURA NORMAL//======================================
//========================================================================================================================
    
       if (COMPARA_PERCURSO_ABRIR > PERCURSO_ABRIR && REV_FECHAR == 0) {
        
     
        ALTERNAR = 1;
        EEPROM.update(205, ALTERNAR);
        ABRIR_PORTAO = 0;//PORTÃO ABERTO
        COMPARA_PERCURSO_ABRIR = 0;
        EEPROM.put(450, COMPARA_PERCURSO_ABRIR);
        DISPARO_TEMPO_AB = 0;
        ANGULO = 7.33;
        APAGA_AB   = 1;
        REV_FECHAR = 0;
        REV_ABRIR = 0;
        
        TEMPO_APAGA_L = millis();//INICIAR TEMPO DO 0;
        PERCURSO_REV_FECHAR = 0;
        PERCURSO_REV_ABRIR = 0;
      if (TAMANHO_RAMPA < 1 && FIXA_DESACELARACAO == 0) {//CONDIÇÃO PARA DISPARO DE FECHAMENTO AUTOMATICO PARA A CONFIGURAÇÃO E GRAVAÇÃO DA RAMPA
        NEXT3 = 1;//ATIVA TEMPO DE 4 SEG, PARA DISPARO D FECHAMENTO
       INICIO_RTD3 = millis();
       
      }
    }
        if(NEXT3 == 1){
     if(millis() - INICIO_RTD3  > 4000){// 4 SEG, PORTÃO INCIA AUTOMATICAMENTE O FECHEMENTO
     INICIO_RTD3 = millis();
     DISPARO_TEMPO_FC = 1;//LIGA AUTOMATICAMENTE  O FECHAMNETO PARA ENCONTRAR O PERIODO E GRAVAÇÃO DA RAMPA,
                          //QUE É FINALIZADO NO BOTÃO FIM DE CURSO FECHAR
     FECHAR_PORTAO = 1;//MOTOR LIGA PARA FECHAMENTO DO POTÃO
     NEXT3 = 0;
}
     
    }
  

 //==========PROCESSO DE PARADA AUTOMATICA NA ABERTURA DE VOLTA DA PAUSA FECHAR======================================
 //==================================================================================================================
       

     if (REV_FECHAR == 3 &&  PERCURSO_REV_FECHAR < 1 ) {// PARADA DO PORTÃO NO CICLO DE REVERÃO DE FECHAMENTO
        ALTERNAR = 1;
        EEPROM.update(205, ALTERNAR);
        ABRIR_PORTAO = 0;
        COMPARA_PERCURSO_ABRIR = 0;
        EEPROM.put(450, COMPARA_PERCURSO_ABRIR);
        DISPARO_TEMPO_AB = 0;
        ANGULO = 7.33;
        APAGA_AB   = 1;
        REV_FECHAR = 0;
        REV_ABRIR = 0;
        PERCURSO_REV_FECHAR = 0;
        PERCURSO_REV_ABRIR = 0;
        TRAVA = 0;//CONTROLA ESTAGIO DE ABERTURA OU FECHAMENTO
        
        TEMPO_APAGA_L = millis();//INICIAR TEMPO DO 0;
       
    }
if(L_LIGADA_C == 0){ 

if(APAGA_AB ==1){//INIBIÇÃO DO TEMPORIZADOR, CASO A LAMPADA TENHA SIDO LIGADA PELO USUARIO.
  
        if(millis()-TEMPO_APAGA_L >180000){//APAGA LAMPADA EM 3 MINUTOS CASO SENDO  LIGADA NA ABERTURA DO PORTÃO
          TEMPO_APAGA_L = millis();
          TEMPO_APAGA_L  = 0;
          LIGA_DESL_L1 = 0;
          APAGA_AB  = 0;

}
  }
    }
//==============================//PORCESSO DE APLICAÇÃO DA RAMPA NO CICLO DE FECHAMNETO NORMAL//===========================================
//=========================================================================================================================================   
        if (FIXA_DESACELARACAO == 1) { // APLICAÇÃO DA RAMPA
    
    int LIMITE = PERCURSO_FECHAR - TAMANHO_RAMPA;

    if (COMPARA_PERCURSO_FECHAR >= LIMITE && COMPARA_PERCURSO_FECHAR < PERCURSO_FECHAR && PERCURSO_FECHAR > 1 && PERCURSO_ABRIR > 1) {
        ANGULO = 115;  // ativa a rampa suavemente
        TIMER_RAMPA = 1;
 
    }
     else {
        TIMER_RAMPA = 0;  // desativa se saiu da rampa
    }
//==========================================PROCESSO DE PARADA AUTOMATICA NA FECHAMENTO NORMAL======================================    
//================================================================================================================================ 
        if (COMPARA_PERCURSO_FECHAR > PERCURSO_FECHAR && REV_ABRIR ==0) {//PARADA DO PORTÃO NO CICLO DE FECHAMNETO NORMAL
        ALTERNAR = 0;
        EEPROM.update(205, ALTERNAR);
        FECHAR_PORTAO = 0;
        APAGA_AB  = 0;
       if(L_LIGADA_C == 0){ 
        LIGA_DESL_L1 = 0;
       }
        
        COMPARA_PERCURSO_FECHAR = 0;
        EEPROM.put(460, COMPARA_PERCURSO_FECHAR);
        DISPARO_TEMPO_FC = 0;
        ANGULO = 7.33; // volta ao padrão
        TIMER_RAMPA = 0;
        REV_FECHAR = 0;
        REV_ABRIR = 0;
        PERCURSO_REV_FECHAR = 0;
        PERCURSO_REV_ABRIR = 0;
       
    }
 //==========//PROCESSO DE PARADA AUTOMATICA NO FECHAMENTO DE VOLTA DA PAUSA ABRIR//======================================
 //===================================================================================================================
     if (REV_ABRIR == 3 &&  PERCURSO_REV_ABRIR < 1 ){// PARADA DO PORTÃO NO CICLO DE REVERÃO DE ABERTURA

        ALTERNAR = 0;
        EEPROM.update(205, ALTERNAR);
        FECHAR_PORTAO = 0;
        APAGA_AB  = 0;
       if(L_LIGADA_C == 0){ 
        LIGA_DESL_L1 = 0;
       }
        
        COMPARA_PERCURSO_FECHAR = 0;
        EEPROM.put(460, COMPARA_PERCURSO_FECHAR);
        DISPARO_TEMPO_FC = 0;
        ANGULO = 7.33; // volta ao padrão
        TIMER_RAMPA = 0;
        REV_FECHAR = 0;
        REV_ABRIR = 0;
        PERCURSO_REV_FECHAR = 0;
        PERCURSO_REV_ABRIR = 0;
        TRAVA1 = 0;      
     }
    
}
//==============>RESGATA VALORES DAS CONFIGURAÇÕES NA EEPORM<=============================================
//========================================================================================================
    EEPROM.get(550, PERCURSO_ABRIR);
    EEPROM.get(560, PERCURSO_FECHAR);
    EEPROM.get(450, COMPARA_PERCURSO_ABRIR);
    EEPROM.get(460, COMPARA_PERCURSO_FECHAR);
    EEPROM.get(610, TAMANHO_RAMPA);
}
//================================================================================================================
//================================= DADOS DE CONFIGUÇÃO==========================================================
 void APAGAR_EEPORM(int apagar) {
  if( MODO_APRENDER == 0 &&  MENU_LEBERADO == 0){ //SÓ ENTRA NA FUNÇÃO APAGAR EEPROM SE JA TIVER GRAVADA ESTIVER FORA DO MENU
  if (apagar <= 10 ) return; // Não faz nada se apagar <= 25

  if (APRENDER == 0) {  // Apaga percursos
     for (int i = 450; i < 454; i++) {
      EEPROM.update(i, 0);
    }
    PERCURSO_ABRIR = 0UL;
    APRENDER = 20;
      NEXT = 0;
      ALTERNAR = 0;
      EEPROM.update(205, ALTERNAR);
  //************************************************************* 
    for (int i = 460; i < 464; i++) {
      EEPROM.update(i, 0);
    }
    PERCURSO_ABRIR = 0UL;
    APRENDER = 20;
      NEXT = 0;
      ALTERNAR = 0;
      EEPROM.update(205, ALTERNAR); 

  //*************************************************************
    for (int i = 550; i < 554; i++) {
      EEPROM.update(i, 0);
    }
    PERCURSO_ABRIR = 0UL;
    APRENDER = 20;
      NEXT = 0;
      ALTERNAR = 0;
      EEPROM.update(205, ALTERNAR);
    for (int i = 560; i < 564; i++) {
      EEPROM.update(i, 0);
    }
    PERCURSO_FECHAR = 0UL;
    APRENDER = 20;
    NEXT = 0;
    ALTERNAR = 0;//PARA QUANDO FOR APAGDO OS PERCURSOS, A PLACA JA VOLTA PRONTA PARA UM NOVO CICLO DE GRAVAÇÃO
    EEPROM.update(205, ALTERNAR);
    for (int i = 610; i < 614; i++) {
      EEPROM.update(i, 0);
    }
    TAMANHO_RAMPA = 0UL;
    EEPROM.update(208, 0);
    FIXA_DESACELARACAO = 0UL;
    APRENDER = 20;
    NEXT = 0;
    ALTERNAR = 0;
    EEPROM.update(205, ALTERNAR);
    DISPARO_TEMPO_FC = 0;
    DISPARO_TEMPO_AB = 0;

    // Piscar lento uma vez
    digitalWrite(LAMPADA1, HIGH);
    unsigned long t0 = millis();
    while (millis() - t0 < 400);  // Espera 400 ms sem delay travado
    digitalWrite(LAMPADA1, LOW);

  } else if (APRENDER >= 1 && APRENDER <= 20) {  // Apaga TECLA1 a TECLA20 individualmente
    int enderecoBase = ENDERECO + ((APRENDER - 1) * 4);
    for (int i = enderecoBase; i < enderecoBase + 4; i++) {
      EEPROM.update(i, 0);
    }

    // Atualiza variável correspondente na RAM
    switch (APRENDER) {
      case 1: TECLA1 = 0UL; break;
      case 2: TECLA2 = 0UL; break;
      case 3: TECLA3 = 0UL; break;
      case 4: TECLA4 = 0UL; break;
      case 5: TECLA5 = 0UL; break;
      case 6: TECLA6 = 0UL; break;
      case 7: TECLA7 = 0UL; break;
      case 8: TECLA8 = 0UL; break;
      case 9: TECLA9 = 0UL; break;
      case 10: TECLA10 = 0UL; break;
      case 11: TECLA11 = 0UL; break;
      case 12: TECLA12 = 0UL; break;
      case 13: TECLA13 = 0UL; break;
      case 14: TECLA14 = 0UL; break;
      case 15: TECLA15 = 0UL; break;
      case 16: TECLA16 = 0UL; break;
      case 17: TECLA17 = 0UL; break;
      case 18: TECLA18 = 0UL; break;
      case 19: TECLA19 = 0UL; break;
      case 20: TECLA20 = 0UL; break;
    }

    APRENDER = 20;

    // Piscar lento uma vez
    digitalWrite(LAMPADA1, HIGH);
    unsigned long t1 = millis();
    while (millis() - t1 < 200);  // Espera 400 ms sem delay travado
    digitalWrite(LAMPADA1, LOW);
  }
   }

     }
     void limparPerfilPortao(){
  if(MENU_LEBERADO == 0){
    byte T = APRENDER - 1;
    if(T < 20){
      limparLinha(T);
      EEPROM.update(BASE_LAMP + T, 0);
      salvarTecla(T);
    }
  }
}
      void BOT_APRENDER(int CLICADO){
    
         TEMPO_CLEAR ++;//PROGRESSÃO PARA APAGAMNETO DA EEPORM      
    
      }
      
       void BLUETOOT(){
        int DEL = 0;
        int CLICK_2 = 0;
       
        char c = Serial.read();
        
        if(c == 'a'){
          CLICK_2 = 1;
           BOT_APRENDER( CLICK_2);     
        }
       else if(c=='b'){
        configurarLuz(0);
       }
         else if(c=='c'){
        configurarLuz(1);
          
          
        }
           
            }


       
