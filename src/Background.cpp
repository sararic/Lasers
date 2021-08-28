#include "Background.h"
#include <numeric>

Background::Background(int width, int height):
    GameObject{0,0},
    m_width{width}, m_height{height},
    m_texture{nullptr}
{
    int a0{std::gcd(m_width, m_height)};
    a = 30;
    while(a0 % a) a++;
}

Background::~Background()
{
    if(m_texture) SDL_DestroyTexture(m_texture);
}

void Background::init()
{
    if(m_texture) return;
    SDL_Surface* surface{SDL_CreateRGBSurfaceWithFormat(0, m_width, m_height,
        8, SDL_PIXELFORMAT_RGBA8888)};
    if(surface == nullptr) throw InitError{};
    SDL_Renderer* bg_renderer{SDL_CreateSoftwareRenderer(surface)};
    if(bg_renderer == nullptr) throw InitError{};

    // Clear the texture
    SDL_SetRenderDrawColor(bg_renderer, 31, 37, 42, 255);
    SDL_RenderClear( bg_renderer );
    // Draw grid
    SDL_SetRenderDrawColor(bg_renderer, 62, 74, 84, 255);
    for(int x{a} ; x < m_width ; x += a)
        SDL_RenderDrawLine(bg_renderer, x, 0, x, m_height);
    for(int y{a} ; y < m_height ; y += a)
        SDL_RenderDrawLine(bg_renderer, 0, y, m_width, y);

    m_texture = SDL_CreateTextureFromSurface(m_renderer, surface);
    if(m_texture == nullptr) throw InitError{};
    SDL_DestroyRenderer(bg_renderer);
    SDL_FreeSurface(surface);
}

void Background::update()
{
    SDL_Rect r{0,0,m_width,m_height};
    SDL_RenderCopy(m_renderer, m_texture, &r, &r);
}
