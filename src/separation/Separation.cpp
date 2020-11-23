#include <fstream>
#include <graph/GridGraph.hpp>
#include <map>
#include <numeric>
#include <pathfinding/Distance.hpp>
#include <pathfinding/Path.hpp>
#include <separation/Separation.hpp>
#include <sstream>

using separation::ComplexSeparation;
using separation::TrivialSeparation;
using graph::Distance;
using graph::Node;
using graph::GridCell;

ComplexSeparation::ComplexSeparation(GridCell first,
                                     GridCell second,
                                     Node first_center,
                                     Node second_center,
                                     Distance center_distance)
    : first_(first),
      second_(second),
      first_center_(first_center),
      second_center_(second_center),
      center_distance_(center_distance) {}


auto ComplexSeparation::getCenterDistance() const noexcept
    -> Distance
{
    return center_distance_;
}

auto ComplexSeparation::getFirstCluster() const noexcept
    -> GridCell
{
    return first_;
}

auto ComplexSeparation::getSecondCluster() const noexcept
    -> GridCell
{
    return second_;
}

auto ComplexSeparation::getFirstClusterCenter() const noexcept
    -> Node
{
    return first_center_;
}

auto ComplexSeparation::switchSides() const noexcept
    -> ComplexSeparation
{
    return ComplexSeparation{second_,
                             first_,
                             second_center_,
                             first_center_,
                             center_distance_};
}

auto ComplexSeparation::getSecondClusterCenter() const noexcept
    -> Node
{
    return second_center_;
}


auto ComplexSeparation::toFile(std::string_view path) const noexcept
    -> void
{
    std::ofstream file{path.data()};
    for(auto node : first_) {
        file << "0: (" << node.row << ", " << node.column << ")\n";
    }
    for(auto node : second_) {
        file << "1: (" << node.row << ", " << node.column << ")\n";
    }

    file << "center: (" << first_center_.row << ", " << first_center_.column << ")\n";
    file << "center: (" << second_center_.row << ", " << second_center_.column << ")\n";
}

