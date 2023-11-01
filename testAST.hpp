void testAST() {
  ASTNode *three=new ASTNode(Token(INTEGER,"3"));
  ASTNode *c=new ASTNode(Token(VARIABLE,"c"));
  ASTNode *mul=new ASTNode(Token(MULTIPLYING,"*"),three,c);
  ASTNode *a=new ASTNode(Token(VARIABLE,"a"));
  ASTNode *lessEqual=new ASTNode(Token(RELATIONAL,"<="),mul,a);
  ASTNode *root=lessEqual;
  cout << root;
}


void testAST2() {
  ASTNode *four = new ASTNode(Token(INTEGER, "4"));
  ASTNode *e = new ASTNode(Token(VARIABLE, "e"));
  ASTNode *mul = new ASTNode(Token(MULTIPLYING, "*"), e, four);
  ASTNode *d = new ASTNode(Token(VARIABLE, "d"));
  ASTNode *sub = new ASTNode(Token(ADDING, "-"), d, mul);
  ASTNode *root = sub;
  cout << root;
  cout << endl;
}

void testAST3() {
  ASTNode *five = new ASTNode(Token(INTEGER, "5"));
  ASTNode *two = new ASTNode(Token(INTEGER, "2"));
  ASTNode *plus = new ASTNode(Token(ADDING, "+"), five, two);
  ASTNode *b = new ASTNode(Token(VARIABLE, "b"));
  ASTNode *div = new ASTNode(Token(MULTIPLYING, "/"), plus, b);
  ASTNode *root = div;
  cout << root;
}