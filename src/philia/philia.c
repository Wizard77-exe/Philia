#include <stdio.h>
#include <unistd.h>

int main(void)
{
  /* NOTE: SPINNER ANIMATION LATER IF EVERYTHING'S STABLE

    const char *spinner[] = {
        "⠋", "⠙", "⠹", "⠸", "⠼",
        "⠴", "⠦", "⠧", "⠇", "⠏"
    };

    int frame = 0;

    while (1)
    {
        printf("\r%s Training Philia...", spinner[frame]);
        fflush(stdout);

        usleep(80000);           // 80 ms

        frame = (frame + 1) % 10;
    }*/ 

    return 0;
}
