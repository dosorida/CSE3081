#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <cstring>
#include <algorithm>
#include <functional>

#include "compress.h"
#include "decompress.h"

using namespace std;

int main(int argc, char **argv) {
	cin.tie(NULL);
	ios_base::sync_with_stdio(false);

	string inputfilename = argv[2];
	string option = argv[1];

	if (option[0] == '-' && option[1] == 'c') {
		compressor myCompressor;
		myCompressor.compress(inputfilename, inputfilename + ".zz");
	}
	else if (option[0] == '-' && option[1] == 'd') {
		decompressor myDecompressor;
		myDecompressor.decompress(inputfilename, inputfilename + ".yy");
	}
	else {
		cout << "The options are -c or -d\n";
		return 1;
	}
	return 0;
}