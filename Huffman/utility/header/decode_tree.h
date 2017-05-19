#pragma once
#include "my_ifstream.h"
#include <decompress.h>

vector<vector<int> > read_code(my_ifstream& ifstream, int64_t& r);
