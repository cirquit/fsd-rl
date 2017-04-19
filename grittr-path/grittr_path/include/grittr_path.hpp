/** \file grittr_path.hpp
 *  This header contains the main logic for the path calculations...to be moved elsewhere
 */

#include <functional>
#include <algorithm>
#include <iostream>
#include <tuple>
#include <vector>
#include <cmath>

#include "maybe.hpp"
#include "util.hpp"

namespace grittr {
namespace path {

    using grid = std::vector<std::vector<int16_t> >;
    using point = std::tuple<size_t, size_t>;
    using dpoint = std::tuple<double, double>;

    util::Maybe<point> find_outer_cone_index(const grid& g)
    {
        for (size_t row = 0; row < g.size(); ++row) {
            for (size_t col = 0; col < g[row].size(); ++col) {
                if (util::is_(util::CONE, g, row, col))
                    return util::Something<point>({ row, col });
            }
        }
        return util::Nothing<point>();
    }

    void capture_point(point p, const point& direction, util::grid_state target_value, grid & g, std::vector<point>& queue)
    {
        point pdir = util::add_points(p, direction);
        if (util::point_exists(g, pdir) && util::is_(target_value, g, pdir)) {
            queue.push_back(pdir);
        }
    }

    void capture_line(point p, const point& direction, util::grid_state target_value, util::grid_state replacement_value, grid & g, std::vector<point>& results, std::vector<point>& queue)
    {   
        while (util::point_exists(g, p) && util::is_(target_value, g, p)) {
            results.push_back(p);
            queue.push_back(p);
            set_state(g, p, replacement_value);
            capture_point(p, util::north, target_value, g, queue);
            capture_point(p, util::south, target_value, g, queue);
            capture_point(p, util::ne, target_value, g, queue);
            capture_point(p, util::se, target_value, g, queue);
            capture_point(p, util::sw, target_value, g, queue);
            capture_point(p, util::sw, target_value, g, queue);
            p = util::add_points(p, direction);
        }
    }

    //! Flood fill algorithm adapted from https://en.wikipedia.org/wiki/Flood_fill
    //  Premise: The incoming point should be a valid target_value
    //
    //  Use in combination with find_outer_cone_index
    //
    std::vector<point> flood_fill(grid& g, point p, util::grid_state target_value, util::grid_state replacement_value)
    {
        if (!util::is_(target_value, g, p))
            return {};
        if (util::is_(replacement_value, g, p))
            return {};

        std::vector<point> results = {};
        std::vector<point> queue = { p };
        while (!queue.empty()) {
            point p = queue.back();
            queue.pop_back();
            capture_line(p, util::west, target_value, replacement_value, g, results, queue); // look to the left for neighbors
            capture_line(util::add_points(p, util::east), util::east, target_value, replacement_value, g, results, queue); // look to the right for neighbors
        }

        return results;
    }

    //! Transforms every outer cone to a `PARSED_OUTER_CONE` in the grid
    //  Returns said outer cones
    std::vector<point> get_outer_cones(grid& g)
    {
        util::Maybe<point> outerIndex = find_outer_cone_index(g);
        if (outerIndex.failed()) {
            return {};
        }
        return flood_fill(g, outerIndex.getSuccess(), util::CONE, util::PARSED_OUTER_CONE);
    }

    //! Transforms every outer cone to a `PARSED_OUTER_CONE` in the grid
    //  Returns said outer cones
    std::vector<point> get_remaining_cones(grid& g)
    {
        std::vector<point> cones;
        for(size_t row = 0; row < g.size(); ++row){
            for(size_t col = 0; col < g[row].size(); ++col){
                if (util::is_(util::CONE, g, row, col)) cones.push_back({row, col});
            }
        }
        return cones;
    }

    std::vector<dpoint> get_middle_path(std::vector<point> & outer_points, std::vector<point> & inner_points, size_t max_dist){
        std::vector<dpoint> middles;
        std::for_each(outer_points.begin(), outer_points.end(), [&](point outer){
            std::for_each(inner_points.begin(), inner_points.end(), [&](point inner){
                size_t dist = util::distance(outer, inner);
                if (dist <= max_dist) middles.push_back(util::normalize(outer,inner));
                if (dist <= max_dist) {
                    std::cout << "GMP: x1: " << std::get<0>(outer) << " y1: " << std::get<1>(outer)
                       << "\n      x2: " << std::get<0>(inner) << " y2: " << std::get<1>(inner)
                       << ", distance: " << std::to_string(dist) << "\n\n"; 
                }
            });
        });
        return middles;

    }

} // namespace path
} // namespace grittr