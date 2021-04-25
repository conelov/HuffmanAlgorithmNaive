//
// Created by dym on 25.04.2021.
//

#ifndef HUFFMANALGORITHM_HUFFMANALGORITHM_HPP
#define HUFFMANALGORITHM_HUFFMANALGORITHM_HPP
#include <set>
#include <variant>
#include <vector>

template<typename CharIn>
class HuffmanAlgorithm {
public:
  using Char= CharIn;
  template<typename T>
  using Table= std::vector<std::pair<Char, T>>;

private:
  struct TreeNode
  {
    std::variant<Char, std::pair<TreeNode const *, TreeNode const *>> const
        data;
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
  struct CharFrequencyComp
  {
    bool operator()(TreeNode const lhs, TreeNode const rhs) const
    {
      return std::get<0>(lhs.data) < std::get<0>(rhs.data);
    }
  };
  struct TreeFrequencyComp
  {
    bool operator()(TreeNode const lhs, TreeNode const rhs) const
    {
      return lhs.weight <= rhs.weight;
    }
  };
  using CharFrequency= std::set<TreeNode, CharFrequencyComp>;
  using TreeFrequency= std::multiset<TreeNode, TreeFrequencyComp>;

  std::optional<CharFrequency> chars;

public:
  template<typename A>
  void store(A &&view)
  {
    if (!chars)
      chars.emplace();
    for (auto it= std::cbegin(view); it != std::cend(view); ++it) {
      auto [n, f]= chars->insert(TreeNode(*it, 1));
      if (!f)
        ++const_cast<size_t &>(n->weight);
    }
  }
  template<typename Bits>
  Table<Bits> table() const
  {
    using Table= Table<Bits>;
    if (!chars)
      return Table();

    TreeFrequency tree;
    tree.merge(CharFrequency(chars.value()));

    std::vector<typename TreeFrequency::node_type> nodeStorage;
    while (tree.size() > 1) {
      nodeStorage.emplace_back(tree.extract(tree.cbegin()));
      nodeStorage.emplace_back(tree.extract(tree.cbegin()));
      tree.emplace(
          &nodeStorage.back().value(),
          &std::prev(nodeStorage.cend(), 2)->value());
    }

    Table ret;
    extractTable(*tree.cbegin(), ret, Bits{});
    return std::move(ret);
  }

private:
  /// beep boop beer!

  template<typename A, typename Bits>
  void extractTable(TreeNode const root, A &table, Bits bitSet) const
  {
    if (auto const maybeChar= std::get_if<Char>(&root.data)) {
      table.emplace_back(*maybeChar, std::move(bitSet));
    } else {
      auto constexpr stepInBits= [](Bits &bits, bool f)
      {
        if constexpr (requires(Bits bits) { { bits.setBit(int{}, bool{}) }; }) {
          bits.resize(bits.size() + 1);
          bits.setBit(bits.size() - 1, f);
        } else {
          bits.push_back(f);
        }
      };
      {
        auto lBits= bitSet;
        stepInBits(lBits, true);
        extractTable(*std::get<1>(root.data).first, table, std::move(lBits));
      }
      stepInBits(bitSet, false);
      extractTable(*std::get<1>(root.data).second, table, std::move(bitSet));
    }
  }
};

#endif // HUFFMANALGORITHM_HUFFMANALGORITHM_HPP