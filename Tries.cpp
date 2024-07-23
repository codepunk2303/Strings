struct Node{
	int stringsEndingHere,stringsEndingBelow;
	vector<int>child;

	Node(){
		stringsEndingHere = 0;
		stringsEndingBelow = 0;
		child.resize(26);
		fill(child.begin(),child.end(),-1);
	}

	bool canDelete(){
		for(int i = 0;i<26;i++){
			if(child[i]!=-1)return false;
		}

		return stringsEndingHere<=0;
	}
};

struct Trie{
	int root;
	vector<Node>nodes;

	Trie(){
		root = 0;
		nodes.emplace_back();
	}

	void add(string &s){
		int curr = root;
		for(char ch:s){
			int index = ch-'a';
			if(nodes[curr].child[index] == -1){
				nodes[curr].child[index] = nodes.size();
				nodes.emplace_back();
			}
			curr = nodes[curr].child[index];
		}

		nodes[curr].stringsEndingHere++;
	}

	bool search(string &s){
		int curr = root;
		for(char ch:s){
			int index = ch-'a';
			if(nodes[curr].child[index] == -1)return false;
			curr = nodes[curr].child[index];
		}

		return nodes[curr].stringsEndingHere>0;
	}

	bool deleterec(string &s,int curr,int index){
		if(index == s.size()){
			nodes[curr].stringsEndingHere--;
			nodes[curr].stringsEndingBelow--;
			return nodes[curr].canDelete();
		}

		int childNode = nodes[curr].child[s[index]-'a'];
		bool childDeleted = deleterec(s,childNode,index+1);
		if(childDeleted){
			nodes[curr].child[s[index]-'a'] = -1;
		}
		nodes[curr].stringsEndingBelow--;
		return nodes[curr].canDelete();
	}

	void deleteString(string &s){
		if(!search(s))return;

		deleterec(s,root,0);
	}

	void PrecomputeSpecialNodesBelow(int curr){
		nodes[curr].stringsEndingBelow = 0;
		for(int i = 0;i<26;i++){
			if(nodes[curr].child[i]!= -1){
				nodes[curr].stringsEndingBelow += nodes[nodes[curr].child[i]].stringsEndingBelow;
			}
		}

		nodes[curr].stringsEndingBelow += nodes[curr].stringsEndingHere;
	}

	void addrec(string &s,int curr,int index){
		if(index == s.size()){
			nodes[curr].stringsEndingHere++;
			PrecomputeSpecialNodesBelow(curr);
			return;
		}

		int childNode = nodes[curr].child[s[index]-'a'];
		if(childNode == -1){
			nodes[curr].child[s[index]-'a'] = nodes.size();
			nodes.emplace_back();
			childNode = nodes[curr].child[s[index]-'a'];
		}
		addrec(s,childNode,index+1);
		PrecomputeSpecialNodesBelow(curr);
	}

	//addString for common prefix

	void addString(string &s){
		addrec(s,root,0);
	}

	int commonPrefix(string &s){
		int curr = root;
		for(char ch:s){
			if(nodes[curr].child[ch-'a'] == -1)return 0;
			curr = nodes[curr].child[ch-'a'];
		}
		return nodes[curr].stringsEndingBelow;
	}
};