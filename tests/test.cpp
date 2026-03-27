#include "stdafx.hpp" // include stdafx.hpp to ensure proper declarations
#include <gtest/gtest.h>
#include <string>

static std::string U(const char *s) { return s; }

static const unsigned char *do_cast(const char *s) {
    return reinterpret_cast<const unsigned char *>(s);
}

TEST(UnaccentLower, Basic) {
    auto [out, memown] =
        unaccent_lower_impl(do_cast("Olá, ááááééééiiiiiççççç"), 37);
    EXPECT_EQ(out, U("ola, aaaaeeeeiiiiiccccc"));
}

TEST(UnaccentLower, GermanEszett) {
    auto [out, memown] = unaccent_lower_impl(do_cast("Straße"), 7);
    EXPECT_EQ(out, U("strasse")); // casefold de ß => "ss"
}

TEST(UnaccentLower, GreekPiAndEmoji) {
    auto [out, memown] = unaccent_lower_impl(do_cast("π 😀"), 7);
    EXPECT_EQ(out, U("π 😀")); // não perde símbolos
}

TEST(UnaccentLower, CombiningMarks) {
    // "a" + U+0301 (acento agudo combinante)
    const char *in = "a\xCC\x81";
    auto [out, memown] = unaccent_lower_impl(do_cast(in), 3);
    EXPECT_EQ(out, U("a")); // STRIPMARK remove
}

TEST(Unaccent, BasicPreservesCase) {
    auto [out, memown] =
        unaccent_impl(do_cast("ÁRVORE, Açúcar e Ímã"),
                      static_cast<int>(sizeof("ÁRVORE, Açúcar e Ímã") - 1));
    EXPECT_EQ(out, U("ARVORE, Acucar e Ima"));
}

TEST(Unaccent, DottedCapitalI) {
    auto [out, memown] = unaccent_impl(
        do_cast("İstanbul"), static_cast<int>(sizeof("İstanbul") - 1));
    EXPECT_EQ(out, U("Istanbul"));
}

TEST(Unaccent, CombiningMarksPreserveCase) {
    // "A"/"a" com acento agudo combinante
    const char *in = "A\xCC\x81"
                     "a\xCC\x81"; // split to avoid hex overrun
    auto [out, memown] =
        unaccent_impl(do_cast(in), static_cast<int>(sizeof("A\xCC\x81"
                                                           "a\xCC\x81") -
                                                    1));
    EXPECT_EQ(out, U("Aa"));
}
