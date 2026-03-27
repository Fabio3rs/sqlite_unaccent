#pragma once

#include "free_wrap.hpp"

extern "C" {
#include <utf8proc.h>
}

#include <memory>
#include <string_view>
#include <utility>

using utf8owingptr_t = std::unique_ptr<utf8proc_uint8_t, freewrapper>;

std::pair<std::string_view, utf8owingptr_t>
unaccent_lower_impl(const unsigned char *in, int inlen);
