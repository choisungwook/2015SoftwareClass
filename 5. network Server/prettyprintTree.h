#ifndef __PRETTY_PRINT_H__
#define __PRETTY_PRINT_H__

#include "avltree.h"
#include <deque>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
using namespace std;

int maxHeight(BTreeNode *p);
string intToString(int val);
void printBranches(int branchLen, int nodeSpaceLen, int startLen, int nodesInThisLevel, const deque<BTreeNode*>& nodesQueue, ostream& out);
void printPretty(BTreeNode *root, int level, int indentSpace, ostream& out);

#endif
