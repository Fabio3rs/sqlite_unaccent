#include "stdafx.hpp"
#include <cstddef>
#include <cstring>
#include <memory>
#include <string_view>

extern "C" {
#include <utf8proc.h>
}

#include <stdexcept>

std::pair<std::string_view, utf8owingptr_t>
unaccent_lower_impl(const unsigned char *in, int inlen) {
    if (!in) {
        return {};
    }

    utf8owingptr_t out;

    auto opts = static_cast<utf8proc_option_t>(
        UTF8PROC_COMPAT |    // NFK*
        UTF8PROC_STRIPMARK | // remove diacríticos
        UTF8PROC_CASEFOLD |  // casefold Unicode
        UTF8PROC_COMPOSE |   // volta a compor (NFC-ish)
        UTF8PROC_STABLE);

    // Uma passada: NFKD + remove acentos + casefold (+ recompor)
    utf8proc_ssize_t out_len = utf8proc_map(
        reinterpret_cast<const utf8proc_uint8_t *>(in),
        static_cast<utf8proc_ssize_t>(inlen), std::out_ptr(out), opts);

    if (out_len < 0) {
        throw std::runtime_error(std::string(utf8proc_errmsg(out_len)) + "  " +
                                 std::to_string(opts));
    }

    std::string_view result(reinterpret_cast<char *>(out.get()),
                            static_cast<size_t>(out_len));
    return {result, std::move(out)};
}
