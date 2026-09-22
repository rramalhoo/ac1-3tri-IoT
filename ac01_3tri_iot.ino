#include <Keypad.h>
#include <LiquidCrystal.h>
#include <Servo.h>

const byte LINHAS = 4;
const byte COLUNAS = 4;

char teclas[LINHAS][COLUNAS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte pinosLinhas[LINHAS] = {9, 8, 7, 6};
byte pinosColunas[COLUNAS] = {5, 4, 3, 2};

Keypad teclado = Keypad(makeKeymap(teclas), pinosLinhas, pinosColunas, LINHAS, COLUNAS);

LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);

Servo servoPorta;

const int pinoServo = 10;
const int buzzer = 11;
const int ledVerde = 13;
const int ledVermelho = 12;

String senhaCorreta = "1234";
String senhaDigitada = "";

int tentativas = 0;
const int maxTentativas = 3;

void setup() {
  lcd.begin(16, 2);

  servoPorta.attach(pinoServo);

  pinMode(buzzer, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledVermelho, OUTPUT);

  servoPorta.write(0);

  digitalWrite(ledVerde, LOW);
  digitalWrite(ledVermelho, LOW);

  lcd.setCursor(0, 0);
  lcd.print("Controle Acesso");
  delay(2000);

  mostrarTelaSenha();
}

void loop() {
  char tecla = teclado.getKey();

  if (tecla) {

    if (tecla >= '0' && tecla <= '9') {

      if (senhaDigitada.length() < 8) {
        senhaDigitada += tecla;

        lcd.setCursor(senhaDigitada.length() - 1, 1);
        lcd.print("*");

        tone(buzzer, 1000, 50);
      }
    }

    else if (tecla == '*') {
      senhaDigitada = "";

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Senha limpa");

      tone(buzzer, 700, 100);

      delay(1000);

      mostrarTelaSenha();
    }

    else if (tecla == '#') {

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

  if (senhaDigitada == senhaCorreta) {

    tentativas = 0;

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("ACESSO");
    lcd.setCursor(0, 1);
    lcd.print("LIBERADO!");

    digitalWrite(ledVermelho, LOW);
    digitalWrite(ledVerde, HIGH);

    tone(buzzer, 1200, 150);
    delay(200);
    tone(buzzer, 1500, 150);

    servoPorta.write(90);

    delay(5000);

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

    if (tentativas >= maxTentativas) {
      bloquearSistema();
    }

    else {
      mostrarTelaSenha();
    }
  }
}

void bloquearSistema() {

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SISTEMA");
  lcd.setCursor(0, 1);
  lcd.print("BLOQUEADO!");

  digitalWrite(ledVermelho, HIGH);

  tone(buzzer, 200, 1000);

  delay(10000);

  digitalWrite(ledVermelho, LOW);

  tentativas = 0;
  senhaDigitada = "";

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sistema liberado");

  delay(1500);

  mostrarTelaSenha();
}

void mostrarTelaSenha() {

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Digite a senha:");

  lcd.setCursor(0, 1);

  senhaDigitada = "";
}