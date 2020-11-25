#include <fstream>
#include <graph/GridGraph.hpp>
#include <map>
#include <numeric>
#include <pathfinding/Distance.hpp>
#include <pathfinding/Path.hpp>
#include <separation/Separation.hpp>
#include <sstream>

using separation::Separation;
using graph::Distance;
using graph::Node;
using graph::GridCell;

Separation::Separation(GridCell first,
                       GridCell second,
                       Node first_center,
                       Node second_center,
                       Distance center_distance)
    : first_(first),
      second_(second),
      first_center_(first_center),
      second_center_(second_center),
      center_distance_(center_distance) {}


auto Separation::getCenterDistance() const noexcept
    -> Distance
{
    return center_distance_;
}

auto Separation::getFirstCluster() const noexcept
    -> GridCell
{
    return first_;
}

auto Separation::getSecondCluster() const noexcept
    -> GridCell
{
    return second_;
}

auto Separation::getFirstClusterCenter() const noexcept
    -> Node
{
    return first_center_;
}

auto Separation::isComplex() const noexcept
    -> bool
{
    return center_distance_ != -1;
}

auto Separation::isTrivial() const noexcept
    -> bool
{
    return center_distance_ == -1;
}

auto Separation::switchSides() const noexcept
    -> Separation
{
    return Separation{second_,
                      first_,
                      second_center_,
                      first_center_,
                      center_distance_};
}

auto Separation::getSecondClusterCenter() const noexcept
    -> Node
{
    return second_center_;
}


auto Separation::toFile(std::string_view path) const noexcept
    -> void
{
    std::ofstream file{path.data()};
    for(auto node : first_) {
        file << "0: (" << node.row << ", " << node.column << ")\n";
    }
    for(auto node : second_) {
        file << "1: (" << node.row << ", " << node.column << ")\n";
    }

    if(isTrivial()) {
        return;
    }

    file << "center: (" << first_center_.row << ", " << first_center_.column << ")\n";
    file << "center: (" << second_center_.row << ", " << second_center_.column << ")\n";
}

auto Separation::toSmallFile(std::string_view path) const noexcept
    -> void
{
    std::ofstream file{path.data()};
    file << "type: "
         << (isComplex() ? "complex\n" : "trivial\n")
         << "first: ("
         << first_.getTopLeft().getRow()
         << ", "
         << first_.getTopLeft().getColumn()
         << "), ("
         << first_.getBottomRight().getRow()
         << ", "
         << first_.getBottomRight().getColumn()
         << ")\n"
         << "second: ("
         << second_.getTopLeft().getRow()
         << ", "
         << second_.getTopLeft().getColumn()
         << "), ("
         << second_.getBottomRight().getRow()
         << ", "
         << second_.getBottomRight().getColumn()
         << ")\n";

    if(isTrivial()) {
        return;
    }

    file << "first center: ("
         << first_center_.row
         << ", "
         << first_center_.column
         << ")\n"
         << "second center: ("
         << second_center_.row
         << ", "
         << second_center_.column
         << ")\n"
         << "center to center distance: "
         << center_distance_
         << "\n";
}

auto Separation::isSuperSetOf(const Separation& other) const noexcept
    -> bool
{
    return (first_.isSuperSetOf(other.first_) and second_.isSuperSetOf(other.second_))
        or (first_.isSuperSetOf(other.second_) and second_.isSuperSetOf(other.first_));
}

auto Separation::isSubSetOf(const Separation& other) const noexcept
    -> bool
{
    return (first_.isSubSetOf(other.first_) and second_.isSubSetOf(other.second_))
        or (first_.isSubSetOf(other.second_) and second_.isSubSetOf(other.first_));
}

auto Separation::canAnswer(graph::Node from, graph::Node to) const noexcept
    -> bool
{
    auto left_from_iter = std::find(std::begin(first_), std::end(first_), from);
    auto left_to_iter = std::find(std::begin(first_), std::end(first_), to);

    auto right_from_iter = std::find(std::begin(second_), std::end(second_), from);
    auto right_to_iter = std::find(std::begin(second_), std::end(second_), to);

    return (left_from_iter != std::end(first_) and right_to_iter != std::end(second_))
        or (right_from_iter != std::end(second_) and left_to_iter != std::end(first_));
}

auto Separation::weight() const noexcept
    -> std::size_t
{
    return first_.size() * second_.size();
}

auto Separation::toString() const noexcept
    -> std::string
{
    std::stringstream ss;
    ss << first_
       << ":"
       << second_
       << "("
       << weight()
       << ")";

    return ss.str();
}

auto separation::operator==(const Separation& lhs, const Separation& rhs) noexcept
    -> bool
{
    return (lhs.getFirstCluster() == rhs.getFirstCluster()
            and lhs.getSecondCluster() == rhs.getSecondCluster())
        or (lhs.getFirstCluster() == rhs.getSecondCluster()
            and lhs.getSecondCluster() == rhs.getFirstCluster());
}

auto separation::operator!=(const Separation& lhs, const Separation& rhs) noexcept
    -> bool
{
    return !(lhs == rhs);
}

auto separation::operator<(const Separation& lhs, const Separation& rhs) noexcept
    -> bool
{
    return lhs.weight() < rhs.weight();
}

auto Separation::smallestDistance(const graph::GridGraph& graph) const noexcept
    -> graph::Distance
{
    if(isComplex()) {
        return center_distance_;
    }

    return std::accumulate(std::begin(first_),
                           std::end(first_),
                           graph::UNREACHABLE,
                           [&](auto init, auto next) {
                               auto min_dist =
                                   std::accumulate(
                                       std::begin(second_),
                                       std::end(second_),
                                       graph::UNREACHABLE,
                                       [&](auto init, auto next_r) {
                                           auto dist = graph.getTrivialDistance(next, next_r);
                                           return std::min(init, dist);
                                       });

                               return std::min(init, min_dist);
                           });
}

