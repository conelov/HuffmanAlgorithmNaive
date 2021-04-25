//
// Created by dym on 25.04.2021.
//
#include <HuffmanAlgorithm.hpp>
#include <QBitArray>
#include <QDebug>
#include <gtest/gtest.h>

TEST(HuffmanAlgorithm, _1)
{
  HuffmanAlgorithm h;
  h.store("beep boop beer!");
  for (auto const &[s, b] : h.table())
    qDebug().nospace() << s << ' ' << b;
}