auto ComplexSeparation::toSmallFile(std::string_view path) const noexcept
    -> void
{
    std::ofstream file{path.data()};
    file << "type: complex\n";
    file << "first: ("
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
         << ")\n"
         << "first center: ("
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


TrivialSeparation::TrivialSeparation(graph::GridCell first,
                                     graph::GridCell second)
    : first_(first),
      second_(second) {}

auto TrivialSeparation::getFirstCluster() const noexcept
    -> graph::GridCell
{
    return first_;
}

auto TrivialSeparation::getSecondCluster() const noexcept
    -> graph::GridCell
{
    return second_;
}

auto TrivialSeparation::switchSides() const noexcept
    -> TrivialSeparation
{
    return TrivialSeparation{second_,
                             first_};
}


auto TrivialSeparation::toFile(std::string_view path) const noexcept
    -> void
{
    std::ofstream file{path.data()};
    for(auto node : first_) {
        file << "0: (" << node.row << ", " << node.column << ")\n";
    }
    for(auto node : second_) {
        file << "1: (" << node.row << ", " << node.column << ")\n";
    }
}

auto TrivialSeparation::toSmallFile(std::string_view path) const noexcept
    -> void
{
    std::ofstream file{path.data()};
    file << "type: trivial\n";
    file << "first: ("
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
}


auto separation::getFirstCluster(const Separation& sep) noexcept
    -> graph::GridCell
{
    return std::visit(
        [](const auto& separation) {
            return separation.getFirstCluster();
        },
        sep);
}

auto separation::getSecondCluster(const Separation& sep) noexcept
    -> graph::GridCell
{
    return std::visit(
        [](const auto& separation) {
            return separation.getSecondCluster();
        },
        sep);
}

auto separation::isSuperSetOf(const Separation& first, const Separation& second) noexcept
    -> bool
{
    auto first_left = getFirstCluster(first);
    auto first_right = getSecondCluster(first);

    auto second_left = getFirstCluster(second);
    auto second_right = getSecondCluster(second);

    return (first_left.isSuperSetOf(second_left) and first_right.isSuperSetOf(second_right))
        or (first_left.isSuperSetOf(second_right) and first_right.isSuperSetOf(second_left));
}

auto separation::isSubSetOf(const Separation& first, const Separation& second) noexcept
    -> bool
{
    auto first_left = getFirstCluster(first);
    auto first_right = getSecondCluster(first);

    auto second_left = getFirstCluster(second);
    auto second_right = getSecondCluster(second);

    return (first_left.isSubSetOf(second_left) and first_right.isSubSetOf(second_right))
        or (first_left.isSubSetOf(second_right) and first_right.isSubSetOf(second_left));
}

auto separation::canAnswer(const Separation& sep, graph::Node from, graph::Node to) noexcept
    -> bool
{
    auto left = getFirstCluster(sep);
    auto right = getSecondCluster(sep);

    auto left_from_iter = std::find(std::begin(left), std::end(left), from);
    auto left_to_iter = std::find(std::begin(left), std::end(left), to);

    auto right_from_iter = std::find(std::begin(right), std::end(right), from);
    auto right_to_iter = std::find(std::begin(right), std::end(right), to);

    return (left_from_iter != std::end(left) and right_to_iter != std::end(right))
        or (right_from_iter != std::end(right) and left_to_iter != std::end(left));
}


auto separation::operator==(const Separation& lhs, const Separation& rhs) noexcept
    -> bool
{
    return (getFirstCluster(lhs) == getFirstCluster(rhs)
            and getSecondCluster(lhs) == getSecondCluster(rhs))
        or (getFirstCluster(lhs) == getSecondCluster(rhs)
            and getSecondCluster(lhs) == getFirstCluster(rhs));
}

auto separation::operator!=(const Separation& lhs, const Separation& rhs) noexcept
    -> bool
{
    return !(lhs == rhs);
}


auto separation::operator<(const Separation& lhs, const Separation& rhs) noexcept
    -> bool
{
    return weight(lhs) < weight(rhs);
}

auto separation::toFile(const Separation& sep, std::string_view path) noexcept
    -> void

{
    std::visit(
        [=](const auto& separation) {
            return separation.toFile(path);
        },
        sep);
}

auto separation::toSmallFile(const Separation& sep, std::string_view path) noexcept
    -> void
{
    std::visit(
        [=](const auto& separation) {
            return separation.toSmallFile(path);
        },
        sep);
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
    type.erase(type.find("type: "), 6);

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
        return TrivialSeparation{first_cluster, second_cluster};
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

        return ComplexSeparation{first_cluster,
                                 second_cluster,
                                 first_center,
                                 second_center,
                                 distance};
    }

    return std::nullopt;
}

auto separation::weight(const Separation& sep) noexcept
    -> std::size_t
{
    return getFirstCluster(sep).size()
        * getSecondCluster(sep).size();
}

auto separation::toString(const Separation& sep) noexcept
    -> std::string
{
    std::stringstream ss;
    ss << getFirstCluster(sep)
       << ":"
       << getSecondCluster(sep)
       << "("
       << weight(sep)
       << ")";

    return ss.str();
}

auto separation::switchSides(const Separation& sep) noexcept
    -> Separation
{
    return std::visit(
        [](const auto& sep) -> Separation {
            return sep.switchSides();
        },
        sep);
}

auto separation::smallestDistance(const Separation& sep,
                                  const graph::GridGraph& graph) noexcept
    -> graph::Distance
{
    if(std::holds_alternative<ComplexSeparation>(sep)) {
        const auto complex_sep = std::get<ComplexSeparation>(sep);
        return complex_sep.getCenterDistance();
    }

    auto left = getFirstCluster(sep);
    auto right = getSecondCluster(sep);

    return std::accumulate(std::begin(left),
                           std::end(left),
                           graph::UNREACHABLE,
                           [&](auto init, auto next) {
                               auto min_dist =
                                   std::accumulate(
                                       std::begin(right),
                                       std::end(right),
                                       graph::UNREACHABLE,
                                       [&](auto init, auto next_r) {
                                           auto dist = graph.getTrivialDistance(next, next_r);
                                           return std::min(init, dist);
                                       });

                               return std::min(init, min_dist);
                           });
}


auto separation::sizeDistribution3DToFile(const std::vector<Separation>& separations,
                                          std::string_view file_path) noexcept
    -> void
{
    std::map<std::pair<std::int64_t, std::int64_t>, std::int64_t> sep_distribution;

    for(const auto& sep : separations) {
        auto first = getFirstCluster(sep).size();
        auto second = getSecondCluster(sep).size();

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
        const auto w = weight(sep);
        const auto dist = smallestDistance(sep, graph);
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
