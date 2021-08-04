#pragma once

#include <SDL_ttf.h>
#include <GameObject.h>

#define BTN_W 120
#define BTN_H 40
#define BTN_MARGIN 5

#define BTN_COLOR_OFF 80,80,100,255
#define BTN_COLOR_ACTIVE 40,40,50,255
#define BTN_COLOR_CLICKED 20,20,25,255

class ButtonHandler{
    public:
        virtual void handle_button()=0;
};

class Button : public GameObject
{
    public:
        Button(int x, int y, ButtonHandler* handler, std::string const& label);
        virtual ~Button();

        void update();
        void init();
        int manage_event(SDL_Event const& e);

        void setLabel(std::string const& label);
        void setHandler(ButtonHandler* handler) {m_handler = handler;}

    private:
        ButtonHandler* m_handler;
        std::string m_label;
        bool m_is_pressed;
        SDL_Color m_color;
        TTF_Font* m_font;
        SDL_Texture *m_texture;
};

