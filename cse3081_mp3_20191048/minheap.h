#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>
#include <functional>

using namespace std;

class node {
public:
	node* left;
	node* right;
	unsigned char c;
	int freq;

	node(const char c, const int freq, node* l, node* r) : left(l), right(r), c(c), freq(freq) {};
	node() = default;

	bool operator < (const node& x) const {
		bool result;
		if (this->freq == x.freq)
			result = this->c < x.c;
		else
			result = this->freq < x.freq;
		return result;
	}
};

using dataType = node * ;

class minheap {
public:
	vector<dataType>tree;
	int size;
	minheap() : size(0) {};
	minheap(const int size) : tree(size + 1), size(size) {};
	dataType pop();
	void push(const dataType e);
};

dataType minheap::pop() {
	if (size == 0)
		return nullptr;

	const dataType top = tree[1];
	tree[1] = tree[size];
	size = size - 1;

	int curr = 1;
	int next = curr;

	while (curr * 2 <= size) {
		if (*tree[curr * 2 + 1] < *tree[curr * 2] && curr * 2 + 1 <= size) {
			next = curr * 2;
			next++;
		}
		else {
			next = curr * 2;
		}
		bool cmp = *tree[next] < *tree[curr];
		if (cmp) {
			swap(tree[next], tree[curr]);
			curr = next;
		}
		else {
			break;
		}
	}
	return top;
}

void minheap::push(const dataType newElem) {
	size = size + 1;

	if (tree.size() < size + 1) {
		tree.resize(size + 1);
	}

	int curr = size;
	tree[size] = newElem;

	while (curr != 1) {
		bool cmp = *tree[curr] < *tree[curr / 2];
		if (cmp)
			swap(tree[curr], tree[curr / 2]);
		else
			break;
		curr /= 2;
	}
}