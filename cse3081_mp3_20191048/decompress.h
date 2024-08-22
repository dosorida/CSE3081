#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>
#include <functional>
#include <string>
#include <cmath>

#include "minheap.h"

using namespace std;

class decompressor {
public:
	ifstream inputFileStream;
	ofstream outputFileStream;
	
	vector<bool> charBits[256];
	node* huffRoot;

	decompressor() = default;

	void decompress(const string& inputfilename, const string& outputfilename);
	void bitsToChar();
	void printBits(const vector<bool>& bits);
	void makeHuffmanTree();
	void dfs(node* curr, vector<bool>& bits);
};

inline void decompressor::makeHuffmanTree()
{
	minheap mh(0);
	int freq;
	int charNum = 0;
	char c, toint;

	inputFileStream >> charNum;
	inputFileStream.get();
	
	for (int i = 0; i < charNum; i++) {
		inputFileStream.get(c);

		freq = 0;
		inputFileStream.get(toint);
		freq += (unsigned char)toint;
		freq <<= 8;

		inputFileStream.get(toint);
		freq += (unsigned char)toint;
		freq <<= 8;

		inputFileStream.get(toint);
		freq += (unsigned char)toint;

		mh.push(new node(c, freq, nullptr, nullptr));
	}

	while (mh.size > 1) {		
		node* left = mh.pop();
		node* right = mh.pop();
		node* parent = new node(0, left->freq + right->freq, left, right);
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

inline void decompressor::bitsToChar() {
	vector<bool> bits;
	dfs(huffRoot, bits);
}

inline void decompressor::dfs(node* curr, vector<bool>& bits) {
	if (curr->right == nullptr && curr->left == nullptr) {
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

inline void decompressor::printBits(const vector<bool>& bits) {
	node* curr = huffRoot;
	for (int i = 0; i < bits.size(); i++) {
		if (bits[i] == false) {
			curr = curr->left;
		}
		else {
			curr = curr->right;
		}
		if (curr->left == nullptr && curr->right == nullptr) {
			outputFileStream << curr->c;
			curr = huffRoot;
		}
	}
}

inline void decompressor::decompress(const string& inputfilename, const string& outputfilename)
{
	int del = 0;
	char c;

	vector<bool>readBits;

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
	bitsToChar();

	inputFileStream.get(c);
	del = c;

	while(inputFileStream.get(c)) {
		unsigned char uc = c;
		for (int j = 0; j < 8; j++) {
			readBits.push_back(uc / 128);
			uc <<= 1;
		}
	}
	while (del--) {
		readBits.pop_back();
	}

	printBits(readBits);

	inputFileStream.close();
	outputFileStream.close();
}