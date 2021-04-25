//
// Created by dym on 25.04.2021.
//
#include <HuffmanAlgorithm.hpp>
#include <QBitArray>
#include <gtest/gtest.h>

#include <QDebug>

TEST(HuffmanAlgorithm, _1)
{
  HuffmanAlgorithm<char> h;
  h.store(std::string_view("beep boop beer!"));

  for (auto const &[s, b] : h.table<QBitArray>())
    qDebug().nospace() << s << ' ' << b;

//  qDebug() << ' ';
  for (auto const &[s, b] : h.table<std::vector<bool>>()) {
    auto out= qDebug().nospace();
    out << s << ' ';
    for (auto f : b) {
      out << (f ? 1 : 0);
    }
  }
}