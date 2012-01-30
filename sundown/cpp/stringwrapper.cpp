#include "stringwrapper.hpp"

#include <string.h>

#include <iostream>

extern "C" {
#include "markdown.h"
#include "html.h"
#include "buffer.h"
}

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>


std::string markdownString2String(const std::string& s) {
    struct buf *ib, *ob;

    struct sd_callbacks callbacks;
    struct html_renderopt options;
    struct sd_markdown *markdown;

    size_t byteCount = s.length();

    ib = bufnew(byteCount);
    ib->size = byteCount;
    bufgrow(ib, byteCount);

    memcpy(ib->data, s.c_str(), byteCount);

    const size_t OUTPUT_UNIT = 64;

    ob = bufnew(OUTPUT_UNIT);

    sdhtml_renderer(&callbacks, &options, 0);
    markdown = sd_markdown_new(0, 16, &callbacks, &options);

    sd_markdown_render(ob, ib->data, ib->size, markdown);
    sd_markdown_free(markdown);

    fwrite(ob->data, 1, ob->size, stderr);
    std::string ret(reinterpret_cast<char*>(ob->data), ob->size);

    bufrelease(ib);
    bufrelease(ob);

    return ret;
}
