/*
 * l10n.c
 *
 * Created: 16.11.2016 14:25:23
 * Authors: Group 39 (Alexandru, Jens, Vitor)
 */

#include <avr/pgmspace.h>

#include "strings.h"
#include "l10n.h"

Language language;

char *language_menu[] = {"Deutsch", "Norsk", "Portugues", "Romana"};

char buffer[256];

char * _(int message) {
	char *result = &(buffer[message % 4 * 64]);
	strcpy_P(result, strings[language][message]);
	return result;
}