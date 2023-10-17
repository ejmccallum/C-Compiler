#pragma once
#include <string>
#include <iostream>

using namespace std;

string filename;
int linenumber;
bool success=true;

bool error(string message) {
  cerr << filename <<':'<<linenumber<<':'<< 1 << " error "<< message<< endl;
  return false;
}