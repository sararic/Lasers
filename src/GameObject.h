#pragma once

#include <SDL.h>
#include <utility>
#include <exception>
#include <string>

class InitError : public std::exception
{
    std::string msg;
public:
    enum errType{SDL, IMG, TTF}; //type of the error

    InitError(errType t = SDL);
    InitError( const std::string & );
    virtual ~InitError() noexcept;
    virtual const char * what() const noexcept;
};

class GameObject
{
    public:
        GameObject(int x, int y);
        virtual ~GameObject();

        virtual void update()=0;
        virtual int manage_event(SDL_Event const& e); //return value != 0 means the quit message has been sent
        virtual void init(); //for one-time functions that need the renderer or pixel size

        void setRenderer(SDL_Renderer* rdr);
        bool hasRenderer() const;
        int x() const;
        int y() const;
        std::pair<int,int> pos() const;

    protected:
        SDL_Renderer* m_renderer;
        std::pair<int,int> m_pos;
};

