#include <vector>
#include <numeric>

#include "grittr_path.hpp"
#include "maybe.hpp"
#include "util.hpp"

#include "catch.hpp"

const grittr::path::grid grid01 = {
     { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1} // 0
   , { -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1} // 1
   , { -1,  0, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,  0, -1} // 2
   , { -1,  0, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,  0, -1} // 3
   , { -1,  0, -2, -2,  0,  0,  0,  0,  0,  0,  0, -2, -2,  0, -1} // 4
   , { -1,  0, -2, -2,  0, -1, -1, -1, -1, -1,  0, -2, -2,  0, -1} // 5
   , { -1,  0, -2, -2,  0,  0,  0,  0,  0,  0,  0, -2, -2,  0, -1} // 6
   , { -1,  0, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,  0, -1} // 7
   , { -1,  0, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,  0, -1} // 8
   , { -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1} // 9
   , { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1} // 10
}; //   0   1   2   3   4   5   6   7   8   9  10  11  12  13  14

TEST_CASE("grid_state is checked correctly", "[grid_state]")
{   

    REQUIRE(grittr::path::util::is_(grittr::path::util::BLOCKED, grid01, 0, 0) == true);
    REQUIRE(grittr::path::util::is_(grittr::path::util::CONE, grid01, 1, 1)    == true);
    REQUIRE(grittr::path::util::is_(grittr::path::util::FREE, grid01, 2, 2)    == true);
}

TEST_CASE("get the outer cone index", "[cone_index]")
{   

    grittr::path::util::Maybe<std::tuple<size_t, size_t>> result = grittr::path::find_outer_cone_index(grid01);
    REQUIRE(result.successful() == true);
    if (result.successful()) {
        REQUIRE(std::get<0>(result.getSuccess()) == 1);
        REQUIRE(std::get<1>(result.getSuccess()) == 1);
    }
}


TEST_CASE("get the outer & inner lines", "[cone_lines]")
{   
    grittr::path::util::Maybe<std::tuple<size_t, size_t>> result = grittr::path::find_outer_cone_index(grid01);

    grittr::path::grid grid02 = grid01;
    std::vector<grittr::path::point> outer_points = grittr::path::flood_fill(grid02
                                                                           , result.getSuccess()
                                                                           , grittr::path::util::CONE 
                                                                           , grittr::path::util::PARSED_OUTER_CONE);
   REQUIRE(outer_points.size() == 40);

   std::vector<grittr::path::point> inner_points = grittr::path::get_remaining_cones(grid02);

   REQUIRE(inner_points.size() == 16);
}

TEST_CASE("get middle path", "[mid_path]")
{   
    grittr::path::util::Maybe<std::tuple<size_t, size_t>> result = grittr::path::find_outer_cone_index(grid01);

    grittr::path::grid grid02 = grid01;
    std::vector<grittr::path::point> outer_points = grittr::path::flood_fill(grid02
                                                                           , result.getSuccess()
                                                                           , grittr::path::util::CONE 
                                                                           , grittr::path::util::PARSED_OUTER_CONE);
   std::vector<grittr::path::point> inner_points = grittr::path::get_remaining_cones(grid02);
   std::vector<grittr::path::dpoint> middles = grittr::path::get_middle_path(outer_points, inner_points,1);
   
   bool outer_rows_bounds_exceeded = std::accumulate(middles.begin(),
                                               middles.end(),
                                               false,
                                               [&](bool b, grittr::path::dpoint dp){
                                                    double x = std::get<0>(dp);
                                                    double y = std::get<1>(dp);
                                                 return b || (x >= 0 && x <= 1) || (x >= 9 && x <= 10); 
                                                 });
   bool inner_bounds_exceeded = std::accumulate(middles.begin(),
                                               middles.end(),
                                               false,
                                               [&](bool b, grittr::path::dpoint dp){
                                                    double x = std::get<0>(dp);
                                                    double y = std::get<1>(dp);
                                                 return b || (x >= 4 && x <= 6 && y >=4 && y <= 10);
                                                 });
   bool outer_cols_bounds_exceeded = std::accumulate(middles.begin(),
                                               middles.end(),
                                               false,
                                               [&](bool b, grittr::path::dpoint dp){
                                                    double x = std::get<0>(dp);
                                                    double y = std::get<1>(dp);
                                                 return b || (y >= 0 && y <= 1) || (y >=13 && y <= 14); 
                                                 });
    std::for_each(middles.begin(), middles.end(), [&](grittr::path::dpoint dp){
        std::cout << "x: " << std::get<0>(dp) << " y: " << std::get<1>(dp) << '\n';
   });

   REQUIRE(outer_rows_bounds_exceeded == false);
   REQUIRE(inner_bounds_exceeded == false);
   REQUIRE(outer_cols_bounds_exceeded == false);
   

   
}