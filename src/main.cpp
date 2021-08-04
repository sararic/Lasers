#include <iostream>

#include "GameManager.h"


int main( int argc, char * argv[] )
{
    int win_width{400};
    int win_height{700};
    int dt{1};

    try
    {
        Scene scene{ win_width, win_height, "Lasers!!", dt, SDL_INIT_VIDEO | SDL_INIT_TIMER };
        GameManager manager{&scene};
        scene.play();
        return EXIT_SUCCESS;
    }
    catch ( const InitError & err )
    {
        std::cerr << err.what()
                  << std::endl;
    }
    return EXIT_FAILURE;
}

