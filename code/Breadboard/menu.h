/*
 * menu.h
 *
 * Created: 28.09.2016 07:51:32
 * Authors: Group 39 (Alexandru, Jens, Vitor)
 */ 

#ifndef MENU_H_
#define MENU_H_

/* Print the menu */
void menu_print_menu(char **structure, int size, int element);

/* Menu functionality (option selection) */
int menu_show_menu(char **structure, int size);

#endif /* MENU_H_ */