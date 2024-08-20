#pragma once

size_t max_size_t(size_t left, size_t right)
{
    return left < right ? right : left;
}
