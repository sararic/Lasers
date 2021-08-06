#pragma once

#include <vector>
#include <set>

#include "GameObject.h"
#include "StillObject.h"

class Laser : public GameObject
{
    public:
        // we can see the mirror room as a lattice covering all of R^2
        // The targets are organized into four square lattices (indexed by LatticeID)
        enum LatticeID{PP=0x03, PM=0x02, MP=0x01, MM=0x00};

        struct LatticePoint
        {
            int x_coord();
            int y_coord();
            LatticePoint& operator++();
            int nx, ny, Lx, Ly, targetx, targety;
            LatticeID id;
        };

        struct Node
        {
            int x,y;
            float t;
        };

        struct Segment
        {
            float crossp(Segment const& s) const;
            float dotp(Segment const& s) const;
            float norm() const;
            int x0, y0, x1, y1;
        };

        Laser(int width, int height,
              const Shooter* shooter,
              const Target* target,
              const std::vector<Obstacle*>* obstacles);

        void update();

        void refresh_trajectory(bool higher_degree=false);
        bool can_hit_target() const;
        std::set<std::pair<int,int>> get_solution() const;

    private:
        bool is_intercepted() const;
        bool check_collision(Segment const& s, Segment v_obs) const;
        // find a non-obstructed straight line to a point on the target lattice
        void compute_trajectory(LatticePoint target);

        int m_width;
        int m_height;
        const Shooter* m_shooter;
        const Target* m_target;
        const std::vector<Obstacle*>* m_obstacles;
        int m_obstacle_size;
        std::vector<Segment> m_trajectory;
        LatticePoint m_current;
};

bool operator<(Laser::Node const& a, Laser::Node const& b);
