//
// Created by dym on 25.04.2021.
//
#include "HuffmanAlgorithm.hpp"
#include <memory>
#include <set>

namespace
{}

class HuffmanAlgorithm::TreeNode {
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

struct HuffmanAlgorithm::CompTreeFrequency
{
  bool operator()(TreeNode const lhs, TreeNode const rhs) const
  {
    return lhs.weight <= rhs.weight;
  }
};

struct HuffmanAlgorithm::CompCharFrequency
{
  bool operator()(TreeNode const lhs, TreeNode const rhs) const
  {
    return std::get<0>(lhs.data) < std::get<0>(rhs.data);
  }
};

HuffmanAlgorithm::~HuffmanAlgorithm()= default;
HuffmanAlgorithm::HuffmanAlgorithm() = default;
void HuffmanAlgorithm::store(std::basic_string_view<Char> sv)
{
  if (!chars)
    chars= std::make_unique<CharFrequency>();
  for (auto s= sv.front(); !sv.empty(); sv.remove_prefix(1), s= sv.front()) {
    auto [it, f]= chars->insert(TreeNode(s, 1));
    if (!f)
      ++const_cast<size_t &>(it->weight);
  }
}
HuffmanAlgorithm::TableOut HuffmanAlgorithm::table() const
{
  if (!chars)
    return HuffmanAlgorithm::TableOut();

  TreeFrequency tree;
  tree.merge(CharFrequency(*chars));

  std::vector<TreeFrequency::node_type> nodeStorage;
  nodeStorage.reserve(chars->size() * 2 - 2);
  while (tree.size() > 1) {
    nodeStorage.emplace_back(tree.extract(tree.cbegin()));
    nodeStorage.emplace_back(tree.extract(tree.cbegin()));
    tree.emplace(
        &nodeStorage.back().value(),
        &std::prev(nodeStorage.cend(), 2)->value());
  }
  return extractTable(*tree.cbegin());
}

HuffmanAlgorithm::TableOut
HuffmanAlgorithm::extractTable(const HuffmanAlgorithm::TreeNode &root) const
{
  TableOut table;
  extractTable(root, table, {});
  return std::move(table);
}
void HuffmanAlgorithm::extractTable(
    const HuffmanAlgorithm::TreeNode &root,
    HuffmanAlgorithm::TableOut &table,
    HuffmanAlgorithm::BitSet bitSet) const

{
  if (auto const maybeChar= std::get_if<Char>(&root.data)) {
    table.emplace(*maybeChar, std::move(bitSet));
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
