//
// Created by dym on 25.04.2021.
//

#ifndef HUFFMANALGORITHM_HUFFMANALGORITHM_HPP
#define HUFFMANALGORITHM_HUFFMANALGORITHM_HPP
#include <QBitArray>
#include <set>
#include <variant>
#include <vector>

// class HuffmanAlgorithm {
using Char  = char;
using BitSet= QBitArray;
using TableOut= std::vector<std::pair<Char, BitSet>>;
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
        [](TreeNode const lhs, TreeNode const rhs) -> bool
        { return lhs.weight <= rhs.weight; })>;

using CharFrequency= std::set<
    TreeNode,
    decltype(
        [](TreeNode const &lhs, TreeNode const &rhs) -> bool
        { return std::get<0>(lhs.data) < std::get<0>(rhs.data); })>;

void extractTable(TreeNode const root, TableOut &table, BitSet bitSet)
{
  if (auto const node= std::get_if<Char>(&root.data)) {
    table.emplace_back(*node, std::move(bitSet));
  } else {
    {
      auto lBits= bitSet;
      lBits.resize(lBits.size() + 1);
      lBits.setBit(lBits.size() - 1, true);
      //        lBits.push_back(true);
      extractTable(*std::get<1>(root.data).first, table, std::move(lBits));
    }
    bitSet.resize(bitSet.size() + 1);
    bitSet.setBit(bitSet.size() - 1, false);
    //      bitSet.push_back(false);
    extractTable(*std::get<1>(root.data).second, table, std::move(bitSet));
  }
}

TableOut extractTable(TreeNode const root)
{
  TableOut table;
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
//};

#endif // HUFFMANALGORITHM_HUFFMANALGORITHM_HPP