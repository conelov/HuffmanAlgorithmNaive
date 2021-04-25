//
// Created by dym on 25.04.2021.
//

#ifndef HUFFMANALGORITHM_HUFFMANALGORITHM_HPP
#define HUFFMANALGORITHM_HUFFMANALGORITHM_HPP
#include <QBitArray>
#include <map>
#include <memory>
#include <set>
#include <variant>

class HuffmanAlgorithm {
public:
  using Char    = char;
  using BitSet  = QBitArray;
  using TableOut= std::map<Char, BitSet>;

private:
  class TreeNode;
  struct CompCharFrequency;
  struct CompTreeFrequency;

  using CharFrequency= std::set<TreeNode, CompCharFrequency>;
  using TreeFrequency= std::multiset<TreeNode, CompTreeFrequency>;

  std::unique_ptr<CharFrequency> chars;

public:
  ~HuffmanAlgorithm();
  HuffmanAlgorithm();
  void store(std::basic_string_view<Char> sv);
  [[nodiscard]] TableOut table() const;

private:
  [[nodiscard]] TableOut extractTable(TreeNode const &root) const;
  void extractTable(TreeNode const &root, TableOut &table, BitSet bitSet) const;
  //// beep boop beer!
};

#endif // HUFFMANALGORITHM_HUFFMANALGORITHM_HPP