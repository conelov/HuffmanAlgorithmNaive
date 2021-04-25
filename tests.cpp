//
// Created by dym on 25.04.2021.
//
#include <HuffmanAlgorithm.hpp>
#include <QBitArray>
#include <QDebug>
#include <gtest/gtest.h>

TEST(HuffmanAlgorithm, _1)
{
  TreeFrequency tree;
  size_t uniqueCharCount;
  {
    CharFrequency source(fromString("beep boop beer!"));

    uniqueCharCount= source.size();

    tree.merge(std::move(source));
  }

  for (auto const &i : tree)
    qDebug() << i.weight << ' ' << std::get<0>(i.data) << '\n';

  std::vector<decltype(tree)::node_type> nodeStorage;
  nodeStorage.reserve(uniqueCharCount * 2 - 2);
  while (tree.size() > 1) {
    nodeStorage.emplace_back(tree.extract(tree.cbegin()));
    nodeStorage.emplace_back(tree.extract(tree.cbegin()));
    tree.emplace(
        &nodeStorage.back().value(),
        &std::prev(nodeStorage.cend(), 2)->value());
  }

  auto const table= extractTable(*tree.cbegin());
  for (auto const &[s, b] : table)
    qDebug() << s << ' ' << b << '\n';
}