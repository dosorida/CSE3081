#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>
#include <functional>
#include <string>

#include "minheap.h"

using namespace std;

class compressor {
public:
	ifstream inputFileStream;
	ofstream outputFileStream;

	vector<bool> charBits[256];
	int charCount[256];
	
	node* huffRoot;

	compressor() { fill(charCount, charCount + 256, 0); };
	
	void compress(const string& inputfilename, const string& outputfilename);
	void charToBits();
	void printBits(vector<bool>& bits);
	void makeHuffmanTree();
	void printHuffmanTree();
	void dfs(node* curr, vector<bool>& bits);
};


inline void compressor::makeHuffmanTree() {
	char c;
	minheap mh(0);

	while (inputFileStream.get(c)) {
		charCount[(unsigned char)(c)] = charCount[(unsigned char)(c)] + 1;
	}

	for (int i = 0; i < 256; i++) {
		if (charCount[i] != 0) {
			mh.push(new node(i, charCount[i], nullptr, nullptr));
		}
	}

	while (mh.size > 1) {
		node* left = mh.pop();
		node* right = mh.pop();
		int parantfreq = left->freq + right->freq;
		node* parent = new node(0, parantfreq, left, right);
		mh.push(parent);
	}

	if (mh.size != 0) {
		huffRoot = mh.pop();
		if (huffRoot->left == nullptr && huffRoot->right == nullptr) {
			huffRoot = new node(0, huffRoot->freq, huffRoot, nullptr);
		}
	}
	else {
		inputFileStream.close();
		outputFileStream.close();
		exit(0);
	}
}

inline void compressor::charToBits() {
	vector<bool> bits;
	dfs(huffRoot, bits);
}

inline void compressor::dfs(node* curr, vector<bool>& bits) {
	if (curr->left == nullptr && curr->right == nullptr) {
		charBits[curr->c] = bits;
		return;
	}
	if (curr->left != nullptr) {
		bits.push_back(0);
		dfs(curr->left, bits);
		bits.pop_back();
	}
	if (curr->right != nullptr) {
		bits.push_back(1);
		dfs(curr->right, bits);
		bits.pop_back();
	}
}

inline void compressor::printHuffmanTree() {
	int charNum = 0;

	for (int i = 0; i < 256; i++) {
		int x = charCount[i] != 0;
		charNum += x;
	}

	outputFileStream << charNum << ' ';

	for (int i = 0; i < 256; i++) {
		if (charCount[i] != 0) {
			outputFileStream << char(i);
			outputFileStream << (char)(charCount[i] >> 16);
			outputFileStream << (char)(charCount[i] >> 8);
			outputFileStream << (char)(charCount[i]);
		}
	}
}

inline void compressor::printBits(vector<bool>& bits) {
	while (bits.size() % 8 != 0)
		bits.push_back(0);
	
	int i, j;

	for (i = 0; i + 7 < bits.size(); i += 8) {
		unsigned char wchar = 0;
		for (j = 0; j < 8; j++) {
			wchar <<= 1;
			wchar |= bits[i + j];
		}
		outputFileStream << wchar;
	}
}

inline void compressor::compress(const string& inputfilename, const string& outputfilename) {
	vector<bool>bits;
	char c;
	int bitsNum = 0;

	inputFileStream.open(inputfilename, ifstream::binary);
	if (!inputFileStream.is_open()) {
		cout << "The input file does not exist\n";
		return;
	}
	outputFileStream.open(outputfilename, ifstream::binary);
	if (!outputFileStream.is_open()) {
		cout << "Failed to open the output file\n";
		return;
	}

	makeHuffmanTree();
	printHuffmanTree();

	inputFileStream.close();

	charToBits();

	inputFileStream.open(inputfilename, ifstream::binary);

	while (inputFileStream.get(c)) {
		for (auto i : charBits[(unsigned char)(c)]) {
			bits.push_back(i);
			bitsNum = bitsNum + 1;
		}
	}

	outputFileStream << (char)((8 - bitsNum % 8) % 8);

	printBits(bits);
	inputFileStream.close();
	outputFileStream.close();
}