#include <sstream>

#include "StatusBar.h"

#define BORDER 5;

StatusBar::StatusBar(const Scene* scene,
        const Laser* laser, const std::vector<Obstacle*>* obstacles):
    GameObject{0, scene->h()-100},
    m_bar{0, scene->h()-100, scene->w(), 100},
    m_won{false},
    m_laser{laser},
    m_obstacles{obstacles},
    m_texture{nullptr}
{
    m_font = TTF_OpenFont((RESOURCES+"arcadeclassic.ttf").c_str(), 24);
    if (m_font == nullptr) throw InitError{InitError::TTF};
}

StatusBar::~StatusBar()
{
    if(m_font) TTF_CloseFont(m_font);
    if(m_texture) SDL_DestroyTexture(m_texture);
}

void StatusBar::init()
{
    refresh();
    if(m_texture == nullptr) throw InitError{};
}

void StatusBar::update()
{
    SDL_SetRenderDrawColor(m_renderer, 70, 70, 80, 255);
    SDL_RenderFillRect(m_renderer, &m_bar);
    SDL_SetRenderDrawColor(m_renderer, 40, 40, 50, 255);
    SDL_Rect border{m_bar};
    border.h = BORDER;
    SDL_RenderFillRect(m_renderer, &border);
    border.y = m_bar.y + m_bar.h - BORDER;
    SDL_RenderFillRect(m_renderer, &border);
    border = m_bar; border.w = BORDER;
    SDL_RenderFillRect(m_renderer, &border);
    border.x = m_bar.w - BORDER;
    SDL_RenderFillRect(m_renderer, &border);

    SDL_Rect label{m_bar};
    label.x += 2*BORDER; label.y += 2*BORDER;
    label.h -= 4*BORDER;

    if(m_won) {
        label.w -= 28*BORDER;
    }
    else {
        label.w = label.h;
        SDL_Rect sprite{34,0,34,34};
        SDL_RenderCopy(m_renderer, m_sprite_sheet, &sprite, &label);
        label.x += label.w;
    }

    SDL_RenderCopy(m_renderer, m_texture, nullptr, &label);
}

unsigned StatusBar::get_remaining() const
{
    return m_laser->get_solution().size() - m_obstacles->size();
}

void StatusBar::refresh()
{
    SDL_Color color{10,10,80,255};
    std::stringstream text;

    if(m_laser->can_hit_target()) {
    m_won = false;
        text <<  get_remaining();
    }
    else {
        m_won = true;
        text << "You win!";
        color = SDL_Color{30,60,10,255};
    }

    if(m_texture) SDL_DestroyTexture(m_texture);
    SDL_Surface* surface{ TTF_RenderText_Solid(
        m_font, text.str().c_str(), color) };
    m_texture = SDL_CreateTextureFromSurface(m_renderer, surface);
    SDL_FreeSurface(surface);
}
