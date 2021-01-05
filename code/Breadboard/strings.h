/*
 * strings.h
 *
 * Created: 16.11.2016 14:35:14
 * Authors: Group 39 (Alexandru, Jens, Vitor)
 */ 


#ifndef STRINGS_H_
#define STRINGS_H_

#include <avr/pgmspace.h>

/* The options for the menu in all languages */

const char de_0[] PROGMEM = "Spiel 1 starten";
const char de_1[] PROGMEM = "Spiel 2 starten";
const char de_2[] PROGMEM = "Bestwert anzeigen";
const char de_3[] PROGMEM = "Spiel vorbei!";
const char de_4[] PROGMEM = "Zeit abgelaufen!";
const char de_5[] PROGMEM = "Tore: %i";
const char de_6[] PROGMEM = "Zeit: %i";
const char de_7[] PROGMEM = "Bestwert: %i";
const char de_8[] PROGMEM = "Drueck das Joystick";
const char de_9[] PROGMEM = "nach links!";

const char *german[] = {
	de_0,
	de_1,
	de_2,
	de_3,
	de_4,
	de_5,
	de_6,
	de_7,
	de_8,
	de_9,
};

const char no_0[] PROGMEM = "Begynne med spill 1";
const char no_1[] PROGMEM = "Begynne med spill 2";
const char no_2[] PROGMEM = "Vis maksimum";
const char no_3[] PROGMEM = "Spill forbi!";
const char no_4[] PROGMEM = "Tid forbi!";
const char no_5[] PROGMEM = "Maal: %i";
const char no_6[] PROGMEM = "Tid: %i";
const char no_7[] PROGMEM = "Maksimum: %i";
const char no_8[] PROGMEM = "Trykk styrespaket";
const char no_9[] PROGMEM = "til venstre!";

const char *norwegian[] = {
	no_0,
	no_1,
	no_2,
	no_3,
	no_4,
	no_5,
	no_6,
	no_7,
	no_8,
	no_9,
};

const char pt_0[] PROGMEM = "Iniciar Jogo 1";
const char pt_1[] PROGMEM = "Iniciar Jogo 2";
const char pt_2[] PROGMEM = "Mostrar Recorde";
const char pt_3[] PROGMEM = "Fim de jogo!";
const char pt_4[] PROGMEM = "Tempo Esgotado!";
const char pt_5[] PROGMEM = "Gols: %i";
const char pt_6[] PROGMEM = "Tempo: %i";
const char pt_7[] PROGMEM = "Recorde: %i";
const char pt_8[] PROGMEM = "Mova o Joystick";
const char pt_9[] PROGMEM = "para a esquerda!";

const char *portuguese[] = {
	pt_0,
	pt_1,
	pt_2,
	pt_3,
	pt_4,
	pt_5,
	pt_6,
	pt_7,
	pt_8,
	pt_9,
};

const char ro_0[] PROGMEM = "Incepe Jocul 1";
const char ro_1[] PROGMEM = "Incepe Jocul 2";
const char ro_2[] PROGMEM = "Arata Scor Record";
const char ro_3[] PROGMEM = "Joc terminat!";
const char ro_4[] PROGMEM = "Timp terminat!";
const char ro_5[] PROGMEM = "Goluri: %i";
const char ro_6[] PROGMEM = "Timp: %i";
const char ro_7[] PROGMEM = "Scor Record: %i";
const char ro_8[] PROGMEM = "Misca Joystick";
const char ro_9[] PROGMEM = "la stanga!";

const char *romanian[] = {
	ro_0,
	ro_1,
	ro_2,
	ro_3,
	ro_4,
	ro_5,
	ro_6,
	ro_7,
	ro_8,
	ro_9,
};

const char **strings[] = {
	german,
	norwegian,
	portuguese,
	romanian,
};

#endif /* STRINGS_H_ */
