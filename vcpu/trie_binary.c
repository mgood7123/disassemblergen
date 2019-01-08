// C implementation of search and insert operations 
// on Trie 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <stdbool.h> 
#include "../Shell/builtins/printfmacro.h"
#include "../Shell/builtins/regex_str.h"


#define ARRAY_SIZE(a) sizeof(a)/sizeof(a[0]) 

// Alphabet size (# of symbols) 
#define ALPHABET_SIZE (26) 

// Converts key current character into index 
// use only 'a' through 'z' and lower case 
#define CHAR_TO_INDEX(c) ((int)c - (int)'a') 
#define INT_TO_INDEX(c) ((int)c - (int)'0')

// trie node 
struct TrieNode 
{ 
	struct TrieNode *children[ALPHABET_SIZE]; 
	int * idx;
	int count;
	// isEndOfWord is true if the node represents 
	// end of a word 
	bool isEndOfWord; 
	struct TrieNode * next;
}; 

// Returns new trie node (initialized to NULLs) 
struct TrieNode *getNode(void) 
{ 
	struct TrieNode *pNode = NULL; 

	pNode = (struct TrieNode *)malloc(sizeof(struct TrieNode)); 

	if (pNode) 
	{ 
		int i; 

		pNode->isEndOfWord = false; 
		pNode->idx = NULL;
		pNode->count = 0;
		for (i = 0; i < ALPHABET_SIZE; i++) 
			pNode->children[i] = NULL; 
	} 

	return pNode; 
} 

// If not present, inserts key into trie 
// If the key is prefix of trie node, just marks leaf node 
void insert(struct TrieNode *root, const char *key, int dest_idx, struct TrieNode *next)
{ 
	int level; 
	int length = strlen(key); 
	int index; 

	struct TrieNode *pCrawl = root;
// 	pi(dest_idx)
	for (level = 0; level < length; level++) 
	{ 
// 		pi(level);
// 		pc(key[level])
		str_new(ch);
		str_insert_char(ch, key[level]);
		if (ch.type & STR_TYPE_DIGIT) index = INT_TO_INDEX(key[level]);
		else if (ch.type & STR_TYPE_ALPHABETIC) index = CHAR_TO_INDEX(key[level]);
		str_free(ch)
// 		pi(index)
// 		pp(pCrawl->children[index])
		if (!pCrawl->children[index]) {
			pCrawl->children[index] = getNode();
		}
		
		if (level == length-1) {
// 			pp(pCrawl->children[index])
// 			pp(pCrawl->children[index]->idx)
			pCrawl->children[index]->next = next;
			pCrawl->children[index]->count++;
			if (!pCrawl->children[index]->idx) {
				pCrawl->children[index]->idx = malloc(sizeof(int*)*pCrawl->children[index]->count);
// 				pp(pCrawl->children[index]->idx)
			}
			else {
				pCrawl->children[index]->idx = realloc(pCrawl->children[index]->idx, sizeof(int*)*pCrawl->children[index]->count);
			}
			pCrawl->children[index]->idx[pCrawl->children[index]->count-1] = dest_idx;
// 			pi(pCrawl->children[index]->idx[pCrawl->children[index]->count-1])
		}
		
		pCrawl = pCrawl->children[index]; 
	} 

	// mark last node as leaf 
	pCrawl->isEndOfWord = true; 
} 

// Returns true if key presents in trie, else false 
bool search(struct TrieNode **root, const char *key, int idx) 
{ 
	int level; 
	int length = strlen(key); 
	int index; 
	struct TrieNode *pCrawl = *root; 
// 	pi(idx);
	for (level = 0; level < length; level++) 
	{
		str_new(ch);
		str_insert_char(ch, key[level]);
		if (ch.type & STR_TYPE_DIGIT) index = INT_TO_INDEX(key[level]);
		else if (ch.type & STR_TYPE_ALPHABETIC) index = CHAR_TO_INDEX(key[level]);
		str_free(ch)
		if (!pCrawl->children[index]) {
			index = CHAR_TO_INDEX('x');
			if (!pCrawl->children[index]) return false; 
		}

		pCrawl = pCrawl->children[index]; 
// 		pp(pCrawl->next);
		if (pCrawl->next) {
// 			pi(pCrawl->next->idx)
// 			pp(*root)
			*root = pCrawl->next;
// 			pp(*root)
// 			bool result = search(pCrawl->next, key, idx);
// 			pb(result);
		}
	} 

// 	pi(pCrawl->count)
	bool final_match = false;
	for (int i = 0; i < pCrawl->count; i++) {
// 		pi(pCrawl->idx[i])
		if (pCrawl->idx[i] == idx) final_match = true;
	}
// 	pb(final_match)
	return (pCrawl != NULL && pCrawl->isEndOfWord && final_match); 
} 

