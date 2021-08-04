#include <queue>

#include "Laser.h"

Laser::Laser(int width, int height,
             const Shooter* shooter,
             const Target* target,
             const std::vector<Obstacle*>* obstacles):
    GameObject{0,0},
    m_width{width}, m_height{height},
    m_shooter{shooter}, m_target{target},
    m_obstacles{obstacles},
    m_trajectory{}
{
    refresh_trajectory();
}

void Laser::update()
{
    SDL_SetRenderDrawColor(m_renderer, 255, 0, 0, 255);

    for(auto s : m_trajectory)
        SDL_RenderDrawLine(m_renderer,s.x0,s.y0,s.x1,s.y1);
/*
    SDL_SetRenderDrawColor(m_renderer, 0, 255, 0, 255);
    for(auto p : get_solution())
        SDL_RenderDrawPoint(m_renderer, p.first, p.second);
*/
}

void Laser::refresh_trajectory(bool higher_degree)
{
    if(!can_hit_target()) {
        m_trajectory = {};
        return;
    }

    if(!higher_degree){
        m_current.targetx = m_target->x();
        m_current.targety = m_target->y();
        m_current.Lx = m_width;
        m_current.Ly = m_height;

        m_current.nx = 0;
        m_current.ny = 0;
        m_current.id = LatticeID::PP;

        m_obstacle_size = 6;
    }

    for(LatticePoint p{m_current} ; p.nx < 25 ; ++p) {
        m_trajectory = {};
        compute_trajectory(p);

        if(!is_intercepted()) {
            m_current = p;
            return;
        }

        if(p.nx == 24) {
            // try with smaller obstacle size....
            m_obstacle_size -= 1;
            if(m_obstacle_size){
                refresh_trajectory(true);
            }
            break;
        }
    }
}

void Laser::compute_trajectory(Laser::LatticePoint target)
{
    // we first look at the nodes of the trajectory,
    // where it intercepts the boundary
    Node start, finish, p;
    start.t = 0, finish.t = 1;
    start.x = m_shooter->x();
    start.y = m_shooter->y();
    finish.x = target.targetx;
    finish.y = target.targety;
    std::priority_queue<Node> nodes{};
    nodes.push(start); nodes.push(finish);

    int k{1};
    float t{(float)(start.x - target.Lx)/(start.x - target.x_coord())};
    while(0 < t && t < 1){
        p.t = t;
        p.x = (k%2) ? m_width : 0;
        p.y = (int)std::round((1-t)*start.y + target.y_coord()*t) % (2*m_height);
        if(p.y < 0 ) p.y = 2*m_height + p.y;
        p.y = m_height - std::abs(p.y - m_height);
        nodes.push(p);
        k++;
        t = k*(float)(target.Lx - start.x)/(target.x_coord() - start.x);
    }
    k = 0;
    t = (float)start.x/(start.x - target.x_coord());
    while(0 < t && t < 1){
        p.t = t;
        p.x = (k%2) ? m_width : 0;
        p.y = (int)std::round((1-t)*start.y + target.y_coord()*t) % (2*m_height);
        if(p.y < 0 ) p.y = 2*m_height + p.y;
        p.y = m_height - std::abs(p.y - m_height);
        nodes.push(p);
        k--;
        t = k*(float)(target.Lx - start.x)/(target.x_coord() - start.x);
    }

    k = 1;
    t = (float)(start.y - target.Ly)/(start.y - target.y_coord());
    while(0 < t && t < 1){
        p.t = t;
        p.y = (k%2) ? m_height : 0;
        p.x = (int)std::round((1-t)*start.x + target.x_coord()*t) % (2*m_width);
        if(p.x < 0) p.x = 2*m_width + p.x;
        p.x = m_width - std::abs(p.x - m_width);
        nodes.push(p);
        k++;
        t = k*(float)(target.Ly - start.y)/(target.y_coord() - start.y);
    }
    k = 0;
    t = (float)start.y/(start.y - target.y_coord());
    while(0 < t && t < 1){
        p.t = t;
        p.y = (k%2) ? m_height : 0;
        p.x = (int)std::round((1-t)*start.x + target.x_coord()*t) % (2*m_width);
        if(p.x < 0) p.x = 2*m_width + p.x;
        p.x = m_width - std::abs(p.x - m_width);
        nodes.push(p);
        k--;
        t = k*(float)(target.Ly - start.y)/(target.y_coord() - start.y);
    }

    // we can then construct the segments
    p = nodes.top(); nodes.pop();
    while(nodes.size()){
        Segment s;
        s.x0 = p.x; s.y0 = p.y;
        s.x1 = nodes.top().x; s.y1 = nodes.top().y;
        m_trajectory.push_back(s);
        p = nodes.top();
        nodes.pop();
    }
}

