#include "Background.h"
#include <numeric>

Background::Background(int width, int height):
    GameObject{0,0},
    m_width{width}, m_height{height}
{
    int a0{std::gcd(m_width, m_height)};
    a = 30;
    while(a0 % a) a++;
}

void Background::update()
{
    // Clear the window
    SDL_SetRenderDrawColor(m_renderer, 31, 37, 42, 255);
    SDL_RenderClear( m_renderer );
    // Draw grid
    SDL_SetRenderDrawColor(m_renderer, 62, 74, 84, 255);
    for(int x{a} ; x < m_width ; x += a)
        SDL_RenderDrawLine(m_renderer, x, 0, x, m_height);
    for(int y{a} ; y < m_height ; y += a)
        SDL_RenderDrawLine(m_renderer, 0, y, m_width, y);
}