// Returns true if key presents in trie, else false 
bool search_segmented(struct TrieNode *root, struct TrieNode *segments, const char *key) 
{ 
	int level; 
	int length = strlen(key); 
	int index; 
	struct TrieNode *pCrawl = root; 
	
	str_new(segment);
	int idx = 0;

	for (level = 0; level < length; level++) 
	{
		str_insert_char(segment, key[level]);
		
		ps(segment.string);
// 		pp(segments)
		bool result = search(&segments, segment.string, idx);
// 		pp(segments)
		pb(result);
		if (result) {
			str_reset(segment);
			idx++;
		}
		str_new(ch);
		str_insert_char(ch, key[level]);
		if (ch.type & STR_TYPE_DIGIT) index = INT_TO_INDEX(key[level]);
		else if (ch.type & STR_TYPE_ALPHABETIC) index = CHAR_TO_INDEX(key[level]);
		str_free(ch)

		if (!pCrawl->children[index]) {
			index = CHAR_TO_INDEX('x');
			if (!pCrawl->children[index]) return false; 
		}

		pCrawl = pCrawl->children[index]; 
	} 
	str_free(segment);
	return (pCrawl != NULL && pCrawl->isEndOfWord); 
} 

#define isp(root, segments, c) printf("%s --- %s\n", c, output[search_segmented(root, segments, c)] )

// Driver 
int main() 
{ 
	char output[][32] = {"Not present in trie", "Present in trie"}; 

/*
	// Construct trie 
	insert(segments, "0011", 0, segments1);
	pp(segments->next)
	insert(segments1, "001", 1, NULL);
	insert(segments1, "0", 2, NULL);
	insert(segments1, "0", 3, NULL);
	insert(root, "001100100", 0, NULL);

	// Search for different keys 
	isp(root, segments, "001100100");
*/

	// Construct trie
	
/* shall aim to be equivilant to this

rule_1 ::= "1" "0";
rule_2 ::= "0" "1";
rule_3 ::= "0" "0";

*/
	// rule 1
	
	// when adding a new rule indexing resets to 0
	
	struct TrieNode *root = getNode(); 
	puts("adding rule 1: segments [0, 1, 0] root [010]");
	insert(root, "010", 0, NULL);
	struct TrieNode *sub_root = getNode(); 
	struct TrieNode *rule_1 = getNode();
	struct TrieNode *rule_2 = getNode(); 
	insert(sub_root, "0", 0, rule_1);
		insert(rule_1, "1", 1, rule_2);
		insert(rule_2, "0", 2, NULL);
	
	puts("adding rule 2: segments [0, 1, 1] root [011]");
	insert(root, "011", 0, NULL);
	struct TrieNode *rule_3 = getNode(); 
	struct TrieNode *rule_4 = getNode(); 
	insert(sub_root, "0", 0, rule_3);
		insert(rule_3, "1", 1, rule_4);
		insert(rule_4, "1", 2, NULL);
		
	// Search for different keys
	isp(root, sub_root, "010");
	isp(root, sub_root, "011");
	return 0; 
} 

/*

adding rule 1: segments [0, 1, 0] root [010]
adding rule 2: segments [0, 1, 1] root [011]
segment.string = 0
result = true
segment.string = 1
result = true
segment.string = 0
result = false
010 --- Present in trie
segment.string = 0
result = true
segment.string = 1
result = true
segment.string = 1
result = true
011 --- Present in trie

*/
