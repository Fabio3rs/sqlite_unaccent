#include "stdafx.hpp"
#include <cstddef>
#include <cstring>

extern "C" {
#include <utf8proc.h>
}

std::string unaccent_lower_impl(const unsigned char *in, int inlen) {
    if (!in)
        return std::string();

    utf8proc_uint8_t *out = nullptr;

    auto opts = static_cast<utf8proc_option_t>(
        UTF8PROC_COMPAT |    // NFK*
        UTF8PROC_STRIPMARK | // remove diacríticos
        UTF8PROC_CASEFOLD |  // casefold Unicode
        UTF8PROC_COMPOSE |   // volta a compor (NFC-ish)
        UTF8PROC_STABLE);

    // Uma passada: NFKD + remove acentos + casefold (+ recompor)
    utf8proc_ssize_t out_len =
        utf8proc_map(reinterpret_cast<const utf8proc_uint8_t *>(in),
                     static_cast<utf8proc_ssize_t>(inlen), &out, opts);

    if (out_len < 0) {
        if (out != nullptr) {
            free(out);
        }

        throw std::runtime_error(std::string(utf8proc_errmsg(out_len)) + "  " +
                                 std::to_string(opts));
    }

    std::string result(reinterpret_cast<char *>(out),
                       static_cast<size_t>(out_len));
    free(out);
    return result;
}