bool Laser::is_intercepted() const
{
    Segment v_obs;

    for(auto obs : *m_obstacles)
    {
        v_obs.x1 = obs->x();
        v_obs.y1 = obs->y();

        bool intersects_first{false};
        bool intersects_last{false};

        v_obs.x0 = m_trajectory.front().x0;
        v_obs.y0 = m_trajectory.front().y0;
        if( check_collision(m_trajectory.front(), v_obs)
         && v_obs.dotp(m_trajectory.front()) > 0 )
            intersects_first = true;

        v_obs.x0 = m_trajectory.back().x1;
        v_obs.y0 = m_trajectory.back().y1;
        if( check_collision(m_trajectory.back(), v_obs)
         && v_obs.dotp(m_trajectory.back()) < 0 )
            intersects_last = true;

        // special case with only one segment
        if(m_trajectory.size() == 1){
            if(intersects_first && intersects_last)
                return true;
        }
        // in general
        else if(intersects_first || intersects_last)
            return true;

        // check all other segments
        for(size_t i{1} ; i < m_trajectory.size()-1 ; i++)
        {
            v_obs.x0 = m_trajectory[i].x0;
            v_obs.y0 = m_trajectory[i].y0;
            if(check_collision(m_trajectory[i], v_obs)) return true;
        }
    }
    return false;
}

bool Laser::check_collision(Segment const& s, Segment v_obs) const
{
    float z{v_obs.crossp(s)/s.norm()};
    if(std::abs(z) < m_obstacle_size) return true;
    return false;
}

std::set<std::pair<int,int>> Laser::get_solution() const
{
    std::set<std::pair<int,int>> solution;
    int x0{m_shooter->x()}; int y0{m_shooter->y()};
    int x1{m_target->x()}; int y1{m_target->y()};

    for(unsigned i{0} ; i < 16 ; i++)
    {
        int x{( (i&1 ? 1 : -1) * (x0 + (i&2 ? 1 : -1)*x1 )/2 ) % m_width};
        int y{( (i&4 ? 1 : -1) * (y0 + (i&8 ? 1 : -1)*y1 )/2 ) % m_height};
        if(x < 0) x = m_width + x;
        else if(x == 0) x = i&1 ? 0 : m_width;
        if(y < 0) y = m_height + y;
        else if(y == 0) y = i&4 ? 0 : m_height;
        solution.insert(std::pair<int,int>{x,y});
    }
    return solution;
}

bool Laser::can_hit_target() const
{
    std::set<std::pair<int,int>> solution{get_solution()};

    if(m_obstacles->size() >= solution.size()){
        for(auto obs : *m_obstacles){
            std::pair<int,int> p{obs->x(),obs->y()};
            if(solution.find(p) != solution.end())
                solution.erase(p);
        }
        if(solution.empty()) return false;
    }
    return true;
}

int Laser::LatticePoint::x_coord()
{
    int r{2*Lx*nx};
    r += (id & 0x02) ? targetx : -targetx;
    return r;
}

int Laser::LatticePoint::y_coord()
{
    int r{2*Ly*ny};
    r += (id & 0x01) ? targety : -targety;
    return r;
}

Laser::LatticePoint& Laser::LatticePoint::operator++()
{
    // cycle through the lattices
    switch(id) {
        case Laser::LatticeID::PP:
            id = Laser::LatticeID::PM;
            return *this;
        case Laser::LatticeID::PM:
            id = Laser::LatticeID::MP;
            return *this;
        case Laser::LatticeID::MP:
            id = Laser::LatticeID::MM;
            return *this;
        case Laser::LatticeID::MM:
            id = Laser::LatticeID::PP;
            break;
        default:
            break;
    };

    // spiral through L1 circles (squares) of increasing radius from the origin

    if(nx*ny == 0) { // the corner case
        if((nx > 0) || (!nx && !ny)) {ny++;} // move to next circle
        else if(ny > 0) {ny--; nx--;}
        else if(nx < 0) {ny--; nx++;}
        else if(ny < 0) {ny++; nx++;}
        return *this;
    }

    int dx{(ny > 0) ? -1 : 1};
    int dy{(nx > 0) ? 1 : -1};
    nx += dx; ny += dy;

    return *this;
}

bool operator<(Laser::Node const& a, Laser::Node const& b) {return a.t < b.t;}

float Laser::Segment::crossp(Segment const& s) const
    {return (x1-x0)*(s.y1-s.y0) - (y1-y0)*(s.x1-s.x0);}
float Laser::Segment::dotp(Segment const& s) const
    {return (x1-x0)*(s.x1-s.x0) + (y1-y0)*(s.y1-s.y0);}
float Laser::Segment::norm() const
    {return std::sqrt(dotp(*this));}