namespace {

auto split_string(const std::string& str)
    -> std::vector<std::string>
{
    std::vector<std::string> strings;

    std::string::size_type pos = 0;
    std::string::size_type prev = 0;
    while((pos = str.find('\n', prev)) != std::string::npos) {
        strings.push_back(str.substr(prev, pos - prev));
        prev = pos + 1;
    }

    strings.push_back(str.substr(prev));

    return strings;
}
auto eraseAllSubStr(std::string main_str, const std::string& to_erase)
    -> std::string
{
    size_t pos = std::string::npos;
    // Search for the substring in string in a loop untill nothing is found
    while((pos = main_str.find(to_erase)) != std::string::npos) {
        // If found then erase it from string
        main_str.erase(pos, to_erase.length());
    }

    return main_str;
}

auto parseCluster(std::string_view str)
    -> GridCell
{
    std::vector<std::int64_t> vect;

    std::stringstream ss(str.data());

    for(std::int64_t i; ss >> i;) {
        vect.push_back(i);
        if(ss.peek() == ',')
            ss.ignore();
    }

    std::int64_t top_left_row = vect[0];
    std::int64_t top_left_column = vect[1];
    std::int64_t bottom_right_row = vect[2];
    std::int64_t bottom_right_column = vect[3];

    graph::GridCorner top_left{top_left_row, top_left_column};
    graph::GridCorner bottom_right{bottom_right_row, bottom_right_column};

    return GridCell{top_left, bottom_right};
}

auto parseNode(std::string_view str)
    -> Node
{
    std::vector<std::size_t> vect;

    std::stringstream ss(str.data());

    for(std::size_t i; ss >> i;) {
        vect.push_back(i);
        if(ss.peek() == ',')
            ss.ignore();
    }

    return Node{vect[0], vect[1]};
}

} // namespace

auto separation::fromFile(std::string_view path) noexcept
    -> std::optional<Separation>
{
    std::ifstream istream(path.data());
    std::string str((std::istreambuf_iterator<char>(istream)),
                    std::istreambuf_iterator<char>());

    auto splitted = split_string(str);
    auto type = splitted[0];
    type = eraseAllSubStr(type, "type: ");

    auto first_str = splitted[1];
    auto second_str = splitted[2];

    first_str = eraseAllSubStr(first_str, "(");
    first_str = eraseAllSubStr(first_str, ")");
    first_str = eraseAllSubStr(first_str, " ");
    first_str = eraseAllSubStr(first_str, "first:");

    second_str = eraseAllSubStr(second_str, "(");
    second_str = eraseAllSubStr(second_str, ")");
    second_str = eraseAllSubStr(second_str, " ");
    second_str = eraseAllSubStr(second_str, "second:");

    auto first_cluster = parseCluster(first_str);
    auto second_cluster = parseCluster(second_str);

    if(type == "trivial") {
        return Separation{first_cluster, second_cluster};
    }

    if(type == "complex") {
        auto first_center_str = splitted[3];
        auto second_center_str = splitted[4];
        auto distance_str = splitted[5];

        first_center_str = eraseAllSubStr(first_center_str, "(");
        first_center_str = eraseAllSubStr(first_center_str, " ");
        first_center_str = eraseAllSubStr(first_center_str, ")");
        first_center_str = eraseAllSubStr(first_center_str, "firstcenter:");

        second_center_str = eraseAllSubStr(second_center_str, "(");
        second_center_str = eraseAllSubStr(second_center_str, " ");
        second_center_str = eraseAllSubStr(second_center_str, ")");
        second_center_str = eraseAllSubStr(second_center_str, "secondcenter:");

        distance_str = eraseAllSubStr(distance_str, "center to center distance: ");

        auto first_center = parseNode(first_center_str);
        auto second_center = parseNode(second_center_str);
        auto distance = std::stol(distance_str);

        return Separation{first_cluster,
                          second_cluster,
                          first_center,
                          second_center,
                          distance};
    }

    return std::nullopt;
}

auto separation::sizeDistribution3DToFile(const std::vector<Separation>& separations,
                                          std::string_view file_path) noexcept
    -> void
{
    std::map<std::pair<std::int64_t, std::int64_t>, std::int64_t> sep_distribution;

    for(const auto& sep : separations) {
        auto first = sep.getFirstCluster().size();
        auto second = sep.getSecondCluster().size();

        auto smaller = std::min(first, second);
        auto larger = std::max(first, second);

        sep_distribution[std::pair{smaller, larger}]++;
    }

    std::ofstream file{file_path.data()};
    for(auto [pair, amount] : sep_distribution) {
        auto [smaller, larger] = pair;
        file << smaller << ", " << larger << ", " << amount << "\n";
    }
}

auto separation::sizeToDistanceToFile(const std::vector<Separation>& separations,
                                      const graph::GridGraph& graph,
                                      std::string_view file_path) noexcept
    -> void
{
    std::unordered_map<std::size_t, std::vector<graph::Distance>> sep_dist;

    for(const auto& sep : separations) {
        const auto w = sep.weight();
        const auto dist = sep.smallestDistance(graph);
        sep_dist[w].emplace_back(dist);
    }

    std::ofstream file{file_path.data()};
    for(auto [w, dist] : sep_dist) {
        auto avg_dist = std::accumulate(std::begin(dist),
                                        std::end(dist),
                                        0l)
            / static_cast<double>(dist.size());
        file << w << ", " << avg_dist << "\n";
    }
}
