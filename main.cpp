#include <boost/dynamic_bitset.hpp>
#include <cassert>
#include <iostream>
#include <set>
#include <variant>
#include <vector>

namespace
{
using Char  = char;
using BitSet= boost::dynamic_bitset<>;
using Table = std::vector<std::pair<Char, BitSet>>;
class TreeNode {

public:
  std::variant<Char, std::pair<TreeNode const *, TreeNode const *>> const data;
  size_t weight;

  TreeNode()= delete;

  TreeNode(TreeNode const *lhs, TreeNode const *rhs)
      : data{ std::in_place_index<1>, lhs, rhs }
      , weight{ std::get<1>(data).first->weight +
                std::get<1>(data).second->weight }
  {}

  TreeNode(Char symbol, size_t weightIn)
      : data{ symbol }
      , weight{ weightIn }
  {}
};
using TreeFrequency= std::multiset<
    TreeNode,
    decltype(
        [](TreeNode const &lhs, TreeNode const &rhs) -> bool
        { return lhs.weight <= rhs.weight; })>;

using CharFrequency= std::set<
    TreeNode,
    decltype(
        [](TreeNode const &lhs, TreeNode const &rhs) -> bool
        { return std::get<0>(lhs.data) < std::get<0>(rhs.data); })>;

void extractTable(TreeNode const root, Table &table, BitSet bitSet)
{
  if (auto const node= std::get_if<Char>(&root.data)) {
    table.emplace_back(*node, std::move(bitSet));
  } else {
    {
      auto lBits= bitSet;
      lBits.push_back(true);
      extractTable(*std::get<1>(root.data).first, table, std::move(lBits));
    }
    bitSet.push_back(false);
    extractTable(*std::get<1>(root.data).second, table, std::move(bitSet));
  }
}

Table extractTable(TreeNode const root)
{
  Table table;
  extractTable(root, table, {});
  return std::move(table);
}
// beep boop beer!
CharFrequency fromString(std::string_view sv)
{
  CharFrequency frequency;
  for (auto s= sv.front(); !sv.empty(); sv.remove_prefix(1), s= sv.front()) {
    auto [it, f]= frequency.insert(TreeNode(s, 1));
    if (!f)
      ++const_cast<size_t &>(it->weight);
  }
  return std::move(frequency);
}
} // namespace

int main()
{
  TreeFrequency tree;
  size_t uniqueCharCount;
  {
    CharFrequency source(fromString("beep boop beer!"));

    uniqueCharCount= source.size();

    tree.merge(std::move(source));
  }

  for (auto const &i : tree)
    std::cout << i.weight << ' ' << std::get<0>(i.data) << '\n';

  std::vector<decltype(tree)::node_type> nodeStorage;
  nodeStorage.reserve(uniqueCharCount);
  while (tree.size() > 1) {
    nodeStorage.emplace_back(tree.extract(tree.cbegin()));
    nodeStorage.emplace_back(tree.extract(tree.cbegin()));
    tree.emplace(
        &nodeStorage.back().value(),
        &std::prev(nodeStorage.cend(), 2)->value());

    std::cout << '\n';
  }

  auto const table= extractTable(*tree.cbegin());
  std::cout << "reverse bits\n";
  for (auto const &[s, b] : table)
    std::cout << s << ' ' << b << '\n';

  return EXIT_SUCCESS;
}
