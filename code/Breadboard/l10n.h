/*
 * l10n.h
 *
 * Created: 16.11.2016 14:25:37
 * Authors: Group 39 (Alexandru, Jens, Vitor)
 */


#ifndef L10N_H_
#define L10N_H_

/* Language options */
typedef enum {
	GERMAN = 0,
	NORWEGIAN = 1,
	PORTUGUESE = 2,
	ROMANIAN = 3,
} Language;

/* Menu options */
typedef enum {
	S_GAME_START_1,
	S_GAME_START_2,
	S_SHOW_HIGHSCROE,
	S_GAME_OVER,
	S_TIME_OVER,
	S_GOALS,
	S_TIME,
	S_HIGHSCORE,
	S_JOYSTICK_MESSAGE,

} Strings;

char * _(int message);

extern Language language;

extern char *language_menu[];

#endif /* L10N_H_ */