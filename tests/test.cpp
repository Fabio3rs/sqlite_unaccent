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
