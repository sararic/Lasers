#include "Button.h"

Button::Button(int x, int y, ButtonHandler* handler, std::string const& label):
    GameObject{x,y},
    m_handler{handler},
    m_label{label},
    m_is_pressed{false},
    m_color{BTN_COLOR_OFF},
    m_texture{nullptr}
{
    m_font = TTF_OpenFont((std::string{RESOURCES}+"arcadeclassic.ttf").c_str(), 24);
    if (m_font == nullptr) throw InitError{InitError::TTF};
}

Button::~Button()
{
    if(m_font) TTF_CloseFont(m_font);
    if(m_texture) SDL_DestroyTexture(m_texture);
}

void Button::init()
{
    setLabel(m_label);
    if(m_texture == nullptr) throw InitError{};
}

void Button::setLabel(std::string const& label)
{
    if(!m_renderer) throw InitError{"add button to scene before setting a label;"};

    m_label = label;
    SDL_Color color{10,10,80,255};

    if(m_texture) SDL_DestroyTexture(m_texture);
    SDL_Surface* surface{ TTF_RenderText_Solid(
        m_font, m_label.c_str(), color) };
    m_texture = SDL_CreateTextureFromSurface(m_renderer, surface);
    SDL_FreeSurface(surface);
}

void Button::update()
{
    SDL_Rect button{m_pos.first, m_pos.second, BTN_W, BTN_H};
    SDL_SetRenderDrawColor(m_renderer, m_color.r, m_color.g, m_color.b, m_color.a);
    SDL_RenderFillRect(m_renderer, &button);
    button.x += BTN_MARGIN; button.y += BTN_MARGIN;
    button.h -= 2*BTN_MARGIN; button.w -= 2*BTN_MARGIN;
    SDL_RenderCopy(m_renderer, m_texture, nullptr, &button);
}

int Button::manage_event(SDL_Event const& e)
{
    if(e.type == SDL_MOUSEBUTTONDOWN // being pressed
       && e.button.button == SDL_BUTTON_LEFT
       && e.button.x > m_pos.first && e.button.x < m_pos.first+BTN_W
       && e.button.y > m_pos.second && e.button.y < m_pos.second+BTN_H)
    {
        m_color = SDL_Color{BTN_COLOR_CLICKED};
        m_is_pressed = true;
    }

    else if(e.type == SDL_MOUSEBUTTONUP // un-press
       && e.button.button == SDL_BUTTON_LEFT
       && m_is_pressed)
    {
        m_is_pressed = false;
        m_color = SDL_Color{BTN_COLOR_OFF};

        if(e.button.x > m_pos.first && e.button.x < m_pos.first+BTN_W // send click signal
        && e.button.y > m_pos.second && e.button.y < m_pos.second+BTN_H)
        {
            m_color = SDL_Color{BTN_COLOR_ACTIVE};
            if(m_handler != nullptr) m_handler->handle_button();
        }
    }

    else if(e.type == SDL_MOUSEMOTION) // highlight button if necessary
    {
        if(e.button.x <= m_pos.first || e.button.x >= m_pos.first+BTN_W // un-highlight
        || e.button.y <= m_pos.second || e.button.y >= m_pos.second+BTN_H)
            m_color = SDL_Color{BTN_COLOR_OFF};
        else if(m_is_pressed) m_color = SDL_Color{BTN_COLOR_CLICKED};
        else m_color = SDL_Color{BTN_COLOR_ACTIVE};
    }

    return 0;
}
