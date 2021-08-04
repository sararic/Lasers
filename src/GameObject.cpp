#include "GameObject.h"

#include <SDL_image.h>
#include <SDL_ttf.h>

InitError::InitError(errType t) :
    exception{}, msg{"Error while initializing "}
{
    switch(t)
    {
        case SDL:
            msg += "SDL: ";
            msg += SDL_GetError();
            break;
        case IMG:
            msg += "SDL_Image: ";
            msg += IMG_GetError();
            break;
        case TTF:
            msg += "SDL_TTF: ";
            msg += TTF_GetError();
            break;
        default:
            msg += "unknown library.";
            break;
    }
}

InitError::InitError( const std::string & m ) :
    exception{},
    msg{ "Error: " }
{msg += m;}

InitError::~InitError() noexcept
{}

const char * InitError::what() const noexcept {return msg.c_str();}

GameObject::GameObject(int x, int y):
    m_renderer{nullptr},
    m_pos{x, y}
{}

GameObject::~GameObject() {}

int GameObject::manage_event(SDL_Event const& e) {return 0;}

void GameObject::init() {}

void GameObject::setRenderer(SDL_Renderer* rdr) {m_renderer = rdr;}

bool GameObject::hasRenderer() const {return (m_renderer != nullptr);}

int GameObject::x() const {return m_pos.first;}
int GameObject::y() const {return m_pos.second;}
std::pair<int,int> GameObject::pos() const {return m_pos;}
