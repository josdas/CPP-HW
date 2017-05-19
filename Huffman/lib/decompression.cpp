#include "decompress.h"
#include "stream_output.h"

Decompression::Decompression(vector<vector<int> > const& temp): 
	tree_(temp), 
	cur_vertex(tree_.get_root()) { }

void Decompression::decompress_block(Stream_output& stream, Stream_input& result) {
	while (!stream.end()) {
		while (cur_vertex >= SIZE_BLOCK && !stream.end()) {
			cur_vertex = tree_.get_next(cur_vertex, stream.get_next());
		}
		if (cur_vertex < SIZE_BLOCK) {
			result.add_byte(cur_vertex);
			cur_vertex = tree_.get_root();
		}
	}
}
