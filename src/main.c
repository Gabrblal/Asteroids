#include <stdio.h>

#include <SFML/Window.h>

#include "list.h"

int main(int argc , char ** argv)
{
    // sfWindow *window = sfWindow_create(
    //     (sfVideoMode){500, 500, 255},
    //     "Astroids",
    //     sfDefaultStyle,
    //     NULL
    // );

    // while (sfWindow_isOpen(window))
    // {
    //     sfEvent event;
    //     while (sfWindow_pollEvent(window, &event))
    //     {
    //         if (event.type == sfEvtClosed) {
    //             sfWindow_close(window);
    //         }
    //     }
    // }

    List *list = list_create(sizeof(int));
    for (int i = 0; i < 10; i++) {
        list_push_front(list, &i);
    }

    for (int i = 0; i < 10; i++) {
        int j;
        list_pop_back(list, &j); 
        printf("%i", j);
    }

    return 0;
}