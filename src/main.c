#include <stdio.h>

#include <SFML/Window.h>

int main(int argc, char ** argv)
{
    if (argc);
    if (argv);

    sfWindow *window = sfWindow_create(
        (sfVideoMode){500, 500, 255},
        "Astroids",
        sfDefaultStyle,
        NULL
    );

    while (sfWindow_isOpen(window))
    {
        sfEvent event;
        while (sfWindow_pollEvent(window, &event))
        {
            if (event.type == sfEvtClosed) {
                sfWindow_close(window);
            }
        }
    }

    return 0;
}