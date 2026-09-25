#include <Keypad.h>          // Biblioteca do teclado 4x4
#include <LiquidCrystal.h>   // Biblioteca do LCD
#include <Servo.h>           // Biblioteca do servo motor

const byte LINHAS = 4;
const byte COLUNAS = 4;

// Define as teclas do teclado
char teclas[LINHAS][COLUNAS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

// Pinos conectados ao teclado
byte pinosLinhas[LINHAS] = {9, 8, 7, 6};
byte pinosColunas[COLUNAS] = {5, 4, 3, 2};

Keypad teclado = Keypad(makeKeymap(teclas), pinosLinhas, pinosColunas, LINHAS, COLUNAS);

// Pinos do LCD
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);

Servo servoPorta;

// Pinos dos componentes
const int pinoServo = 10;
const int buzzer = 11;
const int ledVerde = 13;
const int ledVermelho = 12;

// Senha do sistema
String senhaCorreta = "1234";
String senhaDigitada = "";

int tentativas = 0;
const int maxTentativas = 3;

void setup() {
  // Inicia o LCD
  lcd.begin(16, 2);

  // Configura o servo
  servoPorta.attach(pinoServo);

  // Configura as saídas
  pinMode(buzzer, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledVermelho, OUTPUT);

  // Mantém a porta fechada
  servoPorta.write(0);

  digitalWrite(ledVerde, LOW);
  digitalWrite(ledVermelho, LOW);

  // Mensagem inicial
  lcd.setCursor(0, 0);
  lcd.print("Controle Acesso");
  delay(2000);

  mostrarTelaSenha();
}

void loop() {
  // Lê a tecla pressionada
  char tecla = teclado.getKey();

  if (tecla) {

    // Adiciona números à senha
    if (tecla >= '0' && tecla <= '9') {

      if (senhaDigitada.length() < 8) {
        senhaDigitada += tecla;

        // Mostra * no lugar dos números
        lcd.setCursor(senhaDigitada.length() - 1, 1);
        lcd.print("*");

        tone(buzzer, 1000, 50);
      }
    }

    // Limpa a senha
    else if (tecla == '*') {
      senhaDigitada = "";

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Senha limpa");

      tone(buzzer, 700, 100);

      delay(1000);

      mostrarTelaSenha();
    }

    // Confirma a senha
    else if (tecla == '#') {

      // Verifica se alguma senha foi digitada
      if (senhaDigitada.length() == 0) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Digite a senha");

        lcd.setCursor(0, 1);
        lcd.print("antes de OK");

        tone(buzzer, 300, 300);

        delay(1500);

        mostrarTelaSenha();
      }

      else {
        verificarSenha();
      }
    }
  }
}

void verificarSenha() {

  // Verifica se a senha está correta
  if (senhaDigitada == senhaCorreta) {

    tentativas = 0;

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("ACESSO");
    lcd.setCursor(0, 1);
    lcd.print("LIBERADO!");

    // Acende o LED verde
    digitalWrite(ledVermelho, LOW);
    digitalWrite(ledVerde, HIGH);

    // Som de acesso liberado
    tone(buzzer, 1200, 150);
    delay(200);
    tone(buzzer, 1500, 150);

    // Abre a porta
    servoPorta.write(90);

    delay(5000);

    // Fecha a porta
    servoPorta.write(0);

    digitalWrite(ledVerde, LOW);

    senhaDigitada = "";

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Porta fechada");

    delay(1500);

    mostrarTelaSenha();
  }

  else {

    // Soma uma tentativa errada
    tentativas++;

    digitalWrite(ledVerde, LOW);
    digitalWrite(ledVermelho, HIGH);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Senha incorreta");

    lcd.setCursor(0, 1);
    lcd.print("Tentativa ");
    lcd.print(tentativas);
    lcd.print("/3");

    tone(buzzer, 300, 500);

    delay(2000);

    digitalWrite(ledVermelho, LOW);

    senhaDigitada = "";

    // Bloqueia após 3 erros
    if (tentativas >= maxTentativas) {
      bloquearSistema();
    }

    else {
      mostrarTelaSenha();
    }
  }
}

void bloquearSistema() {

  // Mostra que o sistema foi bloqueado
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SISTEMA");
  lcd.setCursor(0, 1);
  lcd.print("BLOQUEADO!");

  digitalWrite(ledVermelho, HIGH);

  tone(buzzer, 200, 1000);

  // Mantém bloqueado por 10 segundos
  delay(10000);

  digitalWrite(ledVermelho, LOW);

  // Reinicia as tentativas
  tentativas = 0;
  senhaDigitada = "";

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sistema liberado");

  delay(1500);

  mostrarTelaSenha();
}

void mostrarTelaSenha() {

  // Volta para a tela de digitação
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Digite a senha:");

  lcd.setCursor(0, 1);

  senhaDigitada = "";
}
