/** \file util.hpp
 *  Multiple grid helper
 *
 */

#ifndef GRITTR_PATH_UTIL
#define GRITTR_PATH_UTIL

#include <tuple>
#include <vector>

namespace grittr {
namespace path {
    namespace util {

        using grid = std::vector<std::vector<int16_t> >;
        using point = std::tuple<size_t, size_t>;
        using dpoint = std::tuple<double, double>;

        //! possible grid states, these should be gotten from the grittr project when it compiles
        enum grid_state {
            PARSED_OUTER_CONE = -4,
            PARSED_INNER_CONE = -3,
            FREE = -2,
            BLOCKED = -1,
            CONE

        };

        //! simple wrapper for grid-states `int16_t` to `grid_state` enum
        grid_state to_state(int16_t i)
        {
            switch (i) {
            case -4:
                return PARSED_OUTER_CONE;
                break;
            case -3:
                return PARSED_INNER_CONE;
                break;
            case -2:
                return FREE;
                break;
            case -1:
                return BLOCKED;
                break;
            default:
                return CONE;
                break;
            }
        }

        //! helper to add two points together
        point add_points(const point& a, const point& b)
        {
            return { std::get<0>(a) + std::get<0>(b), std::get<1>(a) + std::get<1>(b) };
        }

        // TODO template or whatever
        dpoint normalize(const point& a, const point& b)
        {   
            point x = add_points(a, b);
            return { static_cast<double>(std::get<0>(x)) / 2.0 , static_cast<double>(std::get<1>(x)) / 2.0 };
        }

        double distance(const point& a, const point&b){
            return std::sqrt( std::pow(static_cast<int>(std::get<0>(a) - std::get<0>(b)), 2)
                            + std::pow(static_cast<int>(std::get<1>(a) - std::get<1>(b)), 2));
        }

        //! multiple predefined points to move through the grid
        const point north = { -1, 0 };
        const point east = { 0, 1 };
        const point south = { 1, 0 };
        const point west = { 0, -1 };
        const point ne = add_points(north, east);
        const point se = add_points(south, east);
        const point sw = add_points(south, west);
        const point nw = add_points(north, west);

        //! `get_state` accesses a single value at the indicies _row_ and _column_
        /*!
    */
        const grid_state get_state(const grid& g, point p)
        {
            return to_state(g[std::get<0>(p)][std::get<1>(p)]);
        }

        const grid_state get_state(const grid& g, size_t row, size_t col)
        {
            return to_state(g[row][col]);
        }

        void set_state(grid& g, point p, grid_state gs)
        {
            g[std::get<0>(p)][std::get<1>(p)] = gs;
        }

        void set_state(grid& g, size_t row, size_t col, grid_state gs)
        {
            g[row][col] = gs;
        }

        bool is_(grid_state gs, const grid& g, size_t row, size_t column)
        {
            return get_state(g, row, column) == gs;
        }

        bool is_(grid_state gs, const grid& g, point p)
        {
            return get_state(g, std::get<0>(p), std::get<1>(p)) == gs;
        }

        bool point_exists(const grid& g, point p)
        {
            return g.size() > std::get<0>(p)
                && g[std::get<0>(p)].size() > std::get<1>(p);
        }
    }
}
}

#endif
